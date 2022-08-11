use nix::unistd::{Gid, Group, Uid, User};

pub fn user_name_from_id(uid: Uid) -> Option<String> {
    Some(User::from_uid(uid).ok()??.name)
}

pub fn user_id_from_name(name: &str) -> Option<Uid> {
    if let Ok(raw_uid) = name.parse() {
        return Some(Uid::from_raw(raw_uid));
    }

    Some(User::from_name(name).ok()??.uid)
}

pub fn group_name_from_id(gid: Gid) -> Option<String> {
    Some(Group::from_gid(gid).ok()??.name)
}

pub fn group_id_from_name(name: &str) -> Option<Gid> {
    if let Ok(raw_gid) = name.parse() {
        return Some(Gid::from_raw(raw_gid));
    }

    Some(Group::from_name(name).ok()??.gid)
}
