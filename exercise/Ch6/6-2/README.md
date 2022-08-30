#### QUESTION

write a program to see what happens if we try to longjmp() into a 
function that has already returned.


#### ANSWER

the behavior is undefined

```shell
$ ./a.out
return point is set
successfully jumped
return point is set
[1]    136122 segmentation fault (core dumped)  ./a.out
```
