use clap::{Arg, ArgAction, Command};
use rand::{seq::SliceRandom, thread_rng};
use std::{
    fs::{remove_file, OpenOptions},
    io::stdout,
    io::Write,
    process,
    time::{Duration, SystemTime},
};

#[derive(Default, Debug)]
struct Config {
    ordered: bool,
    n: usize,
}

fn main() {
    print!("File system: ");
    stdout().flush().unwrap();
    let out = process::Command::new("bash")
        .arg("-c")
        .arg("stat . -f -c \"%T\"")
        .output()
        .unwrap();

    print!("{}", std::str::from_utf8(&out.stdout).unwrap());

    let cfg = arg_parsing();
    if cfg.ordered {
        println!(
            "Benchmarking (Creating and deleting {} file in an **ordered** way...)",
            cfg.n
        );
    } else {
        println!(
            "Benchmarking (Creating and deleting {} file in a **random** way...)",
            cfg.n
        );
    }

    println!("Consumed {:?}", file_manipulation(&cfg));
}

fn arg_parsing() -> Config {
    let mut config = <Config as Default>::default();

    let mth = Command::new("14-1")
        .arg(Arg::new("ordered").short('o').action(ArgAction::SetTrue))
        .arg(
            Arg::new("n")
                .short('n')
                .required(true)
                .action(ArgAction::Set),
        )
        .get_matches();

    config.ordered = mth.get_flag("ordered");
    config.n = mth
        .get_one::<String>("n")
        .unwrap()
        .parse()
        .expect("invalid argument for n");

    config
}

fn file_manipulation(cfg: &Config) -> Duration {
    let mut arr = (0..cfg.n).collect::<Vec<usize>>();
    if cfg.ordered {
        arr.shuffle(&mut thread_rng());
    }

    let now = SystemTime::now();

    for item in arr.iter() {
        let filename = item.to_string();
        if let Ok(mut file) = OpenOptions::new()
            .create(true)
            .write(true)
            .open(filename.as_str())
        {
            file.write(b"C").unwrap();
        }
    }

    for item in arr.iter() {
        let filename = item.to_string();
        let _ = remove_file(filename);
    }

    now.elapsed().unwrap()
}
