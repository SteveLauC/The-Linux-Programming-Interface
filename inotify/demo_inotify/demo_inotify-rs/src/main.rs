use inotify::{Event, EventMask, Inotify, WatchMask};
use libc::{inotify_event, FILENAME_MAX};
use std::{env::args, mem::size_of, process::exit};
use std::ffi::OsStr;

const BUF_LEN: usize = 10 * (size_of::<inotify_event>() + FILENAME_MAX as usize + 1);

fn main() {
    let av: Vec<String> = args().collect();
    if av.len() < 2 || av[1] == "--help" {
        eprintln!("Usage: {} pathname...", av[0].as_str());
        exit(1);
    }
    let mut inotify = Inotify::init().expect("inotify_init1");

    for item in &av[1..] {
        let wd = inotify
            .add_watch(item, WatchMask::ALL_EVENTS)
            .expect("inotify_add_watch");
        println!("Watching [{}] using wd [{:?}]\n", item, wd);
    }

    let mut buf = [0; BUF_LEN];
    loop {
        let events = inotify.read_events_blocking(&mut buf).expect("read");
        events.for_each(|e| display_inotify_event(&e));
    }
}

fn display_inotify_event(event: &Event<&OsStr>) {
    print!("    wd = {:?}; ", event.wd);
    if event.cookie != 0 {
        print!("cookie = {}; ", event.cookie);
    }
    print!("mask = ");
    let mask = event.mask;
    if mask.contains(EventMask::ACCESS) {
        print!("IN_ACCESS ");
    }
    if mask.contains(EventMask::ATTRIB) {
        print!("IN_ATTRIB ");
    }
    if mask.contains(EventMask::CLOSE_NOWRITE) {
        print!("IN_CLOSE_NOWRITE ");
    }
    if mask.contains(EventMask::CLOSE_WRITE) {
        print!("IN_CLOSE_WRITE ");
    }
    if mask.contains(EventMask::CREATE) {
        print!("IN_CREATE ");
    }
    if mask.contains(EventMask::DELETE) {
        print!("IN_DELETE ");
    }
    if mask.contains(EventMask::DELETE_SELF) {
        print!("IN_DELETE_SELF ");
    }
    if mask.contains(EventMask::IGNORED) {
        print!("IN_IGNORED ");
    }
    if mask.contains(EventMask::ISDIR) {
        print!("IN_ISDIR ");
    }
    if mask.contains(EventMask::MODIFY) {
        print!("IN_MODIFY ");
    }
    if mask.contains(EventMask::MOVE_SELF) {
        print!("IN_MOVE_SELF ");
    }
    if mask.contains(EventMask::MOVED_FROM) {
        print!("IN_MOVED_FROM ");
    }
    if mask.contains(EventMask::MOVED_TO) {
        print!("IN_MOVED_TO ");
    }
    if mask.contains(EventMask::OPEN) {
        print!("IN_OPEN ");
    }
    if mask.contains(EventMask::Q_OVERFLOW) {
        print!("IN_Q_OVERFLOW ");
    }
    if mask.contains(EventMask::UNMOUNT) {
        print!("IN_UNMOUNT ");
    }
    println!("");

    if let Some(name) = event.name {
        println!("        name = {:?}", name);
    }
}
