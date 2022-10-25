use std::{
    collections::VecDeque,
    ffi::OsString,
    path::{Path, PathBuf},
};

#[derive(Debug)]
pub struct Paths {
    pub parsed: PathBuf,
    pub remained: VecDeque<OsString>,
}

impl Paths {
    /// Construct a new [`Paths`] struct
    pub fn new<P>(parsed: Option<PathBuf>, remained: Option<P>) -> Self
    where
        P: AsRef<Path>,
    {
        let parsed = match parsed {
            Some(p) => p,
            None => PathBuf::new(),
        };
        let remained = match remained {
            Some(r) => r
                .as_ref()
                .components()
                .map(|com| com.as_os_str().to_owned())
                .collect(),
            None => VecDeque::new(),
        };

        Self { parsed, remained }
    }

    #[inline]
    /// Replaces `self.parsed` with `new_parsed`
    pub fn replace_parsed_with(&mut self, new_parsed: PathBuf) {
        self.parsed = new_parsed;
    }

    #[inline]
    pub fn parsed_push_back<P: AsRef<Path>>(&mut self, entry: P) {
        self.parsed.push(entry);
    }

    #[inline]
    pub fn parsed_cd_to_parent(&mut self) {
        if let Some(parent) = self.parsed.parent() {
            let parent_len = parent.as_os_str().len();

            // Note that to avoid allocating memory, we directly alter the `length`
            // field of `self.parsed (PathBuf)`.
            assert!(parent_len <= self.parsed.capacity());
            let mut p_to_heap_memory = &mut self.parsed as *mut PathBuf as *mut usize;
            unsafe {
                p_to_heap_memory = p_to_heap_memory.add(2);
                *p_to_heap_memory = parent_len;
            }
            assert!(self.parsed.as_os_str().len() == parent_len);
        }
    }

    #[inline]
    pub fn remained_next_entry(&mut self) -> Option<OsString> {
        self.remained.pop_front()
    }

    #[inline]
    pub fn remained_push_front<P: AsRef<Path>>(&mut self, entry: P) {
        entry
            .as_ref()
            .components()
            .rev()
            .map(|com| com.as_os_str().to_owned())
            .for_each(|item| {
                self.remained.push_front(item);
            });
    }
}

#[inline]
pub fn is_dot<P: AsRef<Path>>(path: P) -> bool {
    path.as_ref() == Path::new(".")
}

#[inline]
pub fn is_a_pair_of_dots<P: AsRef<Path>>(path: P) -> bool {
    path.as_ref() == Path::new("..")
}
