#### Question

If a process whose user IDs all have the value `X` executes a set-user-ID program
whose user ID, `Y`, is nonzero, then the process credentials are set as follows:

```
real=X eff=Y saved=Y
```

(We ignore the file-system user ID, since it tracks the effictieve user ID.)
Show the `setuid()`, `seteuid()`, `setreuid()` and `setresuid()` calls, 
respectively, that would be used to prefrom the following operations.

1. Suspend and resume the set-UID identity(i.e., switch the `EUID` to the value
   of RUID and then back to the `SUID`).

2. Permanently drop the set-UID identity (i.e., ensure that the `EUID` and `SUID`
   are set to the value of `RUID`)

(This exercise also requires the use of `getuid()` and `geteuid()` to retrieve
the process's real and effective user IDs.) Note that for certain of the syscalls
listed above, some of these operations can't be performed.


#### Answer

Let's name the first process `A`, second process `B`, then `A` is the parent 
process of `B`. When `B` is created, it inherit `RUID` from its parent 
(i.e. `X`). Since `set-UID` is set and the owner of this programn is `Y`, 
`EUID` and `SUID` all have the value `Y`.


##### answer for `1`: 

```c
euid = geteuid()

setuid(getuid());setuid(euid); // or
seteuid(getuid()); seteuid(euid) // or 
setreuid(-1, getuid()); setreuid(-1, euid) // or
setresuid(-1, getuid(), -1); setresuid(-1, euid, -1)
```

##### answer for `2`:

```c
setreuid(getuid(), getuid()) // or 
setresuid(-1, getuid(), getuid()) // or
setresuid(getuid(), getuid(), getuid())
```

> NOTE: `setreuid` is a magical syscall.
>
> `setreuid(getuid(), getuid())` will update `SUID` cause `getuid()` is not `-1
> (or MAX value)`
> 
> `setreuid(-1, getuid())` will not...
