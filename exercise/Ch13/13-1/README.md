#### Question

Using the time built-in command of the shell, try timing the operation of the 
program in Listing 4-1 (copy.c) on your system.

1. Experiment with different file and buffer sizes. You can set the buffer size
   using the `-DBUF_SIZE=nbytes` option when compiling the program.

2. Modify the `open(2)` syscall to include the `O_SYNC` flag. How much difference
   does this make to the speed for various buffer sizes?

3. Try performing these timing tests on a range of file systems (e.g, ext3, XFS,
   Btrfs, and JFS). Are the results similar? Are the trends the same when going
   small to large buffer sizes?

#### Answer

1. Try to copy a 10MB file:
   
   |buffer size | time   	                      |
   |------------|-------------------------------------|
   | 1 byte     | user: 0.54s sys: 15.86s tot: 16.437s|
   | 512 bytes  | user: 0.00s sys: 0.04s tot: 0.046s  |
   | 1024 bytes | user: 0.00s sys: 0.02s  tot: 0.021s |

2. Try to copy a 10MB file:

   |buffer size | time   	                      |
   |------------|-------------------------------------|
   | 1 byte     | takes forever, I gave up.           |
   | 512 bytes  | user: 0.01s sys: 1.86s tot: 21.603s |
   | 1024 bytes | user: 0.00s sys: 0.94s  tot: 9.058s |

3. Didn't try, the performances of different file systems
   are definitely different, the results should be similar
   but absolutely not same.

   The trend shoud hold.
