use std::{
    env::args,
    fs::{File, OpenOptions},
    io::{Read, Seek, SeekFrom, Write},
    process::Command,
};

fn main() {
    let new_val: Option<String> = args().nth(1);

    let mut file: File = OpenOptions::new()
        .read(true)
        .write(new_val.is_some())
        .open("/proc/sys/kernel/pid_max")
        .expect("open");

    let mut buf: String = String::new();
    file.read_to_string(&mut buf).expect("read");
    if new_val.is_some() {
        print!("Old value: ");
    }
    print!("{}", buf);

    if new_val.is_some() {
        file.seek(SeekFrom::Start(0)).expect("seek");

        file.write(buf.as_bytes()).expect("write");

        Command::new("sh").args([
            "-c",
            "echo /proc/sys/kernel/pid_max now contains;\
            cat /proc/sys/kernel/pid_max",
        ]);
    }
}
