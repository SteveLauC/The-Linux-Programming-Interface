use libc;
use nix::{errno::Errno, Result};
use std::os::raw::c_void;

#[cfg(any(target_os = "linux", target_os = "netbsd", target_os = "freebsd"))]
pub fn sbrk(increment: libc::intptr_t) -> Result<*mut c_void> {
    let res: *mut c_void = unsafe { libc::sbrk(increment) };

    Errno::result(res)
}
