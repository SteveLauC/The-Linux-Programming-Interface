use std::{
    env::args,
    ffi::OsString,
    fs::{remove_file, File},
    io::Write,
    os::unix::ffi::OsStringExt,
    process::{exit, Command},
};

const BUF_SIZE: usize = 1024;

fn main() {
    let mut av = args().skip(1);
    let temp_file = av.next();
    let num_blks = av.next();
    let buf = [0; BUF_SIZE];

    let temp_file = match temp_file {
        Some(file_name) if file_name != "--help" => file_name,
        _ => {
            eprintln!("t_unlink temp-file [num-1kb-blocks]");
            exit(1);
        }
    };

    let num_blks = if num_blks.is_none() {
        1000000
    } else {
        num_blks.unwrap().parse().unwrap()
    };

    let mut file = File::create(temp_file.as_str()).expect("can not create file");
    remove_file(temp_file.as_str()).expect("can not delete file");

    for _ in 0..num_blks {
        assert!(file.write(&buf).unwrap().eq(&BUF_SIZE));
    }

    let mut out = Command::new("dirname")
        .arg(temp_file)
        .output()
        .unwrap()
        .stdout;
    out.truncate(out.len() - 1);
    let dir = OsString::from_vec(out);

    Command::new("df")
        .arg("-k")
        .arg(dir.as_os_str())
        .spawn()
        .unwrap()
        .wait()
        .unwrap();

    drop(file);
    println!("********** Closed file descriptor");

    Command::new("df")
        .arg("-k")
        .arg(dir.as_os_str())
        .spawn()
        .unwrap()
        .wait()
        .unwrap();
}
