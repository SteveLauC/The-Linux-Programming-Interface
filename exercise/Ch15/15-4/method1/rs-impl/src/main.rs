use nix::{
    libc::{gid_t, uid_t},
    unistd::{
        access, getegid, geteuid, getgid, getuid, setresgid, setresuid, AccessFlags, Gid, Uid,
    },
    NixPath, Result,
};

fn main() {}

fn my_eaccess<P>(path: &P, amode: AccessFlags) -> Result<()>
where
    P: ?Sized + NixPath,
{
    let old_r_uid = getuid();
    let old_r_gid = getgid();
    let e_uid = geteuid();
    let e_gid = getegid();
    setresuid(e_uid, Uid::from(uid_t::MAX), Uid::from(uid_t::MAX)).unwrap();
    setresgid(e_gid, Gid::from(gid_t::MAX), Gid::from(gid_t::MAX)).unwrap();
    let ret = access(path, amode);
    setresuid(old_r_uid, Uid::from(uid_t::MAX), Uid::from(uid_t::MAX)).unwrap();
    setresgid(old_r_gid, Gid::from(gid_t::MAX), Gid::from(gid_t::MAX)).unwrap();

    ret
}

#[cfg(test)]
mod test {
    use super::*;
    use std::fs::{remove_file, OpenOptions};
    use std::os::unix::fs::OpenOptionsExt;

    #[test]
    fn file_that_does_not_exist() {
        assert!(my_eaccess("file_that_does_not_exist", AccessFlags::F_OK).is_err());
        assert!(my_eaccess(
            "file_that_does_not_exist",
            AccessFlags::F_OK | AccessFlags::R_OK
        )
        .is_err());
        assert!(my_eaccess(
            "file_that_does_not_exist",
            AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK
        )
        .is_err());
        assert!(my_eaccess(
            "file_that_does_not_exist",
            AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK | AccessFlags::X_OK
        )
        .is_err());
    }

    #[test]
    fn with_no_permission() {
        let _ = remove_file("file1");
        assert!(OpenOptions::new()
            .write(true)
            .create_new(true)
            .mode(0o0)
            .open("file1")
            .is_ok());
        assert!(my_eaccess("file1", AccessFlags::F_OK).is_ok());
        // if privileged, `rw` should be granted
        if geteuid().is_root() {
            assert!(my_eaccess("file1", AccessFlags::F_OK | AccessFlags::R_OK).is_ok());
            assert!(my_eaccess(
                "file1",
                AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK
            )
            .is_ok());
        } else {
            assert!(my_eaccess("file1", AccessFlags::F_OK | AccessFlags::R_OK).is_err());
            assert!(my_eaccess(
                "file1",
                AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK
            )
            .is_err());
        }
        // `x` is not granted no matter whether this process is privileged
        assert!(my_eaccess(
            "file1",
            AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK | AccessFlags::X_OK
        )
        .is_err());
        assert!(remove_file("file1").is_ok());
    }

    #[test]
    fn with_read_permission() {
        let _ = remove_file("file2");
        assert!(OpenOptions::new()
            .write(true)
            .create_new(true)
            .mode(0o400)
            .open("file2")
            .is_ok());
        assert!(my_eaccess("file2", AccessFlags::F_OK).is_ok());
        assert!(my_eaccess("file2", AccessFlags::R_OK).is_ok());
        assert!(my_eaccess("file2", AccessFlags::X_OK).is_err());

        if geteuid().is_root() {
            assert!(my_eaccess("file2", AccessFlags::W_OK).is_ok());
        } else {
            assert!(my_eaccess("file2", AccessFlags::W_OK).is_err());
        }

        assert!(my_eaccess(
            "file2",
            AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK | AccessFlags::X_OK
        )
        .is_err());
        assert!(remove_file("file2").is_ok());
    }

    #[test]
    fn with_read_write_permission() {
        let _ = remove_file("file3");
        assert!(OpenOptions::new()
            .write(true)
            .create_new(true)
            .mode(0o600)
            .open("file3")
            .is_ok());

        assert!(my_eaccess("file3", AccessFlags::F_OK).is_ok());
        assert!(my_eaccess("file3", AccessFlags::R_OK).is_ok());
        assert!(my_eaccess("file3", AccessFlags::W_OK).is_ok());
        assert!(my_eaccess("file3", AccessFlags::R_OK | AccessFlags::W_OK).is_ok());
        assert!(my_eaccess("file3", AccessFlags::R_OK | AccessFlags::X_OK).is_err());
        assert!(my_eaccess(
            "file3",
            AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK | AccessFlags::X_OK
        )
        .is_err());
        assert!(my_eaccess(
            "file3",
            AccessFlags::F_OK | AccessFlags::R_OK | AccessFlags::W_OK
        )
        .is_ok());
        assert!(remove_file("file3").is_ok());
    }

    #[test]
    fn with_read_write_execute_permission() {
        let _ = remove_file("file4");
        // with default umask `0222`, this new file should have permission `0755`
        assert!(OpenOptions::new()
            .write(true)
            .create_new(true)
            .mode(0o777)
            .open("file4")
            .is_ok());
        assert!(my_eaccess("file4", AccessFlags::F_OK).is_ok());

        assert!(my_eaccess("file4", AccessFlags::R_OK).is_ok());
        assert!(my_eaccess("file4", AccessFlags::R_OK | AccessFlags::W_OK).is_ok());
        assert!(my_eaccess(
            "file4",
            AccessFlags::R_OK | AccessFlags::W_OK | AccessFlags::X_OK
        )
        .is_ok());

        assert!(remove_file("file4").is_ok());
    }
}
