use libc::{endpwent, getpwent, passwd, strlen, strncmp};
use std::ffi::CStr;

fn main() {
    unsafe {
        let name: &CStr = CStr::from_ptr((*my_getpwnam("steve")).pw_name);
        println!("{:?}", name);
    }
}

unsafe fn my_getpwnam(name: &str) -> *mut passwd {
    let mut v: Vec<u8> = name.bytes().collect();
    v.push(0);
    let name_c_str: &CStr = CStr::from_bytes_with_nul(&v).unwrap();
    let mut p: *mut passwd = getpwent();
    while !p.is_null() {
        if strncmp(
            (*p).pw_name,
            name_c_str.as_ptr() as *mut i8,
            strlen(name_c_str.as_ptr() as *mut i8),
        ) == 0
        {
            endpwent();
            return p;
        }

        p = getpwent();
    }

    endpwent();
    p
}
