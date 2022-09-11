use std::{
    env::args,
    ffi::OsString,
    fs::{read_dir, DirEntry, File, OpenOptions},
    io::{BufRead, BufReader},
    path::PathBuf,
    process::exit,
};
use ugid_functions_rs::user_id_from_name;

fn main() {
    if let Some(uid_str) = args().nth(1) {
        let uid: u32 = user_id_from_name(&uid_str)
            .expect("user_id_from_name")
            .as_raw();

        for entry in read_dir("/proc").expect("read_dir") {
            let entry: DirEntry = entry.unwrap();
            let entry_name: OsString = entry.file_name();

            if entry_name.to_str().unwrap().chars().nth(0).unwrap().is_digit(10) {
                let mut path: PathBuf = entry.path();
                path.push("status");

                parse_status_file(path, uid);
            }
        }
    } else {
        eprintln!("usage: 12-1 <USER>");
        exit(1);
    };
}

fn parse_status_file(path: PathBuf, id: u32) {
    let file: BufReader<File> =
        BufReader::new(OpenOptions::new().read(true).open(path).expect("open"));

    let mut name_pid_buf: String = String::new();
    for opt_line in file.lines() {
        if let Ok(line) = opt_line {
            if line.starts_with("Name") {
                name_pid_buf.push_str(line.split('\t').nth(1).unwrap());
            }
            if line.starts_with("Pid") {
                name_pid_buf.push(' ');
                name_pid_buf.push_str(line.split('\t').nth(1).unwrap());
            }
            if line.starts_with("Uid") {
                if id == line.split('\t').nth(1).unwrap().parse::<u32>().unwrap() {
                    println!("{}", name_pid_buf);
                }
                break;
            }
        }
    }
}
