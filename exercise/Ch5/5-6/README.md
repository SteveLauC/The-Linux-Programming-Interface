#### QUESTION

after each of the calls to write() in the following code, explain what
the content of the output file would be, and why:

```c
fd1 = open(file, O_RDWR|O_CREAT|O_TRUNC, 0600);
fd2 = dup(fd1);
fd3 = open(file, O_RDWR);

write(fd1, "Hello,", 6);
write(fd2, "world", 6);
lseek(fd2, 0, SEEK_SET);
write(fd1, "Hello,", 6);
write(fd3, "Gidday", 6);
```

#### ANSWER

```
Giddayworld
```

#### EXPLANATION:
	
fd1 and fd2 are duplicated file descriptors, and thus share the same offset.
fd3 is another file descriptor referring, but all of them are the same file.

```
write(fd1, "Hello,", 6);
file content: "Hello,"        offset of fd1 and fd2: 6  offset of fd2: 0
write(fd2, "world", 6);
file content: "Hello,world\0" offset of fd1 and fd2: 12 offset of fd3: 0
lseek(fd2, 0, SEEK_SET);
file content: "Hello,world\0" offset of fd1 and fd2: 0  offset of fd3: 0
write(fd1, "Hello,", 6);
file content: "Hello,world\0" offset of fd1 and fd2: 6  offset of fd3: 0
write(fd3, "Gidday", 6);
file content: "Giddayworld\0" offset of fd1 and fd2: 6  offset of fd3: 6
```
