#![allow(non_snake_case)]
// use crate::ename;
use libc::{_exit, abort, exit, EXIT_FAILURE};
use std::env::var;

/// purpose: terminate the program
///
/// action: if `EF_DUMPCORE` is validly set, call `abort()`. Otherwise, call `exit`
///         or `_exit()` depending on `useExit3`
///
/// arguments:
///  * `useExit3`: boolean to represent if we use `exit` over `_exit`
///
/// note: static function
fn terminate(useExit3: bool) {
    if let Ok(var) = var("EF_DUMPCORE") {
        if !var.is_empty() {
            unsafe {
                abort();
            }
        }
    } else {
        unsafe {
            if useExit3 {
                exit(EXIT_FAILURE);
            } else {
                _exit(EXIT_FAILURE);
            }
        }
    }
}
