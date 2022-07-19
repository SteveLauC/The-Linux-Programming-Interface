//! dup.rs: make File duplicable

use nix::{
    fcntl::{fcntl, FcntlArg, OFlag},
    unistd::dup,
};
use std::{
    fs::File,
    io::Error,
    os::unix::{
        io::{AsRawFd, RawFd},
        prelude::FromRawFd,
    },
};

pub trait Dup
where
    Self: Sized,
{
    fn dup(&self) -> Result<Self, Error>;
}

impl Dup for File {
    fn dup(&self) -> Result<Self, Error> {
        let self_fd: RawFd = self.as_raw_fd();

        match dup(self_fd) {
            Ok(fd) => unsafe { Ok(File::from_raw_fd(fd)) },
            Err(errno) => Err(Error::from(errno)),
        }
    }
}

pub trait StatusFlag {
    fn get(&self) -> Result<OFlag, Error>;
    fn set(&self, flag: OFlag) -> Result<(), Error>;
}

impl StatusFlag for File {
    fn get(&self) -> Result<OFlag, Error> {
        let fd: RawFd = self.as_raw_fd();

        match fcntl(fd, FcntlArg::F_GETFL) {
            Ok(flag) => Ok(OFlag::from_bits_truncate(flag)),
            Err(errno) => Err(Error::from(errno)),
        }
    }

    fn set(&self, flag: OFlag) -> Result<(), Error> {
        let fd: RawFd = self.as_raw_fd();
        let mut prev_flag: OFlag = self.get()?;
        prev_flag.insert(flag);

        if let Err(errno) = fcntl(fd, FcntlArg::F_SETFL(prev_flag)) {
            return Err(Error::from(errno));
        }
        Ok(())
    }
}
