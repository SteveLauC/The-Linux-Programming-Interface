use nix::unistd::{sysconf, SysconfVar};
use std::process::exit;

fn main() {
    sysconf_print("_SC_ARG_MAX", SysconfVar::ARG_MAX);
    sysconf_print("_SC_LOGIN_NAME_MAX", SysconfVar::LOGIN_NAME_MAX);
    sysconf_print("_SC_OPEN_MAX", SysconfVar::OPEN_MAX);
    sysconf_print("_SC_NGROUPS_MAX", SysconfVar::NGROUPS_MAX);
    sysconf_print("_SC_PAGE_SIZE", SysconfVar::PAGE_SIZE);
    sysconf_print("_SC_RTSIG_MAX", SysconfVar::RTSIG_MAX);
}
fn sysconf_print(msg: &str, name: SysconfVar) {
    match sysconf(name) {
        Ok(Some(val)) => println!("{} {}", msg, val),
        Ok(None) => println!("{} (indeterminate)", msg),
        Err(_) => {
            eprintln!("sysconf {}", msg);
            exit(1);
        }
    }
}
