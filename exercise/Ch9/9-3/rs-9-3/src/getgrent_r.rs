//! Expose `getgrent_r` in a rusty way (Linux-specific)

use nix::{
    errno::Errno,
    libc::{c_char, c_int, endgrent, getgrent_r, group, setgrent, ENOENT, ERANGE},
    unistd::Group,
};
use std::{mem::zeroed, ptr::null_mut};

pub fn get_all_groups() -> Result<Vec<Group>, Errno> {
    let mut v: Vec<Group> = Vec::new();

    let mut gr_buf: group = unsafe { zeroed() };
    let mut gr_str_buf: [c_char; 4096] = [0; 4096];
    unsafe { setgrent() };

    loop {
        let mut result: *mut group = null_mut();
        let res: c_int = unsafe {
            getgrent_r(
                &mut gr_buf as *mut group,
                &mut gr_str_buf as *mut c_char,
                4096,
                &mut result as *mut *mut group,
            )
        };

        // no more entries
        if res == ENOENT {
            unsafe { endgrent() };
            break;
        }

        // error
        if res == ERANGE {
            return Err(Errno::from_i32(ERANGE));
        }

        // man page for `getgrent_r` only lists the two above error cases.
        // In other cases, it should be successful.
        assert_eq!(res, 0);
        v.push(Group::from(&gr_buf));
    }

    Ok(v)
}
