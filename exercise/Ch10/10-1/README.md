#### Question

Assume a system where the value returned by the call `sysconf(_SC_CLK_TCK)` is
100. Assuming that the `clock_t` value returned by `times(2)` is a signed 32-bit
integer, how long will it take before this value cycles so that it restarts at 0?
Perform the same calculation for the `CLOCK_PER_SEC` value returned by `clock(3)`.

#### Answer

In our question, `clock_t` is an alias to `i32`. The max value of `i32` is 
`2^31-1(2147483647)`. In order to overflow, the return value has to reach its
max value. And since the return value is always positive, the time consumed
in seconds is:

```
[(2^31-1)-0]/sysconf(_SC_CLK_TCK) = 2147483647/100 = 21474836.47 seconds
```

Same calculation for `clock(3)`:

```
2^31-1/CLOCK_PER_SEC =  2147483647/1000000 = 2147.483647 seconds
```
