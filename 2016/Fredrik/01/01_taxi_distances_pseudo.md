# Day 1

## part a

### idea

```
Im thinking of having one struct for the person, where it has the coordinates and current direction. 

Coordinates [X, Y] starts at [0, 0] at start.

Direction is like the angle for sin/cos, starting 1 north
    East    0
    North   1
    West    2
    South   3
    East    0
    ...

Then we can have the index in an array that will overflow to correct angle
        obviously this didnt work, but its handled in execution

Turning right: index --
Turning left:  index ++

and a function pointer-taking old pos, applying opcode, new position

final distance is simple pythagoras from origin

```

### details

```c
typedef struct {
    int64_t x;       
    int64_t y;      
    int64_t dir_index;  
} Position;

static const uint8_t direction_from_ind_arr[] = {0, 1, 2, 3}; // {east, north, west, south}

typedef struct {
    int8_t turn;     // -1 or 1
    uint64_t steps;  
} Opcode;
```

### execution

```
initialize memalloc struct
initialize Position and direction_from_ind_arr
transform input to opcodes:
    find out how many opcodes with rowcolcunt
    allocate an array of opcodes
    fill the array with a modified rowcolcunt
for all opcodes:
    move(&Position, opcode_array[i])
do the pythagoras
```

## part b

### idea

```
We store all the positions we have been in whenever we finish a move. As a stop (Position struct), we waste the direction but lets not bother.

We could use varrays but I think its easier to just use an allocated array of size (number_of_opcodes + 1(startpos))

So after a store look for duplicate stop if not found already

```
### details

```c
uint8_t duplicate_found = 0;

heap.stop_array = malloc((number_of_opcodes + 1) * sizeof(Position));
if (!heap.stop_array) goto error;
Position *stop_array = heap.stop_array;
```

### execution
```

We wing it

```
## part b attempt 2

```
After "finishing" partb and debugging and testing testcase i realize its not duplicate STOP, but rather duplicate COORD we visit

I wish i went with the varrays instead, would have made it much easier from here.

```
### idea

```
We still use stops array but fill it out with all the coords in between.

```
~~We have to reallocate and copy the stop_array as is fills up.~~
```

I can't be bothered, we try to just bruteforce it with a VERY large stop_array and see if its enough

```
### details

### execution
```

~~we wing it.~~

got messy. new idea:
in the move, we split the actual moving into each steps.
each step adds its coord to the stop_array and looks for duplicate.

```
