use std::{
    fs::{metadata, symlink_metadata, Metadata},
    os::unix::fs::{FileTypeExt, MetadataExt},
    path::PathBuf,
    time::{Duration, UNIX_EPOCH},
};

use chrono::{DateTime, Local};
use clap::Parser;
use libc::{major, minor};
use unix_permissions_ext::UNIXPermissionsExt;

#[derive(Debug, Parser)]
struct Config {
    #[arg(short, action = clap::ArgAction::SetTrue)]
    link: bool,
    file: PathBuf,
}

fn show_time(seconds: i64) -> String {
    let date: DateTime<Local> =
        DateTime::from(UNIX_EPOCH + Duration::from_secs(u64::try_from(seconds).unwrap()));
    date.format("%b %e %H:%M:%S %Y").to_string()
}

fn display_stat_info(metadata: &Metadata) {
    print!("File type: ");
    let file_type = metadata.file_type();
    if file_type.is_file() {
        println!("regular file");
    } else if file_type.is_dir() {
        println!("directory");
    } else if file_type.is_char_device() {
        println!("character device");
    } else if file_type.is_block_device() {
        println!("block device");
    } else if file_type.is_symlink() {
        println!("symbolic (soft) link");
    } else if file_type.is_fifo() {
        println!("FIFO or pipe");
    } else if file_type.is_socket() {
        println!("socket");
    } else {
        println!("unknown file type?");
    }

    unsafe {
        println!(
            "Device containing i-node: major={} minor={}",
            major(metadata.dev()),
            minor(metadata.dev())
        )
    }
    println!("I-node number: {}", metadata.ino());
    let permission = metadata.permissions();
    println!("Mode: {:o} ({})", metadata.mode(), permission.stringify());
    println!(
        "Special bits set: {}{}{}",
        if permission.set_uid() { "set-UID" } else { "" },
        if permission.set_gid() { "set-GID" } else { "" },
        if permission.sticky_bit() {
            "sticky bit"
        } else {
            ""
        },
    );
    println!("Number of (hard) links: {}", metadata.nlink());
    println!("Ownership: UID={} GID={}", metadata.uid(), metadata.gid());
    if file_type.is_char_device() || file_type.is_block_device() {
        unsafe {
            println!(
                "Device number (st_rdev): major={} minor={}",
                major(metadata.rdev()),
                minor(metadata.rdev())
            )
        }
    }

    println!("File size: {} bytes", metadata.size());
    println!("Optimal I/O block size: {} bytes", metadata.blksize());
    println!("512B blocks allocated: {}", metadata.blocks());
    println!("Last file access: {}", show_time(metadata.atime()));
    println!("Last file modification: {}", show_time(metadata.mtime()));
    println!("Last file change: {}", show_time(metadata.ctime()))
}
fn main() {
    let cfg = Config::parse();
    let metadata = if cfg.link {
        symlink_metadata(cfg.file).expect("lstat")
    } else {
        metadata(cfg.file).expect("stat")
    };

    display_stat_info(&metadata);
}
