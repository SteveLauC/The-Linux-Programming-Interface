mod utility;

use inotify::{Inotify, WatchMask};
use std::{env::args_os, ffi::OsString, process::exit};
use utility::{handle_event, Entry, Mappings};
use walkdir::WalkDir;

fn main() {
    let mut inotify = Inotify::init().expect("inotify_init1(2)");
    let mut mapping = Mappings::new();
    let mask = WatchMask::EXCL_UNLINK
        | WatchMask::CREATE
        | WatchMask::DELETE
        | WatchMask::DELETE_SELF
        | WatchMask::MOVE
        | WatchMask::MOVE_SELF;

    let dirs = args_os().skip(1).collect::<Vec<OsString>>();
    if dirs.is_empty() {
        exit(0);
    }

    for dir in dirs {
        let root_wd = inotify
            .add_watch(dir.as_os_str(), mask)
            .expect("inotify_add_watch(2)");
        mapping.add(Entry::new(root_wd, dir.as_os_str(), true));

        let walker = WalkDir::new(dir.as_os_str());
        for dir in walker.into_iter().skip(1)
            .filter_map(|entry| entry.ok())
            .filter(|entry| entry.metadata().unwrap().is_dir())
        {
            let path = dir.path();
            let wd = inotify.add_watch(path, mask).expect("inotify_add_watch(2)");
            mapping.add(Entry::new(wd, path, false));
        }
    }

    let mut buf = [0; 1024];
    loop {
        let events = inotify.read_events_blocking(&mut buf).expect("read(2)");
        for event in events {
            handle_event(&event, &mut inotify, &mut mapping, mask);
        }
    }
}
