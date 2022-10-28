use std::{
    fs::{metadata, read_dir, Metadata, canonicalize},
    io::Result,
    path::{Path, PathBuf},
};


#[derive(Debug)]
pub struct Node {
    pub metadata: Metadata,
    pub path: PathBuf,
    pub level: usize,
}

impl Node {
    #[inline]
    fn new(metadata: Metadata, path: PathBuf, level: usize) -> Self {
        Self {
            metadata,
            path,
            level,
        }
    }

    #[inline]
    fn is_dir(&self) -> bool {
        self.metadata.is_dir()
    }

    #[inline]
    fn path(&self) -> &Path {
        self.path.as_path()
    }
}

#[derive(Default, Debug)]
pub struct TreeWalker {
    stack: Vec<Node>,
    fatal_error: bool,
}


impl TreeWalker {
    /// Construct a [`TreeWalker`] instance.
    pub fn new<P: AsRef<Path>>(start: P) -> Result<Self> {
        let root_metadata = metadata(start.as_ref())?;
        let mut walker = TreeWalker::default();
        // Absolutize `start`
        // We can guarantee `start` is a valid path, so `unwrap()` here.
        let start = canonicalize(start.as_ref()).unwrap();

        let root_node = Node::new(root_metadata, start, 0);
        walker.stack.push(root_node);

        Ok(walker)
    }
}

impl Iterator for TreeWalker {
    type Item = Result<Node>;

    fn next(&mut self) -> Option<Self::Item> {
        // To avoid dead loop
        if self.fatal_error {
            return None;
        }

        if let Some(node) = self.stack.pop() {
            // If the popping node is a directory, push its files to the stack.
            if node.is_dir() {
                // To do a pre-order traversal, we have to use a temporary stack to
                // reverse the order of its files.
                let mut temp_stack = Vec::with_capacity(5);

                let dir = match read_dir(node.path()) {
                    Ok(d) => d,
                    Err(e) => {
                        self.fatal_error = false;
                        return Some(Err(e));
                    }
                };

                for res_entry in dir {
                    let entry = match res_entry {
                        Ok(e) => e,
                        Err(e) => {
                            self.fatal_error = true;
                            return Some(Err(e));
                        }
                    };

                    temp_stack.push(entry);
                }

                // push its files into the stack
                while let Some(entry) = temp_stack.pop() {
                    let entry_metadata = match entry.metadata() {
                        Ok(m) => m,
                        Err(e) => {
                            self.fatal_error = true;
                            return Some(Err(e));
                        }
                    };
                    self.stack.push(Node::new(entry_metadata, entry.path(), node.level + 1));
                }
            }

            return Some(Ok(node));
        }

        // stack is empty, traversal is done.
        None
    }
}
