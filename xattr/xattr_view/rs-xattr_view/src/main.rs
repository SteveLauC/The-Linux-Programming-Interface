use std::{os::unix::ffi::OsStrExt, path::PathBuf};

use clap::{ArgAction, Parser};
use nix::sys::xattr::{getxattr, listxattr};

#[derive(Parser)]
struct Cmd {
    #[arg(short = 'x', action = ArgAction::SetTrue)]
    hex_mode: bool,
    #[arg(action = ArgAction::Set)]
    target_file: PathBuf,
}

fn main() {
    let cmd = Cmd::parse();

    println!("{}:", cmd.target_file.display());

    for name in listxattr(cmd.target_file.as_path()).unwrap() {
        let value =
            getxattr(cmd.target_file.as_path(), name.as_os_str()).unwrap();
        print!("    name: {:?}", name);
        if cmd.hex_mode {
            print!("value=");
            value.as_bytes().iter().for_each(|byte| {
                print!("{:02x} ", byte);
            });
            println!();
        } else {
            println!("value={:?}", value);
        }
    }
}
