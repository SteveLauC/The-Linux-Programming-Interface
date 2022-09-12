use libc::pid_t;
use std::{
    fs::{read_dir, File, OpenOptions},
    io::{BufRead, BufReader},
    path::PathBuf,
};

#[derive(Default)]
struct Process {
    cmd: String,
    pid: pid_t,
    ppid: pid_t,
    parent_idx: Option<usize>, // default value of Option<T> is None
}

impl Process {
    /// construct a Process instance from `/proc/PID/status` file specified in `path`
    fn from_file(path: PathBuf) -> Option<Self> {
        let mut ret: Process = Process::default();

        if let Ok(file) = OpenOptions::new().read(true).open(path) {
            let buf_file: BufReader<File> = BufReader::new(file);
            for opt_line in buf_file.lines() {
                let line: String = opt_line.unwrap();

                if line.starts_with("Name") {
                    ret.cmd = line
                        .split('\t')
                        .nth(1)
                        .expect("Linux Kernel Bug: Wrong format of /proc/PID/status")
                        .to_owned();
                }
                if line.starts_with("Pid") {
                    ret.pid = line
                        .split('\t')
                        .nth(1)
                        .expect("Linux Kernel Bug: Wrong format of /proc/PID/status")
                        .parse::<pid_t>()
                        .expect("Linux Kernel Bug: invalid pid");
                }
                if line.starts_with("PPid") {
                    ret.ppid = line
                        .split('\t')
                        .nth(1)
                        .expect("Linux Kernel Bug: Wrong format of /proc/PID/status")
                        .parse::<pid_t>()
                        .expect("Linux Kernel Bug: invalid PPid");
                    break;
                }
            }
        }
        if ret.pid == 0 {
            None
        } else {
            Some(ret)
        }
    }
}

#[derive(Default)]
struct Processes {
    array: Vec<Process>,
}

impl Processes {
    /// construct a new Processes by scanning `/proc`
    fn new() -> Self {
        let mut ret: Processes = Processes::default();
        for opt_entry in read_dir("/proc").expect("can not open /proc") {
            if let Ok(entry) = opt_entry {
                let mut path: PathBuf = entry.path();
                path.push("status");
                if let Some(p) = Process::from_file(path) {
                    ret.array.push(p);
                }
            }
        }
        ret.array.sort_unstable_by_key(|proc:&Process| proc.pid);
        ret
    }

    /// set the `parent_idx` field
    fn set_parent_idx(&mut self) {
        let len: usize = self.array.len();
        for i in 0..len {
            for j in 0..len {
                if self.array[j].pid == self.array[i].ppid {
                    self.array[i].parent_idx = Some(j);
                }
            }
        }
    }

    /// traverse this process tree in pro-order
    fn preorder_traverse(&self, idx: usize, level: usize) {
        for _ in 0..level {
            print!("\t");
        }
        println!("{} {}", self.array[idx].pid, self.array[idx].cmd);

        for i in 0..self.array.len() {
            if self.array[i].parent_idx == Some(idx){
                self.preorder_traverse(i, level + 1);
            }
        }
    }
}

fn main() {
    let mut p = Processes::new();
    p.set_parent_idx();
    p.preorder_traverse(0, 0);
}