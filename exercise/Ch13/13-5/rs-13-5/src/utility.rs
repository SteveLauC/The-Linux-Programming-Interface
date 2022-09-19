use std::{
    env::args,
    fs::File,
    io::{Seek, SeekFrom},
    process::exit,
};

#[derive(Debug)]
pub(crate) struct Cfg {
    pub(crate) filename: String,
    pub(crate) n: usize,
}

impl Cfg {
    pub(crate) fn new() -> Self {
        let mut cfg = Cfg {
            filename: String::new(),
            n: 10,
        };
        let mut av = args();
        let opt_sec_arg = av.nth(1);
        let opt_third_arg = av.next();

        if opt_sec_arg.is_none() {
            eprintln!("Usage: 13-5 [-n] filename");
            exit(1);
        }

        if let Some(third_arg) = opt_third_arg {
            cfg.filename = third_arg;
            cfg.n = opt_sec_arg
                .unwrap()
                .trim_matches(|c| c == '-')
                .parse()
                .expect("not a valid number");
        } else {
            cfg.filename = opt_sec_arg.unwrap();
        }

        cfg
    }
}

#[inline]
pub(crate) fn line_count(buf: &[u8]) -> usize {
    buf.iter()
        .filter(|ref_ref_to_item| **ref_ref_to_item == b'\n')
        .count()
}

pub(crate) fn trim_buf(buf: &[u8], file: &mut File, num_read: &mut usize) {
    let idx = buf
        .iter()
        .rposition(|item| *item == b'\n')
        .expect("no linebreak found in buf");

    let seek_offset = (buf.len() - idx - 1) as i64 * -1;

    *num_read += seek_offset as usize;

    // seek the file backwards
    file.seek(SeekFrom::Current(seek_offset))
        .expect("can not seek the file");
}
