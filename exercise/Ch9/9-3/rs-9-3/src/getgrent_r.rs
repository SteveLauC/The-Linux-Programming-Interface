//! Expose `getgrent_r` in a rusty way (Linux-specific)

use nix::{
    errno::Errno,
    libc,
    unistd::{sysconf, Group, SysconfVar},
};
use std::{mem, ptr};

#[cfg(target_os = "linux", target_env = "gnu")]
pub struct AllGroups;

impl AllGroups {
    pub fn new() -> AllGroups {
        unsafe {
            libc::setgrent();
        }
        AllGroups
    }
}

impl Drop for AllGroups {
    fn drop(&mut self) {
        unsafe {
            libc::endgrent();
        }
    }
}

impl Iterator for AllGroups {
    type Item = Result<Group, Errno>;

    fn next(&mut self) -> Option<Self::Item> {
        let mut grp = mem::MaybeUninit::<libc::group>::uninit();
        let str_buf_size = match sysconf(SysconfVar::GETGR_R_SIZE_MAX) {
            Ok(Some(n)) => n as usize,
            Err(_) | Ok(None) => 4096,
        };
        let mut str_buf = Vec::with_capacity(str_buf_size);
        let mut res: *mut libc::group = ptr::null_mut();
        let ret = unsafe {
            libc::getgrent_r(
                grp.as_mut_ptr(),
                str_buf.as_mut_ptr(),
                str_buf_size,
                &mut res as *mut *mut libc::group,
            )
        };

        // encounters an error
        if ret != 0 && ret != libc::ENOENT && res.is_null() {
            return Some(Err(Errno::from_i32(ret)));
        }

        // Now, all we need to do is to distinguish the successful case and
        // the case where there are no more entries. Luckily, we can do this
        // through `res`.

        // no more entries
        if res.is_null() {
            None
        } else {
            // successful case
            let grp = unsafe { grp.assume_init() };
            Some(Ok(Group::from(&grp)))
        }
    }
}
