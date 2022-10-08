use nix::{
    fcntl::{open, OFlag},
    sys::stat::{umask, Mode},
    unistd::mkdir,
};
use std::{
    fs::{metadata, remove_dir, remove_file},
    os::unix::fs::PermissionsExt,
};
use unix_mode::to_string;

const MY_FILE: &str = "myfile";
const MY_DIR: &str = "mydir";
const FILE_PERMS: Mode = Mode::from_bits_truncate(0o660);
const DIR_PERMS: Mode = Mode::from_bits_truncate(0o777);
const UMASK_SETTING: Mode = Mode::from_bits_truncate(0o033);

fn main() {
    umask(UMASK_SETTING);

    let mut o_flag = OFlag::empty();
    o_flag.insert(OFlag::O_CREAT);
    o_flag.insert(OFlag::O_RDWR);
    o_flag.insert(OFlag::O_EXCL);
    open(MY_FILE, o_flag, FILE_PERMS).expect("can not create file");
    mkdir(MY_DIR, DIR_PERMS).expect("can not create dir");

    let file_perm = metadata(MY_FILE)
        .expect("can not fetch metadata")
        .permissions()
        .mode();
    let dir_perm = metadata(MY_DIR)
        .expect("can not fetch metadata")
        .permissions()
        .mode();

    println!(
        "Requested file perms: {}",
        &to_string(FILE_PERMS.bits())[1..]
    );
    println!("Process umask: {}", &to_string(UMASK_SETTING.bits())[1..]);
    println!("Actual file perms: {}", &to_string(file_perm)[1..]);

    println!("Requested dir perms: {}", &to_string(DIR_PERMS.bits())[1..]);
    println!("Process umask: {}", &to_string(UMASK_SETTING.bits())[1..]);
    println!("Actual dir perms: {}", &to_string(dir_perm)[1..]);

    remove_file(MY_FILE).expect("can not delete file");
    remove_dir(MY_DIR).expect("can not delete dir");
}
