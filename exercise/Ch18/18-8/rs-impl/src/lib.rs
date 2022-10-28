mod utility;

use std::{
    fs::Metadata,
    io::Result,
    path::{Path, PathBuf},
};
use utility::TreeWalker;

/// A simplified version of [`nftw(3)`](https://man7.org/linux/man-pages/man3/nftw.3p.html)
///
/// Supports only `pre-order` traversal.
///
/// #### Example
///
/// ```rust
/// use rs_impl::nftw;
///
/// nftw("src", |path, _, level| {
///     println!("path: {}, level: {}", path.display(), level);
///     Ok(())
/// });
/// ```
pub fn nftw<P, F>(dirpath: P, f: F) -> Result<()>
    where
        P: AsRef<Path>,
        F: Fn(PathBuf, Metadata, usize) -> Result<()>,
{
    let mut walker = TreeWalker::new(dirpath.as_ref())?;
    while let Some(Ok(node)) = walker.next() {
        f(node.path, node.metadata, node.level)?;
    }
    Ok(())
}