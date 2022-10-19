#### Question

Write a program that displays the permissions from the ACL entry that corresponds
to a particular user or group. The program should take two command-line arguments.
The first argument is iether of the letters u or g, indicating whether the second 
argument identifies a user or group. (The functions defined in Listing 8-1, on page 
159, can be used to allow the second command-line argument to be specified numerically
or as a name.) If the ACL entry that corresponds to the given user or group falls 
into the group class, then the program should additionally display the permissions 
that would apply after the ACL entry has been modified by the ACL mask entry.

#### Answer

See [rs-impl](./rs-impl)

```shell
$ cd rs-impl
$ cargo b -q
$ setfacl -m g:docker:rw src
$ getfacl -c src
user::rwx
group::r-x
group:docker:rw-
mask::rwx
other::r-x
$ ./target/debug/rs-impl u steve src 
rwx
$ ./target/debug/rs-impl g 1000 src
r-x
$ ./target/debug/rs-impl g docker src
rw-
$ ./target/debug/rs-impl u daemon src
Entry does not exist
```
