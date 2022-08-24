#### Question

##### Wrong Question
When we execute the following code, we find that it displays the same number twice,
even though the two users have different IDs in the password file. Why is that?

```c
printf("%ld %ld\n", (long)getpwnam("avr")->pw_uid, (long)getpwnam("tsr")->pw_uid);
```

##### Revised Version (go [errata](https://man7.org/tlpi/errata/) to see it)
When we execute the following code, which attempts to display the usernames for 
two different user IDs, we find that it displays the same username twice. Why 
is this?

```c
printf("%s %s\n", getpwuid(uid1)->pw_name, getpwuid(uid2)->pw_name);
```

#### Vertification

> This test code uses `getpwnam` instead of `getpwuid`, this does not affect
> the result.

```c
// verify.c

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s %s\n", getpwnam("steve")->pw_name, getpwnam("root")->pw_name);
	printf("%d %d\n", getpwnam("steve")->pw_uid, getpwnam("root")->pw_uid);

	return EXIT_SUCCESS;
}
```

```shell
$ gcc verify.c && ./a.out
steve steve
1000 0
```

#### Answer

The pointer returned by `getpwnam` pointing to a statically allocated memory,
so do the pointer fields of `struct passwd`. And these static values are 
nonreentrant, which means they can be overritten by the subsequent calls.

Since they are static, multiple calls of `getpwnam` will always return the same
address.

```c
printf("%s %s\n", getpwnam("steve")->pw_name, getpwnam("root")->pw_name);
```
Why does this print two `steve`? The key point is we can not control the execution
order of these two `getpwnam` functions, and since the returned addresses of these
two calls are the same one, the result of this statement can be `steve steve` or 
`root root` (depends on which executes lastly).

```c
printf("%d %d\n", getpwnam("steve")->pw_uid, getpwnam("root")->pw_uid);
```

But why does this statement always returns the right result? Because `pw_uid` is
numeric type, which is passed by value so that we will get two duplicates inside 
the stack frame of `print (or vsprintf as printf calls vsprintf inside it)`.
