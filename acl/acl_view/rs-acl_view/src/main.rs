use clap::{ArgAction, Parser};
use nix::unistd::{Gid, Uid};
use posix_acl::{ACLEntry, PosixACL, Qualifier};
use std::path::PathBuf;
use ugid_functions_rs::{group_name_from_id, user_name_from_id};
use unix_permissions_ext::raw_fn::*;

#[derive(Parser)]
struct Opt {
    file: PathBuf,
    #[arg(short='d', action = ArgAction::SetTrue)]
    default_acl: bool,
}

fn display(entry: &ACLEntry) {
    match entry.qual {
        Qualifier::UserObj => print!("user_obj            "),
        Qualifier::User(uid) => print!(
            "user      {:10}",
            user_name_from_id(Uid::from(uid)).unwrap()
        ),
        Qualifier::GroupObj => print!("group_obj           "),
        Qualifier::Group(gid) => print!(
            "group     {:10}",
            group_name_from_id(Gid::from(gid)).unwrap()
        ),
        Qualifier::Mask => print!("mask                "),
        Qualifier::Other => print!("other               "),
        _ => unreachable!(),
    }

    let permission = stringify(entry.perm);
    // permission is guaranteed to have length 9, we only need its last 3
    // characters.
    println!("{}", &permission[6..]);
}

fn main() {
    let opt = Opt::parse();

    if opt.default_acl {
        let acl = PosixACL::read_default_acl(opt.file.as_path()).unwrap();
        acl.entries().iter().for_each(|entry| display(entry));
    } else {
        let acl = PosixACL::read_acl(opt.file.as_path()).unwrap();
        acl.entries().iter().for_each(|entry| display(entry));
    }
}
