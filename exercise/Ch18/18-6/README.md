#### Question

Modify the propgram in Listing 18-3 (`nftw_dir_tree.c`) to use the `FTW_DEPTH` flag.
Note the difference in the order in which the directory tree is traversed.

#### Answer

By default, `nftw(3)` does a `pre-order` traversal. If the `FTW_DEPTH` flag is 
present, then it traverse in a `post-order` way.

I created some files in the current directory to test this:

```shell
# Pre-order traversal
$ ./nftw dir
d D    4664066  dir
- F    4664072      file1
d D    4664074      dir1
- F    4664075          file2
- F    4664076          file3
- F    4664753      file4

# Post-order traversal
$ ./nftw dir -d
- F    4664072      file1
- F    4664075          file2
- F    4664076          file3
d DP   4664074      dir1
- F    4664753      file4
d DP   4664066  dir
```

Here is a diagram which makes it more clear:

![diagram](https://github.com/SteveLauC/pic/blob/main/photo_2022-10-26_10-43-17.jpg)

Follow the blue line in the direction of the purple arrow, the orange nodes appear
in the order of the pre-order traversal, the green one is the post-order traversal.
