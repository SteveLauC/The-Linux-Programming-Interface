use std::{
    env::args,
    fs::{read_dir, read_link, File, OpenOptions},
    io::{BufRead, BufReader},
    path::PathBuf,
    process::exit,
};

fn main() {
    if let Some(path) = args().nth(1).map(|path| PathBuf::from(path)) {
        for opt_entry in read_dir("/proc").expect("can not open /proc") {
            if let Ok(entry) = opt_entry {
                let mut fd_dir_path = entry.path(); // `/proc/PID/fd`
                fd_dir_path.push("fd");
                let mut status_path = entry.path(); // `/proc/PID/status`
                status_path.push("status");

                // ignore those disappeared entries
                if let Ok(dir) = read_dir(fd_dir_path) {
                    for opt_opened_files in dir {
                        if let Ok(opened_files) = opt_opened_files {
                            // this process satisfies the condition
                            if read_link(opened_files.path()).unwrap() == path {
                                if let Ok(status_file) =
                                    OpenOptions::new().read(true).open(status_path)
                                {
                                    let mut buf_status_file: BufReader<File> =
                                        BufReader::new(status_file);
                                    let mut buf: String = String::new();
                                    buf_status_file.read_line(&mut buf).unwrap();
                                    println!("{}", buf.trim());
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    } else {
        eprintln!("Usage: 12-3 <PATH>");
        exit(1);
    }
}
