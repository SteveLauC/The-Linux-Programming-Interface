use nix::{
    libc::{setreuid, uid_t},
    unistd::{seteuid, setfsuid, setresuid, setuid, Uid},
};
use std::{
    env::args,
    fs::{File, OpenOptions},
    io::{BufRead, BufReader},
    process::exit,
};

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() != 2 {
        exit(1);
    }

    match av[1].as_str() {
        "1" => setuid(Uid::from_raw(2000)).unwrap(),
        "2" => unsafe {
            assert_eq!(setreuid(uid_t::MAX, 2000), 0);
        },
        "3" => seteuid(Uid::from_raw(2000)).map(drop).unwrap(),
        "4" => {
            setfsuid(Uid::from_raw(2000));
        }
        "5" => setresuid(
            Uid::from_raw(uid_t::MAX),
            Uid::from_raw(2000),
            Uid::from_raw(3000),
        )
        .unwrap(),
        _ => unreachable!(),
    };

    let status: BufReader<File> = BufReader::new(
        OpenOptions::new()
            .read(true)
            .open("/proc/self/status")
            .unwrap(),
    );

    for opt_line in status.lines() {
        if let Ok(line) = opt_line {
            if line.starts_with("Uid") {
                println!("{}", line);
                break;
            }
        }
    }
}
