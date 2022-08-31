#### Question

Repeat the previous [exercise](https://github.com/SteveLauC/The-Linux-Programming-Interface/tree/main/exercise/Ch9/9-4) 
for a process executing a set-UID-root program, which has the following initial
set of process credentials.

```
real=X eff=0 saved=0
```

> 1. Suspend and resume the set-UID identity(i.e., switch the `EUID` to the value
>    of RUID and then back to the `SUID`).
>
> 2. Permanently drop the set-UID identity (i.e., ensure that the `EUID` and `SUID`
>    are set to the value of `RUID`)


#### Answer

##### answer for `1`

```c
euid = geteuid();
seteuid(getuid()); seteuid(euid); // or
setreuid(-1, getuid()); setreuid(-1, euid); // or
setresuid(-1, getuid(), -1); setresuid(-1, euid, -1);
```

`setuid` can't be used here as this process is priviledged, removing the priviledge
is a one-way trip.

##### answer for `2`

```c
setuid(getuid()); // or
setreuid(getuid(), getuid()); // or
setresuid(-1, getuid(), getuid()); // or
```
