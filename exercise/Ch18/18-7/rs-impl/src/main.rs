use std::fs::metadata;
use std::{env::args, fmt, fs::FileType, os::unix::fs::FileTypeExt, process::exit};
use walkdir::WalkDir;

#[derive(Default)]
struct Statistic {
    reg: usize,
    dir: usize,
    link: usize,
    fifo: usize,
    sock: usize,
    char_dev: usize,
    blk_dev: usize,
    total: usize,
}

impl Statistic {
    fn count(&mut self, file_type: FileType) {
        if file_type.is_file() {
            self.reg += 1;
        }
        if file_type.is_dir() {
            self.dir += 1;
        }
        if file_type.is_symlink() {
            self.link += 1;
        }
        if file_type.is_fifo() {
            self.fifo += 1;
        }
        if file_type.is_char_device() {
            self.char_dev += 1;
        }
        if file_type.is_block_device() {
            self.blk_dev += 1;
        }

        self.total = self.reg + self.dir + self.link + self.fifo + self.char_dev + self.blk_dev;
    }
}

impl fmt::Display for Statistic {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "directory: {} ({:.3})\n\
            regular file: {} ({:.3})\n\
            FIFO: {} ({:.3})\n\
            symlink: {} ({:.3})\n\
            socket: {} ({:.3})\n\
            char device: {} ({:.3})\n\
            block device: {} ({:.3})",
            self.dir,
            self.dir as f64 / self.total as f64,
            self.reg,
            self.reg as f64 / self.total as f64,
            self.fifo,
            self.fifo as f64 / self.total as f64,
            self.link,
            self.link as f64 / self.total as f64,
            self.sock,
            self.sock as f64 / self.total as f64,
            self.char_dev,
            self.char_dev as f64 / self.total as f64,
            self.blk_dev,
            self.blk_dev as f64 / self.total as f64
        )
    }
}

fn usage() {
    eprintln!("Usage: rs-impl directory");
    exit(1);
}

fn main() {
    let mut av = args();
    if av.len() != 2 {
        usage();
    }
    let mut statistic = Statistic::default();
    let target_dir = av.nth(1).unwrap();
    let dir_metadata = metadata(target_dir.as_str()).expect("stat");
    if !dir_metadata.is_dir() {
        eprintln!("{} is not a directory", target_dir);
        usage();
    }

    for res_entry in WalkDir::new(target_dir) {
        let entry = res_entry.unwrap();
        let entry_metadata = entry.metadata().expect("stat");

        statistic.count(entry_metadata.file_type());
    }

    println!("{}", statistic);
}
