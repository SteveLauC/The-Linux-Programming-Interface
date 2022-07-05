#![allow(non_snake_case)]

use bitflags::bitflags;
use libc::EXIT_FAILURE;
use parse_int;
use std::process::exit;

bitflags! {
    pub struct GnFlag: i32 {
        const  GN_NONNEG = 0o1;  /* value must be >= 0 */
        const GN_GT_0 = 0o2;  /* value must be > 0  */

        /* By default, integers are decimal */
        const GN_ANY_BASE = 0o100;  /* auto-detect base from prefix, only support `0x` `0o`*/
        const GN_BASE_8 = 0o200;   /* value is expressed in octal       */
        const GN_BASE_16 = 0o400;   /* value is expressed in hexadecimal */
    }
}

/// purpose: error handling for `get_num.c`
///
/// action: print `fname`, `name`, `msg` and `arg` infomation to stderr, the exit.
///
/// argument:
///  * `fname`: function name, `getLong` or `getInt`
///  * `msg`: description for the error case
///  * `arg`: string containing a number
///  * `name`: if `name` is not None, it should contain a string identifying the argument in `arg`
///
/// note: only available in `get_num.c`
fn gnFail(fname: &str, msg: &str, arg: Option<&str>, name: Option<&str>) {
    eprint!("{} error", fname);
    if let Some(n) = name {
        eprintln!("(in {})", n);
    }

    eprintln!(": {}", msg);

    if let Some(a) = arg {
        if !a.is_empty() {
            eprint!("     offending text: {}", a);
        }
    }

    exit(EXIT_FAILURE);
}

/// purpose: underlying number parsing function
///
/// action: call `strtol`, and do some error handling after it
///
/// arguments:
///  * `fname`: function name, `getLong` or `getInt`
///  * `arg`: string containing a number
///  * `flags`: provides some control over the operation
///  * `name`: if `name` is not None, it should contain a string identifying the argument in `arg`
///
///  return: a long integer
///
///  note: only available in `get_num.c`
fn getNum(fname: &str, arg: Option<&str>, flags: GnFlag, name: Option<&str>) -> i64 {
    if arg.is_none() || arg.unwrap().is_empty() {
        gnFail(fname, "null or empty string", arg, name);
    }

    let base: i32 = if flags.contains(GnFlag::GN_ANY_BASE) {
        0
    } else if flags.contains(GnFlag::GN_BASE_8) {
        8
    } else if flags.contains(GnFlag::GN_BASE_16) {
        16
    } else {
        10
    };

    let arg: &str = arg.unwrap();

    let result: Result<i64, _> = match base {
        0 => parse_int::parse::<i64>(arg),
        8 => i64::from_str_radix(arg, 8),
        16 => i64::from_str_radix(arg, 16),
        _ => arg.parse(),
    };

    if let Err(ref e) = result {
        gnFail(
            fname,
            format!("parse failed: {:?}", e).as_str(),
            Some(arg),
            name,
        );
    }

    let res: i64 = result.unwrap();
    if flags.contains(GnFlag::GN_NONNEG) && res < 0 {
        gnFail(fname, "negative value not allowed", Some(arg), name);
    }

    if flags.contains(GnFlag::GN_GT_0) && res <= 0 {
        gnFail(fname, "value must be >= 0", Some(arg), name);
    }

    res
}

/// purpose: parse a long integer from a string
///
/// action: call `getNum`
///
/// arguments:
///  * `arg`: string containing a long integer value
///  * `flags`: provides some control over the operation
///  * `name`: if `name` is not None, it should contain a string identifying
///      the argument in `arg`
///
///  return: a long integer parsed from `arg`
pub fn getLong(arg: Option<&str>, flags: GnFlag, name: Option<&str>) -> i64 {
    return getNum("getLong", arg, flags, name);
}

/// purpose: parse an integer from a string
///
/// action: call `getNum`
///
/// arguments:
///  * `arg`: string containing an integer value
///  * `flags`: provides some control over the operation
///  * `name`: if `name` is not None, it should contain a string identifying the argument in `arg`
///
///  return: an integer parsed from `arg`
fn getInt(arg: Option<&str>, flags: GnFlag, name: Option<&str>) -> i32 {
    let res_long: i64 = getNum("getInt", arg, flags, name);
    if i32::try_from(res_long).is_err() {
        gnFail("getInt", "integer out of range", arg, name);
    }

    return res_long as i32;
}
