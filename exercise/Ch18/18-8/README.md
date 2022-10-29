#### Question

Implement `nftw(3)`. (This will require the use of `opendir(3)`, `readdir(3)`, 
`closedir(3)`, and `stat(2)` system calls, among others.)


#### Answer

My implementation is simplified compared to `nftw(3)`

It provides only the `file path`, `metadata (struct stat)` and `level` arguments
for the user-defined functions. And the traversal is hard-coded to be done in
`pre-order`.

##### C impl 

```c
int32_t my_nftw(const char *dirpath, 
    int (*fn)
    (const char *fpath, const struct stat *sb, uint32_t level))
```

##### Rust impl

```rust
pub fn nftw<P, F>(dirpath: P, f: F) -> Result<()>
    where
        P: AsRef<Path>,
        F: Fn(PathBuf, Metadata, usize) -> Result<()>,
```
