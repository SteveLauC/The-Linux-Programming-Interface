use libc::EXIT_FAILURE;
use std::env::args;
use std::fs::{File, OpenOptions};
use std::io::{Read, Write};
use std::os::unix::fs::OpenOptionsExt;
use std::process::exit;

const BUF_SIZE: usize = 1024;

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() != 3 {
        eprintln!("usage: {} old-file new-file", av[0]);
    }

    let mut input_file: File =
        File::open(av[1].as_str()).expect(format!("opening file: {}", av[1]).as_str());
    let mut output_file: File = OpenOptions::new()
        .write(true)
        .create(true)
        .truncate(true)
        .mode(0o666)
        .open(av[2].as_str())
        .expect(format!("opening file: {}", av[2]).as_str());

    let mut buf: [u8; BUF_SIZE] = [0; BUF_SIZE];
    while let Ok(num_read) = input_file.read(&mut buf) {
        if num_read > 0 {
            let wrrite_res: Result<usize, _> = output_file.write(&buf[..num_read]);
            if wrrite_res.is_err() || wrrite_res.unwrap() != num_read {
                eprintln!("could't write whole buffer");
                exit(EXIT_FAILURE);
            }
        }

        if num_read == 0 {
            break;
        }
    }
}
