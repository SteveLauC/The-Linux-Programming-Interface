use clap::Parser;
use libc::{gid_t, uid_t};
use posix_acl::{PosixACL, Qualifier};
use std::process::exit;
use std::{
    fs::metadata,
    os::linux::fs::MetadataExt,
    path::{Path, PathBuf},
};
use ugid_functions_rs::{group_id_from_name, user_id_from_name};
use unix_permissions_ext::raw_fn::stringify;

#[derive(Parser)]
struct Opt {
    class: char,
    id: String,
    file: PathBuf,
}

fn display_user(uid: uid_t, owner: uid_t, file: &Path) {
    let acl = PosixACL::read_acl(file).unwrap();
    let mask = acl.get(Qualifier::Mask);

    if uid == owner {
        println!("{}", &stringify(acl.get(Qualifier::UserObj).unwrap())[6..]);
        return;
    }

    if let Some(mask) = mask {
        if let Some(entry) = acl
            .entries()
            .iter()
            .find(|entry| entry.qual == Qualifier::User(uid))
        {
            println!("{}", &stringify(entry.perm & mask)[6..]);
        } else {
            eprintln!("Entry does not exist");
            exit(1);
        }
    // If entry `ACL_MASK` does not exist, then no `ACL_USER` entry would exist.
    } else {
        eprintln!("Entry does not exist");
        exit(1);
    }
}

fn display_group(gid: gid_t, group: gid_t, file: &Path) {
    let acl = PosixACL::read_acl(file).unwrap();
    let mask = acl.get(Qualifier::Mask);

    if gid == group {
        let acl_group_obj_perm = acl.get(Qualifier::GroupObj).unwrap();
        let perm = if let Some(mask) = mask {
            acl_group_obj_perm & mask
        } else {
            acl_group_obj_perm
        };
        println!("{}", &stringify(perm)[6..]);
        return;
    }

    if let Some(mask) = mask {
        if let Some(entry) = acl
            .entries()
            .iter()
            .find(|entry| entry.qual == Qualifier::Group(gid))
        {
            println!("{}", &stringify(entry.perm & mask)[6..]);
        } else {
            eprintln!("Entry does not exist");
            exit(1);
        }
    // If entry `ACL_MASK` does not exist, then no `ACL_GROUP` entry would exist.
    } else {
        eprintln!("Entry does not exist");
        exit(1);
    }
}

fn main() {
    let opt = Opt::parse();
    let metadata = metadata(opt.file.as_path()).unwrap();
    let owner = metadata.st_uid();
    let group = metadata.st_gid();

    match opt.class {
        'u' => {
            let uid = if let Ok(id) = opt.id.as_str().parse::<u32>() {
                id
            } else if let Some(uid) = user_id_from_name(opt.id.as_str()) {
                uid.as_raw()
            } else {
                eprintln!("user {} does not exist", opt.id);
                exit(1);
            };
            display_user(uid, owner, opt.file.as_path());
        }
        'g' => {
            let gid = if let Ok(id) = opt.id.as_str().parse::<u32>() {
                id
            } else if let Some(gid) = group_id_from_name(opt.id.as_str()) {
                gid.as_raw()
            } else {
                eprintln!("gourp {} does not exist", opt.id);
                exit(1);
            };
            display_group(gid, group, opt.file.as_path());
        }
        _ => {
            eprintln!("Usage: rs-impl <u/g> <uid/gid/username/groupname> <file>");
            exit(1);
        }
    }
}
