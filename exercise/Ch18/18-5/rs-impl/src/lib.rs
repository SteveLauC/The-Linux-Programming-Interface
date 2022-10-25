use std::{
    collections::VecDeque,
    env::set_current_dir,
    ffi::OsString,
    fs::{metadata, read_dir, File},
    io::Result,
    os::{linux::fs::MetadataExt, unix::io::AsRawFd},
    path::PathBuf,
};

use libc::{fchdir, PATH_MAX};

/// A CWD composer, returns the next path entry
///
/// ##### Example
/// * `/home/steve` -> `Some(Ok("steve"))`
/// * `/home` -> `Some(Ok("home"))`
/// * `/` -> `None`
struct Composer;

impl Composer {
    fn new() -> Self {
        Composer
    }
}

impl Iterator for Composer {
    type Item = Result<OsString>;

    fn next(&mut self) -> Option<Self::Item> {
        let cur_dir_metadata = match metadata(".") {
            Ok(m) => m,
            Err(e) => return Some(Err(e)),
        };
        let parent_metadata = match metadata("..") {
            Ok(m) => m,
            Err(e) => return Some(Err(e)),
        };

        // reaches root
        if cur_dir_metadata.st_dev() == parent_metadata.st_dev()
            && cur_dir_metadata.st_ino() == parent_metadata.st_ino()
        {
            return None;
        }

        // cd to parent dir
        if let Err(e) = set_current_dir("..") {
            return Some(Err(e));
        }

        let dir = match read_dir(".") {
            Ok(dir) => dir,
            Err(e) => return Some(Err(e)),
        };

        for res_entry in dir {
            let entry = match res_entry {
                Ok(entry) => entry,
                Err(e) => return Some(Err(e)),
            };

            let entry_metadata = match entry.metadata() {
                Ok(m) => m,
                Err(e) => return Some(Err(e)),
            };

            if entry_metadata.st_dev() == cur_dir_metadata.st_dev()
                && entry_metadata.st_ino() == cur_dir_metadata.st_ino()
            {
                return Some(Ok(entry.file_name()));
            }
        }
        unreachable!("This is unreachable unless there is something wrong with your file system");
    }
}

pub fn getcwd() -> Result<PathBuf> {
    let cwd = File::open(".")?;

    let comp = Composer::new();
    let mut entries = VecDeque::new();
    for item in comp {
        entries.push_front(item?);
    }

    let mut res = OsString::with_capacity(PATH_MAX as usize);
    entries.iter().for_each(|entry| {
        res.push("/");
        res.push(entry);
    });
    res.shrink_to_fit();

    unsafe { fchdir(cwd.as_raw_fd()) };
    Ok(PathBuf::from(res))
}

#[cfg(test)]
mod test {
    use super::getcwd;
    use std::env::current_dir;

    #[test]
    fn test() {
        let cwd = current_dir().unwrap();
        let res = getcwd().unwrap();
        assert_eq!(cwd, res);
    }
}
