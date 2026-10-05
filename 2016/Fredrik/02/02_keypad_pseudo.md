# Day 2

## part a

Lets go against best practices and instead make blazing fast program

### keypad = x + 3y

| k | k | k |
|---|---|---|
| 1 | 2 | 3 |
| 4 | 5 | 6 |
| 7 | 8 | 9 |

### x

| x | x | x |
|---|---|---|
| 1 | 2 | 3 |
| 1 | 2 | 3 |
| 1 | 2 | 3 |

### y

| y | y | y |
|---|---|---|
| 0 | 0 | 0 |
| 1 | 1 | 1 |
| 2 | 2 | 2 |

### verify input

I want to store the final keypress sequence as a uint64_t. So we need to verify that there are less rows than digits in max uint64_t. Does uint128_t exist? I have never seen it.

### general idea

```
Main flow:
while c = getchar != EOF:

switch(c)
'LDUR': modfiy x/y
'\n': keypress: uint64_t result = result * 10 + (x + 3*y)
'\r': carriage return and other stuff: break

```
