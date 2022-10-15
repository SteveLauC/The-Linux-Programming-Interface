use std::{ffi::OsString, path::PathBuf, process::exit};

use clap::{ArgAction, Parser};
use nix::sys::xattr::{setxattr, SetxattrFlag};

#[derive(Parser)]
struct Opt {
    #[arg(short = 'n', action = ArgAction::Set)]
    name: OsString,
    #[arg(short = 'v', action = ArgAction::Set)]
    value: OsString,
    #[arg(action = ArgAction::Set)]
    file: PathBuf,
}

fn main() {
    let opt = Opt::parse();

    match setxattr(
        opt.file.as_path(),
        opt.name,
        opt.value,
        SetxattrFlag::empty(),
    ) {
        Ok(_) => {}
        Err(errno) => {
            eprintln!("Failed: {:?}", errno);
            exit(1);
        }
    }
}
