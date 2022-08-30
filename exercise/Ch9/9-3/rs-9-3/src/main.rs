mod getgrent_r;
use getgrent_r::get_all_groups;
use nix::{
    unistd::{getgroups, setgroups, Gid, Group, User},
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
        groups.extend_from_slice(
            &get_all_groups()?
                .iter()
                .filter(|group: &&Group| group.mem.contains(&user.name))
                .map(|group: &Group| group.gid)
                .collect::<Vec<Gid>>(),
        );
    };
    setgroups(&groups)
}
