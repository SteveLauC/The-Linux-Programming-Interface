use crate::utility::{is_a_pair_of_dots, is_dot, Paths};
use std::{
    env::current_dir,
    io::Result,
    path::{Path, PathBuf},
};

pub fn my_realpath<P: AsRef<Path>>(path: P) -> Result<PathBuf> {
    let cwd = current_dir().expect("can not get cwd");
    let mut paths = Paths::new(
        if path.as_ref().is_absolute() {
            None
        } else {
            Some(cwd)
        },
        Some(path),
    );

    while let Some(entry) = paths.remained_next_entry() {
        if is_dot(entry.as_os_str()) {
            continue;
        } else if is_a_pair_of_dots(entry.as_os_str()) {
            paths.parsed_cd_to_parent();
        } else {
            paths.parsed_push_back(entry);
        }

        if paths.parsed.is_symlink() {
            let link_content = paths.parsed.read_link().expect("can not follow symlink");
            if link_content.is_absolute() {
                let clean_link: PathBuf = link_content.components().collect();
                paths.replace_parsed_with(clean_link);
            } else {
                paths.parsed_cd_to_parent();
                paths.remained_push_front(link_content);
            }
        }
    }

    Ok(paths.parsed.clone())
}

#[cfg(test)]
mod test {
    use super::my_realpath;
    use std::{env::current_dir, path::Path};

    #[test]
    fn test1() {
        let res1 = my_realpath("/..");
        assert_eq!(res1.unwrap(), Path::new("/"));
    }

    #[test]
    fn test2() {
        let res2 = my_realpath("/../test");
        assert_eq!(res2.unwrap(), Path::new("/test"));
    }

    #[test]
    fn test3() {
        let cwd = current_dir().expect("can not get cwd");
        let res3 = my_realpath("test/..");
        assert_eq!(res3.unwrap(), cwd);
    }

    #[test]
    fn test4() {
        let mut cwd = current_dir().expect("can not get cwd");
        let res4 = my_realpath("test/path/..");
        cwd.push("test");

        assert_eq!(res4.unwrap(), cwd);
    }

    #[test]
    fn test5() {
        let mut cwd = current_dir().expect("can not get cwd");
        let res5 = my_realpath("test/../path");
        cwd.push("path");

        assert_eq!(res5.unwrap(), cwd);
    }

    #[test]
    fn test6() {
        let mut cwd = current_dir().expect("can not get cwd");
        let res6 = my_realpath("test/../path");
        cwd.push("path");

        assert_eq!(res6.unwrap(), cwd);
    }
}
