# Day 5 - If bruteforcing is not working, you are not using enough force

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

[grisen@windowsdatorn ~]$ printf '' | md5sum
d41d8cd98f00b204e9800998ecf8427e  -
[grisen@windowsdatorn ~]$ ( printf 'abc0' ) | md5sum
577571be4de9dcce85a041ba0410f29f  -
[grisen@windowsdatorn ~]$ ( printf 'abc1' ) | md5sum
23734cd52ad4a4fb877d8a1e26e5df5f  -
[grisen@windowsdatorn ~]$ ( printf 'abc2' ) | md5sum
63872b5565b2179bd72ea9c339192543  -

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


### implementation

```c 

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char hash[64] = {0};
    FILE *pf = NULL;
    int ret = 1;

    char cmd[256] = {0};
    const char *part1 = "printf '%s' '";
    char *string = "abc3231929";
    const char *part2 = "' | md5sum | cut -d' ' -f1";

    if (strlen(part1) + strlen(string) + strlen(part2) >= sizeof cmd) {
        fprintf(stderr, "command too long\n");
        goto out;
    }

    strcpy(cmd, part1);  /* cmd now holds part1 */
    strcat(cmd, string); /* append the data */
    strcat(cmd, part2);  /* append the rest */

    pf = popen(cmd, "r");

    if (pf == NULL) {
        fprintf(stderr, "error in popen\n");
        goto out;
    }

    uint8_t hash_ind = 0;
    int c;
    while ((c = getc(pf)) != '\n' && c != EOF) {
        hash[hash_ind++] = (char)c;
    }
    hash[hash_ind++] = 0;

    printf("hash: %s\n", hash);
    ret = 0;

out:
    if (pf != NULL) pclose(pf);
    return ret;
}
```

This gives us a hash in c, if we give it a string, using popen in read mode (stdout gets piped to pf), more complicated if we wanted it to write. Good first practice.

The rest should be easily happycoded I think.

#### Realitycheck

I got it to hashing with indexes, verified by terminal testcases. Also by giving it nice starting index it will find first character in answer. I think its working code.

Having what I think is working code, running 1000 iterations takes approx 9 seconds. Meaning it would almost take 3 hours just to run a million hashes... Ok, now I am running on battery and I do throttle the CPU on battery. Lets complete the code and then when on AC test it for 10k iterations, then 100k iterations. I dont think it will help, its all of the syscalls thats the problem, not number of calculations. I could also try having GCC on O3, still prob wont be enough.

Ok, so testing it for 10k iterations, and looking at top, it's hovering around 3-4 % of one CPU... throttling cpu will not help much. Lets test if turning up optimization will still let it find first answer. And time 10k iterations

O2 - 100 seconds for 10k iter. Finds character

O3 - 100 seconds for 10k iter. Finds character

O3 same speed as O2. => 10k seconds for a million iter. 3h per million. Its doable for a nice little stubborn bruteforce. It will not overheat my CPU either as its running around 3%. It will just give my linux something to do. Starting it once a day in the morning. The testcase gave approximately 1 character per 2 million iterations so its mayby 16h running time, twice a day, thats 4 days. Absolutely doable.

##### GENIOUS

we already have this:

```c
for (uint64_t iteration_ind = STARTING_INDEX; iteration_ind < STARTING_INDEX + MAX_PROMPTS; iteration_ind++) {
```

Just as Im contemplating this, the greatest idea comes to me. Lets implement learning THREADS for c also. This is the PERFECT usecase for it, since its the scheduler, syscalls and other processes taking all of our performance. Lets just split it out into 100k iteration each thread. And so lets do 10million iterations spread into 100 threads! Hopefully thats lets me finish it in one sitting.

For that we need to implement that its not just one answer, but each answer character also comes out with its corresponding index

### Some better ideas that I will not implement


#### Bash full circle

Probably my favorite idea right now: Just rewrite this already functioning to be CLI, take arguments
- -start-index
- -end-index
- Have bash call it with it starting new process and end with exec bash giving me terminals to read
```bash
&(./c-program.c -[@bashsyntax array however it is] -[@bashsyntax array however it is]
exec bash)
```

it would go full circle from where I started designing this challenge. Could probably pipe them to write to a file instead with flock to stop overwrites and use atomic writes. 

I will not do that because I want to practise c threads.

#### Staying in c

Either write the md5 ourselfs or import, and then use threads. Would give by far the fastest and best execution.

I will not do that because its much more fun to go my route of fixing a shitty bruteforce.

#### Limiting the number of syscalls

```
until enough answer character found
    Write one file of indexes to be hashed, one million at a time.
    Call the hasher on each line, write to new file.
    Look over the hashes with either c or grep.
```

I will not do that because its much more fun to go my route of fixing a shitty bruteforce.
