use nix::{
    libc::AT_FDCWD,
    unistd::{fchownat, FchownatFlags},
};
use ugid_functions_rs::{group_id_from_name, user_id_from_name};

use std::{env::args, process::exit};

fn main() {
    let av = args().collect::<Vec<String>>();
    if av.len() < 3 {
        eprintln!("Usage: {} owner group [files...]", av[0].as_str());
        eprintln!("owner and group can be '-' to leave it unchanged");
        exit(1);
    }

    let uid = (av[1] != "-").then(|| {
        if let Some(uid) = user_id_from_name(av[1].as_str()) {
            uid
        } else {
            eprintln!("No such user ({})", av[1].as_str());
            exit(1);
        }
    });

    let gid = (av[2] != "-").then(|| {
        if let Some(gid) = group_id_from_name(av[2].as_str()) {
            gid
        } else {
            eprintln!("No such group ({})", av[2].as_str());
            exit(1);
        }
    });

    let mut err_fnd = false;

    for file in &av[3..] {
        if fchownat(
            Some(AT_FDCWD),
            file.as_str(),
            uid,
            gid,
            FchownatFlags::FollowSymlink,
        )
            .is_err()
        {
            eprintln!("chown: {}", file.as_str());
            err_fnd = true;
        }
    }

    exit(err_fnd as i32);
}
