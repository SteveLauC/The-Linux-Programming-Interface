#### Question

Write a program that measures the time required to create and then remove a 
large number of 1-byte files from a single directory. The program should 
create files with names of the form xNNNNNN where NNNNNN is replaced by a 
random six-digit number. The file should be created in the random order in 
which their names are generated, and then deleted in increasing numberical order 
(i.e., an order that is different from that in which they were created). 
The number of files (NF) and the directory in which they are to be created should 
be specifiable on the command line. Measure the times required for diffferent values
of NF (e.g., in the range from 10000 to 20,0000) and for different file systems 
(e.g. ext2, ext3, and XFS). What patterns do you observe on each file system as 
NF increases? Do the results change if the files are created in increasing 
numerical order (x000001, x000002, x000003, and so on) and then be deleted in the 
same order? If so, what do you think the reason(s) might be? Again, do the results vary
across file-system types?

#### Test

* Btrfs

  ```shell
  $ cd c-14-1
  $ make
  
  
  $ ./14-1 -n 100000 -o
  File system is: btrfs
  Benchmarking (Creating and deleting 100000 file in an **ordered** way...)
  Consumed 2.920844 seconds⏎
  
  $ ./14-1 -n 100000
  File system is: btrfs
  Benchmarking (Creating and deleting 100000 file in a **random** way...)
  Consumed 3.018916 seconds⏎
  ```

* Tmpfs

  ```shell
  $ mkdir tmpfs_mount_point
  $ sudo mount -t tmpfs source tmpfs_mount_point
  $ cp 14-1 tmpfs_mount_point
  $ cd tmpfs_mount_point
  $ ./14-1 -n 100000 -o
  File system: tmpfs
  Benchmarking (Creating and deleting 100000 file in an **ordered** way...)
  Consumed 0.681416 seconds⏎
  
  $ ./14-1 -n 100000
  File system: tmpfs
  Benchmarking (Creating and deleting 100000 file in a **random** way...)
  Consumed 0.684663 seconds⏎
  ```

* Ext4
  
  > This file system resides on my external SSD.
  
  ```shell
  $ ./14-1 -n 100000 -o
  File system: ext2/ext3
  Benchmarking (Creating and deleting 100000 file in an **ordered** way...)
  Consumed 1.972989 seconds⏎
  
  $ ./14-1 -n 100000
  File system: ext2/ext3
  Benchmarking (Creating and deleting 100000 file in a **random** way...)
  Consumed 1.986293 seconds⏎
  ```

* NTFS

> This file system resides on my external SSD.

```shell
$ ./14-1 -n 100000 -o
File system: fuseblk
Benchmarking (Creating and deleting 100000 file in an **ordered** way...)
Consumed 12.042762 seconds⏎

$ ./14-1 -n 100000
File system: fuseblk
Benchmarking (Creating and deleting 100000 file in a **random** way...)
Consumed 13.025892 seconds⏎
```
