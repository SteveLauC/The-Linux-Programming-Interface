use unix_permissions_ext::UNIXPermissionsExt;
use std::{
    fs::{metadata, set_permissions},
    os::unix::fs::PermissionsExt,
    io::Result,
    env::args,
};
use libc::{S_IRUSR, S_IRGRP, S_IROTH, S_IXUSR, S_IXGRP, S_IXOTH};

fn main() {
    args().for_each(|p| {
        if chmod_a_r_x(p.as_str()).is_err() {
            eprintln!("failed on {}", p.as_str());
        }
    })
}

fn chmod_a_r_x(file: &str) -> Result<()> {
    let metadata = metadata(file)?;
    let file_type = metadata.file_type();
    let mut permission = metadata.permissions();

    let mut mode = permission.mode();

    mode |= S_IRUSR;
    mode |= S_IRGRP;
    mode |= S_IROTH;

    if file_type.is_dir() {
        mode |= S_IXUSR;
        mode |= S_IXGRP;
        mode |= S_IXOTH;
    } else if (permission.executable_by_owner()) || (permission.executable_by_group()) || (permission.executable_by_other()) {
        mode |= S_IXUSR;
        mode |= S_IXGRP;
        mode |= S_IXOTH;
    }

    permission.set_mode(mode);

    set_permissions(file, permission)
}
