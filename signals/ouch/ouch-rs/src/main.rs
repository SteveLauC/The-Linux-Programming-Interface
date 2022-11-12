use nix::{
    libc::c_int,
    sys::signal::{signal, SigHandler, Signal},
};
use std::{
    thread::sleep,
    time::Duration,
};

extern "C" fn sig_handler(_signum: c_int) {
    println!("Ouch!");
    return;
}

fn main() {
    unsafe {
        signal(Signal::SIGINT, SigHandler::Handler(sig_handler)).unwrap();
    }

    let mut j = 0;
    loop {
        println!("{}", j);
        sleep(Duration::from_secs(3));

        j+=1;
    }
}
