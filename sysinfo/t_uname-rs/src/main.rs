use nix::sys::utsname::{uname, UtsName};

fn main() {
    let uts: UtsName = uname().expect("uname(2)");
    println!("Node name:    {:?}", uts.nodename());
    println!("System name:  {:?}", uts.sysname());
    println!("Release:      {:?}", uts.release());
    println!("Version :     {:?}", uts.version());
    println!("Machine:      {:?}", uts.machine());
}
