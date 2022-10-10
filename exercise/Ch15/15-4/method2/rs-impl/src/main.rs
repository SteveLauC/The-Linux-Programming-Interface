use nix::{
    errno::Errno,
    sys::stat::stat,
    unistd::{getegid, geteuid, getgroups, AccessFlags, Gid, Uid},
    NixPath, Result,
};
use unix_permissions_ext::raw_fn::*;

fn main() {}

pub fn my_eaccess<P: ?Sized + NixPath>(path: &P, amode: AccessFlags) -> Result<()> {
    let stat_buf = stat(path)?;
    let e_uid = geteuid();
    let e_gid = getegid();
    let supp_groups = getgroups()?;

    if e_uid.is_root() {
        if amode.contains(AccessFlags::X_OK) && !executalbe(stat_buf.st_mode) {
            return Err(Errno::last());
        }

        Ok(())
    } else if e_uid == Uid::from(stat_buf.st_uid) {
        if amode.contains(AccessFlags::R_OK) && !readable_by_owner(stat_buf.st_mode) {
            dbg!("r u ");
            return Err(Errno::last());
        }

        if amode.contains(AccessFlags::W_OK) && !writable_by_owner(stat_buf.st_mode) {
            dbg!("w u ");
            return Err(Errno::last());
        }

        if amode.contains(AccessFlags::X_OK) && !executable_by_owner(stat_buf.st_mode) {
            dbg!("x u ");
            return Err(Errno::last());
        }

        Ok(())
    } else if matches_group(Gid::from(stat_buf.st_gid), e_gid, &supp_groups) {
        if amode.contains(AccessFlags::R_OK) && !readable_by_group(stat_buf.st_mode) {
            return Err(Errno::last());
        }

        if amode.contains(AccessFlags::W_OK) && !writable_by_group(stat_buf.st_mode) {
            return Err(Errno::last());
        }

        if amode.contains(AccessFlags::X_OK) && !executable_by_group(stat_buf.st_mode) {
            return Err(Errno::last());
        }

        Ok(())
    } else {
        if amode.contains(AccessFlags::R_OK) && !readable_by_other(stat_buf.st_mode) {
            return Err(Errno::last());
        }

        if amode.contains(AccessFlags::W_OK) && !writable_by_other(stat_buf.st_mode) {
            return Err(Errno::last());
        }

        if amode.contains(AccessFlags::X_OK) && !executable_by_other(stat_buf.st_mode) {
            return Err(Errno::last());
        }

        Ok(())
    }
}

#[inline]
fn executalbe(mode: u32) -> bool {
    executable_by_other(mode) || executable_by_group(mode) || executable_by_owner(mode)
}

fn matches_group(group: Gid, e_gid: Gid, supp_groups: &[Gid]) -> bool {
    if e_gid == group {
        return true;
    }
    if supp_groups.contains(&e_gid) {
        return true;
    }
    false
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
