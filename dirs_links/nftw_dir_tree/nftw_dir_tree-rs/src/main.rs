use clap::{ArgAction, Parser};
use std::{
    os::{linux::fs::MetadataExt, unix::fs::FileTypeExt},
    path::PathBuf,
};
use walkdir::{DirEntry, WalkDir};

#[derive(Parser, Debug)]
struct Cli {
    #[arg(short = 'd', action = ArgAction::SetTrue)]
    ftw_depth: bool,
    #[arg(short = 'm', action = ArgAction::SetTrue)]
    ftw_mount: bool,
    #[arg(short = 'p', action = ArgAction::SetTrue)]
    ftw_phys: bool,
    file: PathBuf,
}

fn dir_tree(entry: &DirEntry) {
    if let Ok(metadata) = entry.metadata() {
        let file_type = metadata.file_type();
        if file_type.is_file() {
            print!("-");
        } else if file_type.is_dir() {
            print!("d");
        } else if file_type.is_char_device() {
            print!("c");
        } else if file_type.is_block_device() {
            print!("b");
        } else if file_type.is_symlink() {
            print!("l");
        } else if file_type.is_fifo() {
            print!("p");
        } else if file_type.is_socket() {
            print!("s");
        } else {
            print!("?");
        }

        print!(" {:7} ", metadata.st_ino());
    } else {
        print!("?");
    }

    print!("{} ", entry.depth());
    print!("{:?}\n", entry.file_name());
}

fn main() {
    let opt = Cli::parse();
    let walker = WalkDir::new(opt.file)
        .follow_links(opt.ftw_phys)
        .contents_first(opt.ftw_depth)
        .same_file_system(opt.ftw_mount);

    for res_entry in walker {
        let entry = res_entry.unwrap();
        dir_tree(&entry);
    }
}
