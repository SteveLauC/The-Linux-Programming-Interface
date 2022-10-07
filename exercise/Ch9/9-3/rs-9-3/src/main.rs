mod getgrent_r;

use getgrent_r::*;
use nix::{
    unistd::{getgroups, setgroups, Gid, User},
    Result,
};

fn main() {
    my_initgroups("steve", Gid::from_raw(999)).unwrap();

    for grp in getgroups().expect("getgroups") {
        print!("{} ", grp);
    }
}

fn my_initgroups(user: &str, group: Gid) -> Result<()> {
    let mut groups: Vec<Gid> = vec![group];

    // if `user` exists
    if let Some(user) = User::from_name(user)? {
        groups.push(user.gid);
        groups.extend(
            AllGroups::new()
                .map(|opt_grp| opt_grp.unwrap())
                .filter(|group| group.mem.contains(&user.name))
                .map(|group| group.gid),
        );
    };
    setgroups(&groups)
}
