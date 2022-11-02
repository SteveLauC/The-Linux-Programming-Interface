use inotify::{Event, EventMask, Inotify, WatchDescriptor, WatchMask};
use walkdir::WalkDir;

use std::{
    ffi::OsStr,
    path::{Path, PathBuf},
    process::exit,
};

/// A mapping entry representing the relationship between a `watch descriptor`
/// and the path of the watched directory.
#[derive(Debug)]
pub struct Entry {
    /// watch descriptor
    pub wd: WatchDescriptor,
    /// path of the watched directory
    pub watched_file: PathBuf,
    /// if this watched directory is the ROOT of our watching tree
    pub is_root: bool,
}

impl Entry {
    /// Construct a new [`Entry`]
    pub fn new<P: AsRef<Path>>(wd: WatchDescriptor, watched_file_path: P, is_root: bool) -> Self {
        Self {
            wd,
            watched_file: watched_file_path.as_ref().to_owned(),
            is_root,
        }
    }
}

/// A collection of mapping [`Entry`]s.
#[derive(Debug)]
pub struct Mappings {
    inner: Vec<Entry>,
}

impl Mappings {
    /// Construct an empty collection.
    pub fn new() -> Self {
        Self {
            inner: Vec::with_capacity(5),
        }
    }

    /// Does this `watch descriptor` refers the ROOT of our watching tree
    #[inline]
    pub fn is_root(&self, wd: &WatchDescriptor) -> bool {
        self.inner
            .iter()
            .find(|map| map.wd == *wd)
            .expect("fatal error: The wd to be queries does not exist in the mapping")
            .is_root
    }

    /// Add a new mapping [`Entry`] to our collection.
    #[inline]
    pub fn add(&mut self, entry: Entry) {
        self.inner.push(entry);
    }

    /// Try to remove the [`Entry`] that has `watch descriptor` `wd` from our
    /// collection if it exists.
    pub fn try_to_remove(&mut self, wd: &WatchDescriptor) {
        if let Some(idx) = self.inner.iter().position(|map| map.wd == *wd) {
            self.inner.swap_remove(idx);
        }
    }

    /// Get the `watched_file` field of the [`Entry`]
    #[inline]
    pub fn get_watched_file_path(&self, wd: &WatchDescriptor) -> &Path {
        &self
            .inner
            .iter()
            .find(|entry| entry.wd == *wd)
            .expect("fatal error: The wd to be queries does not exist in the mapping")
            .watched_file
    }
}

/// Handle event
pub fn handle_event(
    event: &Event<&OsStr>,
    inotify: &mut Inotify,
    mapping: &mut Mappings,
    mask: WatchMask,
) {
    let returned_mask = event.mask;
    let wd = event.wd.clone();

    if returned_mask.contains(EventMask::CREATE) {
        let watched_directory = mapping.get_watched_file_path(&wd);
        let path = watched_directory.join(event.name.unwrap());

        for entry in WalkDir::new(path.as_path())
            .into_iter()
            .filter_map(|entry| entry.ok())
        {
            println!("{:?} was created", entry.path());

            // watch all the directories in this tree.
            if entry.metadata().unwrap().is_dir() {
                let wd = inotify
                    .add_watch(entry.path(), mask)
                    .expect("inotify_add_watch(2)");
                mapping.add(Entry::new(wd, entry.path(), false));
            }
        }
    }

    if returned_mask.contains(EventMask::DELETE) {
        println!("{:?} was deleted", event.name.unwrap());
    }

    if returned_mask.contains(EventMask::DELETE_SELF) {
        let watched_directory = mapping.get_watched_file_path(&wd);

        if mapping.is_root(&wd) {
            println!("Root {:?} was deleted", watched_directory);
            // Root is gone, exit the program.
            exit(0);
            // Here is a new directory, we need to watch it.
        } else {
            // remove it from the mapping
            mapping.try_to_remove(&wd);
        }
    }

    if returned_mask.contains(EventMask::MOVE_SELF) {
        let watched_directory = mapping.get_watched_file_path(&wd);

        if mapping.is_root(&wd) {
            println!("Root {:?} was moved", watched_directory);
            // Root is gone, exit the program.
            exit(0);
        } else {
            inotify.rm_watch(wd.clone()).expect("inotify_rm_watch(2)");
            mapping.try_to_remove(&wd);
        }
    }

    if returned_mask.contains(EventMask::MOVED_FROM) {
        println!("{:?} was moved", event.name.unwrap());
    }

    if returned_mask.contains(EventMask::MOVED_TO) {
        let watched_directory = mapping.get_watched_file_path(&wd);
        let path = watched_directory.join(event.name.unwrap());

        println!(
            "{:?} was moved to {:?}",
            event.name.unwrap(),
            watched_directory
        );
        let metadata = path.symlink_metadata().unwrap();

        // If the new file is a directory, watch it and its subdirectories.
        if metadata.is_dir() {
            for dir in WalkDir::new(path.as_path())
                .into_iter()
                .filter_map(|entry| entry.ok())
                .filter(|entry| entry.metadata().unwrap().is_dir())
            {
                let wd = inotify
                    .add_watch(dir.path(), mask)
                    .expect("inotify_add_watch(2)");
                mapping.add(Entry::new(wd, dir.path(), false));
            }
        }
    }
}
