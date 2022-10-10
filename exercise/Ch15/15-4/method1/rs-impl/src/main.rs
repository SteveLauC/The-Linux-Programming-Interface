use nix::{
    libc::uid_t,
    unistd::{access, geteuid, getuid, setresuid, AccessFlags, Uid},
    NixPath, Result,
};

fn main() {
    my_eaccess("file", AccessFlags::R_OK).unwrap();
}

fn my_eaccess<P>(path: &P, amode: AccessFlags) -> Result<()>
where
    P: ?Sized + NixPath,
{
    let old_r_uid = getuid();
    let e_uid = geteuid();
    setresuid(e_uid, Uid::from(uid_t::MAX), Uid::from(uid_t::MAX)).unwrap();
    let ret = access(path, amode);
    setresuid(old_r_uid, Uid::from(uid_t::MAX), Uid::from(uid_t::MAX)).unwrap();

    ret
}
