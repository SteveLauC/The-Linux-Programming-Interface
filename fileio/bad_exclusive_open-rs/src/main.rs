use std::env::args;
use std::fs::OpenOptions;
use std::io::ErrorKind;
use std::os::unix::fs::OpenOptionsExt;
use std::process::id;
use std::thread::sleep;
use std::time::Duration;

fn main() {
    let av: Vec<String> = args().collect();
    assert!(av.len() > 1);
    let open_1 = OpenOptions::new().write(true).open(av[1].as_str());
    if open_1.is_ok() {
        println!("[PID {}] File {} already exists\n", id(), av[1]);
    } else {
        if open_1.unwrap_err().kind() != ErrorKind::NotFound {
            eprintln!("open");
        } else {
            println!("[PID {}] File {} doesn't exist yet", id(), av[1]);
            if av.len() > 2 {
                sleep(Duration::from_secs(5));
                println!("[PID {}] done sleeping", id());
            }
            if OpenOptions::new()
                .write(true)
                .create(true)
                .mode(0o600)
                .open(av[1].as_str())
                .is_err()
            {
                eprintln!("open");
            }

            println!("[PID {}] Created file {} exclusively", id(), av[1]);
        }
    }
}
