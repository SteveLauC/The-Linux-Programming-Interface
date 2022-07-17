use std::env::args;
use std::fs::{File, OpenOptions};
use std::io::{stdin, stdout, Read, Result, Write};
use std::process::exit;

fn main() {
    let mut av: Vec<String> = args().collect();
    if av.len() < 2 || av.len() > 3 {
        eprintln!("usage: tee [-a] file");
        exit(1);
    }
    av.swap_remove(0);

    let append: bool = av.iter().find(|item: &&String| *item == "-a").is_some();
    let file_path: &str = av.iter().find(|item: &&String| *item != "-a").unwrap();

    let mut file: File = OpenOptions::new()
        .create(true)
        .write(true)
        .append(append)
        .truncate(!append)
        .open(file_path)
        .expect("can not open or create file");

    let mut buf: [u8; 1024] = [0; 1024];
    while let Ok(num_read) = stdin().read(&mut buf) {
        if num_read == 0 {
            break;
        }

        let stdout_write_res: Result<usize> = stdout().write(&buf[..num_read]);
        if stdout_write_res.is_err() || stdout_write_res.unwrap() != num_read {
            eprintln!("can not write the whole buffer to the stdout");
        }
        let file_write_res: Result<usize> = file.write(&buf[..num_read]);
        if file_write_res.is_err() || file_write_res.unwrap() != num_read {
            eprintln!("can not write the whole buffer to the file");
        }
    }
}
