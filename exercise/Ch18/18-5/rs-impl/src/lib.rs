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
/// * `/home/steve` -> `Ok(Some("steve"))`
/// * `/home` -> `Ok(Some("home"))`
/// * `/` -> `Ok(None)`
struct Composer;

impl Composer {
    fn new() -> Self {
        Composer
    }

    fn next_component(&mut self) -> Result<Option<OsString>> {
        let cur_dir_metadata = metadata(".")?;
        let parent_metadata = metadata("..")?;

        // reaches root
        if cur_dir_metadata.st_dev() == parent_metadata.st_dev()
            && cur_dir_metadata.st_ino() == parent_metadata.st_ino()
        {
            return Ok(None);
        }

        // cd to the parent directory
        set_current_dir("..")?;

        let dir = read_dir(".")?;

        for res_entry in dir {
            let entry = res_entry?;
            let entry_metadata = entry.metadata()?;

            if entry_metadata.st_dev() == cur_dir_metadata.st_dev()
                && entry_metadata.st_ino() == cur_dir_metadata.st_ino()
            {
                return Ok(Some(entry.file_name()));
            }
        }
        unreachable!("This is unreachable unless there is something wrong with your file system");
    }
}


pub fn getcwd() -> Result<PathBuf> {
    let cwd = File::open(".")?;

    let mut comp = Composer::new();
    let mut entries = VecDeque::new();
    loop {
        let opt = comp.next_component()?;
        match opt {
            Some(entry) => entries.push_front(entry),
            // reaches root, exit the loop
            None => break,
        }
    }

    let mut res = OsString::with_capacity(PATH_MAX as usize);
    entries.iter().for_each(|entry| {
        res.push("/");
        res.push(entry);
    });


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
