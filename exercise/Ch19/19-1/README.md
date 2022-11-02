#### Question

Write a program that logs all file creations, deletions, and renames under the 
`directory` named in its command-line argument. The program should monitor events
in all of the subdirectories under the specified directory. To obtain a list of
all of the these subdirectories, you will need to make use of `nftw()`. When a
new directory is added under the tree or a direcotry is deleted, the set of 
monitored subdirectories should be updated accrodingly.

#### Algorithm

1. Initialize an `inotify` instance
2. Prepare a hashmap to store the mapping between `wd` and `watched directory`
3. For every target directory:
   1. Watch it with `IN_EXCL_UNLINK | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE | IN_MOVE_SELF` flag
   2. Use `nftw()` to iterate over its entries, if the `entry` is a direcotry, 
      also `watch` it with the same flag.
4. Read the events:

   1. `IN_CREATE`

      This will only happen for files or  directories insisde the watched directory.
      Find its parent directory through the mapping between `wd` and `watched 
      directory`, concatenate the parent path with the `name` field of `struct 
      inotify_event` to get the full path.

      Print and `stat(2)` it to get the file type. 

      if it is a direcotry, print all the files inside it (recursively), watch it and 
      all of its subdirectories.

   2. `IN_DELETE`
      
      > When a file is deleted, there are three different cases that we need to handle.
      >
      > 1. root directory
      >    
      >    We will receive `IN_DELETE_SELF` event.
      >
      > 2. directory other than root
      >
      >    Receive `IN_DELETE` and `IN_DELETE_SELF` events.
      > 
      > 3. a regular file
      >
      >    Receive `IN_DELETE` event. 

      This will only happen for files or  directories insisde the watched directory.
      Find its parent directory through the mapping, concatenate the parent path 
      with the `name` field of `struct inotify_event` to get the full path.

      print it. 

   3. `IN_DELETE_SELF`

      1. If this is the root (i.e., the root of this tree), print it, close the 
         `inotify` file descriptor and exit the program with status 0.

      2. It is a non-root directory, remove it from the mapping and remove the 
         watch (kernrl will do this for us). (NO PRINT in this branch as it is
	 printed in `step 2`)

   4. `IN_MOVE_SELF`

      > When a file is moved, there are three different cases that we need to handle.
      >
      > 1. root directory
      >    
      >    We will receive `IN_MOVE_SELF` event.
      >
      > 2. directory other than root
      >
      >    Receive `IN_MOVE_SELF` and `IN_MOVED_FROM` events.
      > 
      > 3. a regular file
      >
      >    Receive `IN_MOVED_FROM` event. 

      1. If it is the root that has been moved, print it, close the `inotify` 
         instance and exit. 
      2. If it is a non-root directory, remove it from the mapping and remove
         the watch. (NO PRINT in this branch, will print it in the next step)

   5. `IN_MOVE_FROM`

      1. Get the watched directory through the mapping, concatenate it with the
         `name` field of `struct inotify_event` to get the full path, print it.
      

   6. `IN_MOVE_TO`

      1. Get the watched directory through the mapping, concatenate it with the
         `name` field of `struct inotify_event` to get the full path, print it
	 to stdout and `stat(2)` it. 

      2. If the moved file is a regular file, do nothing.
      3. If it is a directory, watch it and all its subdirectories.
       
#### Answer

In my C implementation, we monitor only one file. In the Rust implementation, 
monitoring multiple files is allowed, though one should be careful not to make
paths overlap (e.g., "/home" and "/home/steve").

#### Verify

```shell
# session 1

$ cd ~/Desktop

$ touch file
$ mkdir dir
$ rm file
$ rmdir dir

$ mkdir dir1
$ mkdir dir1/dir2
$ mkdir dir1/dir2/dir3
$ touch dir1/file
$ touch dir1/dir2/file
$ touch dir1/dir2/dir3/file
$ rm -r dir1


$ touch file
$ mv file ../
$ mv ../file ./
$ mkdir dir
$ touch dir/file
$ mv dir ../
$ mv ../dir .



$ mkdir dir1/dir2/dir3 -p
$ touch dir1/file
$ touch dir1/dir2/file
$ touch dir1/dir2/dir3/file
$ rm -r dir1
```

```shell
# session 2

$ cd c-impl
$ make build
$ ./watch /home/steve/Desktop
/home/steve/Desktop/file was created
/home/steve/Desktop/dir was created
file was deleted
dir was deleted


/home/steve/Desktop/dir1 was created
/home/steve/Desktop/dir1/dir2 was created
/home/steve/Desktop/dir1/dir2/dir3 was created
/home/steve/Desktop/dir1/file was created
/home/steve/Desktop/dir1/dir2/file was created
/home/steve/Desktop/dir1/dir2/dir3/file was created
file was deleted
dir3 was deleted
file was deleted
dir2 was deleted
file was deleted
dir1 was deleted

/home/steve/Desktop/file was created
file was moved
file was moved to /home/steve/Desktop
/home/steve/Desktop/dir was created
/home/steve/Desktop/dir/file was created
dir was moved
dir was moved to /home/steve/Desktop


/home/steve/Desktop/dir1 was created
/home/steve/Desktop/dir1/dir2 was created
/home/steve/Desktop/dir1/dir2/dir3 was created
/home/steve/Desktop/dir1/file was created
/home/steve/Desktop/dir1/dir2/file was created
/home/steve/Desktop/dir1/dir2/dir3/file was created
file was deleted
dir3 was deleted
file was deleted
dir2 was deleted
file was deleted
dir1 was deleted
```

#### Minor Issue

In my C implementation, when you watch `/home/steve/Desktop`, there will be two
duplicate entries in the `Mappings`:

```
wd: 0
watched_file: /home/steve/Desktop
is_root: 1

wd: 1
watched_file: /home/steve/Desktop
is_root: 1
```

It won't affect how my program works since when searching matched entries, we are
searching from front to back.
