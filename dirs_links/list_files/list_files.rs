use std::{env::args, path::Path, process::exit};

fn list_files<P: AsRef<Path>>(path: P) {
    let is_current = path.as_ref() == Path::new(".");

    for entry in path.as_ref().read_dir().expect("opendir") {
        let entry = entry.expect("readdir");
        let file_name = entry.file_name();

        if !is_current {
            print!("{}", path.as_ref().display());
        }
        println!("{:?}", file_name);
    }
}

fn main() {
    let av = args().skip(1).collect::<Vec<String>>();
    if av.len() > 1 && av[0] == "--help" {
        eprintln!("list_files [dir...]");
        exit(1);
    }
    if av.len() == 0 {
        list_files(".");
    } else {
        for item in av {
            list_files(item);
        }
    }
}
