use clap::{builder::Command, Arg};
use nix::mount::{mount, MsFlags};

struct Config {
    source: Option<String>,
    target: String,
    fstype: Option<String>,
    data: Option<String>,
    flags: MsFlags,
}

fn main() {
    let cfg = arg_parsing();
    if mount(
        cfg.source.as_deref(),
        cfg.target.as_str(),
        cfg.fstype.as_deref(),
        cfg.flags,
        cfg.data.as_deref(),
    )
    .is_err()
    {
        eprintln!("mount");
    }
}

fn arg_parsing() -> Config {
    let mth = Command::new("t_mount")
        .arg(
            Arg::new("data")
                .short('o')
                .takes_value(true)
                .required(false),
        )
        .arg(
            Arg::new("fstype")
                .short('t')
                .takes_value(true)
                .required(false),
        )
        .arg(
            Arg::new("mountflags")
                .short('f')
                .takes_value(true)
                .required(false),
        )
        .arg(Arg::new("source").takes_value(true).required(true))
        .arg(Arg::new("target").takes_value(true).required(true))
        .override_help(
            r#"Usage: {} [options] source target


Available options:

    -t fstype        [e.g., 'ext2' or 'reiserfs']
    -o data          [file system-dependent options,
                     e.g., 'bsdgroups' for ext2]
    -f mountflags    can include any of:
        b - MS_BIND         create a bind mount
        d - MS_DIRSYNC      synchronous directory updates
        l - MS_MANDLOCK     permit mandatory locking
        L - MS_LAZYATIME    lazy atime updates
        m - MS_MOVE         atomically move subtree
        A - MS_NOATIME      don't update atime (last access time)
        V - MS_NODEV        don't permit device access
        D - MS_NODIRATIME   don't update atime on directories
        E - MS_NOEXEC       don't allow executables
        S - MS_NOSUID       disable set-user/group-ID programs
        p - MS_PRIVATE      mark as private
        r - MS_RDONLY       read-only mount
        c - MS_REC          recursive mount
        T - MS_RELATIME     relative atime updates
        R - MS_REMOUNT      remount
        h - MS_SHARED       mark as shared
        v - MS_SLAVE        mark as slave
        s - MS_SYNCHRONOUS  make writes synchronous
        u - MS_UNBINDABLE   mark as unbindable"#,
        )
        .get_matches();

    let flags = if let Some(flags) = mth.get_one::<&str>("mountflags") {
        let mut f = MsFlags::empty();
        for char in flags.chars() {
            match char {
                'b' => f.insert(MsFlags::MS_BIND),
                'd' => f.insert(MsFlags::MS_DIRSYNC),
                'l' => f.insert(MsFlags::MS_MANDLOCK),
                'm' => f.insert(MsFlags::MS_MOVE),
                'A' => f.insert(MsFlags::MS_NOATIME),
                'V' => f.insert(MsFlags::MS_NODEV),
                'D' => f.insert(MsFlags::MS_NODIRATIME),
                'E' => f.insert(MsFlags::MS_NOEXEC),
                'S' => f.insert(MsFlags::MS_NOSUID),
                'p' => f.insert(MsFlags::MS_PRIVATE),
                'r' => f.insert(MsFlags::MS_RDONLY),
                'c' => f.insert(MsFlags::MS_REC),
                'T' => f.insert(MsFlags::MS_RELATIME),
                'R' => f.insert(MsFlags::MS_REMOUNT),
                'h' => f.insert(MsFlags::MS_SHARED),
                'v' => f.insert(MsFlags::MS_SLAVE),
                's' => f.insert(MsFlags::MS_SYNCHRONOUS),
                'u' => f.insert(MsFlags::MS_UNBINDABLE),
                _ => unimplemented!("not supported"),
            }
        }

        f
    } else {
        MsFlags::empty()
    };

    Config {
        source: Some(
            mth.get_one::<String>("source")
                .expect("clap error")
                .to_owned(),
        ),
        target: mth
            .get_one::<String>("target")
            .expect("clap error")
            .to_owned(),
        fstype: mth.get_one::<String>("fstype").map(|item| item.to_owned()),
        data: mth.get_one::<String>("data").map(|item| item.to_owned()),
        flags,
    }
}
