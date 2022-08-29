use nix::unistd::{getgroups, getresgid, getresuid, setfsgid, setfsuid, Gid, ResGid, ResUid, Uid};
use ugid_functions_rs::{group_name_from_id, user_name_from_id};

fn main() {
    let res_uid: ResUid = getresuid().expect("getresuid");
    let res_gid: ResGid = getresgid().expect("getresuid");
    let fs_uid: Uid = setfsuid(Uid::from(0));
    let fs_gid: Gid = setfsgid(Gid::from(0));

    let supp_groups: Vec<Gid> = getgroups().expect("getgroups");

    println!(
        "UID: real={} ({}); effc={} ({}); saved={} ({}); fs={} ({})",
        if let Some(name) = user_name_from_id(res_uid.real) {
            name
        } else {
            "???".to_owned()
        },
        res_uid.real,
        if let Some(name) = user_name_from_id(res_uid.effective) {
            name
        } else {
            "???".to_owned()
        },
        res_uid.effective,
        if let Some(name) = user_name_from_id(res_uid.saved) {
            name
        } else {
            "???".to_owned()
        },
        res_uid.saved,
        if let Some(name) = user_name_from_id(fs_uid) {
            name
        } else {
            "???".to_owned()
        },
        fs_uid
    );

    println!(
        "GID: real={} ({}); effc={} ({}); saved={} ({}); fs={} ({})",
        if let Some(name) = group_name_from_id(res_gid.real) {
            name
        } else {
            "???".to_owned()
        },
        res_gid.real,
        if let Some(name) = group_name_from_id(res_gid.effective) {
            name
        } else {
            "???".to_owned()
        },
        res_gid.effective,
        if let Some(name) = group_name_from_id(res_gid.saved) {
            name
        } else {
            "???".to_owned()
        },
        res_gid.saved,
        if let Some(name) = group_name_from_id(fs_gid) {
            name
        } else {
            "???".to_owned()
        },
        fs_gid
    );

    print!("{} supplementary groups: ", supp_groups.len());
    for item in supp_groups.iter() {
        print!(
            "{} {}",
            if let Some(name) = group_name_from_id(*item) {
                name
            } else {
                "???".to_owned()
            },
            item,
        )
    }
    println!();
}
