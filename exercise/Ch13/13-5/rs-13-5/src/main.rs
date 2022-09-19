mod utility;

use std::{collections::VecDeque, fs::OpenOptions, io::Read, str};
use utility::{line_count, trim_buf, Cfg};

const BUF_SIZE: usize = 8192 * 5;

fn main() {
    let cfg = Cfg::new();
    let mut buf = [0; BUF_SIZE];
    let mut queue = VecDeque::new();

    let mut file = OpenOptions::new()
        .read(true)
        .open(cfg.filename)
        .expect("can not open target file");

    while let Ok(mut num_read) = file.read(&mut buf) {
        if num_read == 0 {
            break;
        }

        let line_count = line_count(&buf);
        if line_count > 1 {
            trim_buf(&buf[..num_read], &mut file, &mut num_read);
        }

        let range_start_idx = if line_count > cfg.n {
            line_count - cfg.n
        } else {
            1
        };

        for (idx, data) in buf[..num_read - 1].split(|item| *item == b'\n').enumerate() {
            if idx >= range_start_idx {
                queue.push_back(
                    str::from_utf8(data)
                        .expect("not a valid UTF-8 string")
                        .to_owned(),
                );
            }
        }

        while queue.len() > cfg.n {
            queue.pop_front();
        }
    }

    // print lines in the queue
    for item in queue.iter() {
        println!("{}", item);
    }
}
