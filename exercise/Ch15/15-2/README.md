#### Question

Do you expect any of a file's three timestamps to be changed by the stat() system
call? If not, explain why?

#### Answer

No, `stat` won't touch any of them.

* atime: time of last file contents access
* mtime: time of last file contents change
* ctime: time of last file metadata change

`stat` just access `metadata`, so these 3 timestamps won't be changed.

#### Verfication

```shell
#!/usr/bin/env bash

date=$(date +%F);

touch file;
echo -n "# First call to stat at: "; date;
stat file|grep "$date";

sleep 2

echo -n "# Second call to stat at: "; date;
stat file|grep "$date";

rm file;
```

```shell
$ bash script.sh
# First call to stat at: Sun Oct  9 03:22:31 PM CST 2022
Access: 2022-10-09 15:22:31.409494090 +0800
Modify: 2022-10-09 15:22:31.409494090 +0800
Change: 2022-10-09 15:22:31.409494090 +0800
 Birth: 2022-10-09 15:22:31.409494090 +0800
# Second call to stat at: Sun Oct  9 03:22:33 PM CST 2022
Access: 2022-10-09 15:22:31.409494090 +0800
Modify: 2022-10-09 15:22:31.409494090 +0800
Change: 2022-10-09 15:22:31.409494090 +0800
 Birth: 2022-10-09 15:22:31.409494090 +0800
```
