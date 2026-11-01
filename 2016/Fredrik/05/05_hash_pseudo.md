# Day 5

## part a

I want to practice some more linux - bash - c integration, and there is a built-in hasher for md5 as can be seen in testing. I'm thinking of what would be the best things to practice / learn.
- using c to do terminal calls
- using bash to call c with cli, args kwargs
both are 2 things I really want to practice.

### testing

``` 
c-logic:
md5sum(printf('abc'))

experimenting with echo instead lets one learn that echo contains a newline also...
so lets not use echo.

[grisen@windowsdatorn ~]$ ( printf 'abc' ) | md5sum
900150983cd24fb0d6963f7d28e17f72  -
[grisen@windowsdatorn ~]$ ( printf 'abc3231929' ) | md5sum
00000155f8105dff7f56ee10fa9b9abd  -
[grisen@windowsdatorn ~]$ ( echo 'abc3231929' ) | md5sum
4bf969f4a5cafa59897d0a87d633ce3f  -

```

### ideas

Even just this testcase given by the task shows that we need to test 3 million times here. Thats at least 6 million sys-calls going the linux route. This one could get really slow.

echo piped to md5sum piped to cut: 9 million syscalls... at least.

```
[grisen@windowsdatorn ~]$ ( printf 'abc3231929' ) | md5sum | cut -d' ' -f1
00000155f8105dff7f56ee10fa9b9abd
```

But it gives us the string as we want it. We will try this out and see how slow it gets, limit all loops to I dont know, 10 million to begin with.

I suspect I will have to give up on the linux-way and stay inside c for performance reasons but I want to try it out.

I can also see how this day 5 could be done entirely in bash, but i couldnt do it without major ai assistance, and i want to practise c calling tty, so lets not do that.
