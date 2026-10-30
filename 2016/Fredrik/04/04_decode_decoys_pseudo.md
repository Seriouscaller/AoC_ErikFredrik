# Day 4

## part a

This is a real challenge for me, im not used to advanced string manipulation

### ideas

For the most part i will just have to get started and see what happens but some general ideas. I suspect main() will grow to be horribly big. And then good challenge to clean up
- use linelengthfinder to see how long the longest line is
- go over input file, line by line, fgets or what it is?
    - split it into sub sub-parts
        - text part
        - number part
        - checksum_input
    - make the checksum_calc ourself and verify


### implementation

just going for it made clean code until I started calculating the checksum. With time-pressure and panic-coding got me working, bad lookin, unmaintanable shit code.

```c

uint8_t calculate_checksum(char *checksum_calc, char *character_part_input) {

    // First we just map all the characters used, and how many times they occur
    memset(checksum_calc, 0, 6 * sizeof(char)); // checksum_calc = {0};
    char c = 0;
    char checksum_calc_big[30] = {0};
    uint8_t char_occurences[30] = {0};
    printf("%s\n", character_part_input);
    uint8_t i_in = 0;
    uint8_t i_big_setter = 0;
    uint8_t already_there = 0;
    while (character_part_input[i_in] != 0) {
        already_there = 0;
        c = character_part_input[i_in];
        // printf("%c\n", character_part_input[i_in]);
        for (uint8_t i_char_out = 0; i_char_out < 30; i_char_out++) {
            if (checksum_calc_big[i_char_out] == c) {
                char_occurences[i_char_out] += 1;
                already_there = 1;
            }
        }
        if (!already_there) {
            checksum_calc_big[i_big_setter++] = c;
        }
        i_in++;
    }

    printf("%s\n", checksum_calc_big);
    printf("%s\n", char_occurences); // this print doesnt work yet, array of uint8s

    // Then we sort
    return 0;
}

```

very messy... working but too ugly, to many indexes and seperate arrays

### new idea for calculating the checksum

inspired by Eriks dungeon crawler

```c
typedef struct {
    char c;
    uint8 occurence;
} CharOccur;

```

just because cleaner code and then we can make a sorting function for quicksort

```c
CharOccur character_sort(CharOccur a, CharOccur b){
    if(a.occurence > b.occurence) return a;
    if(b.occurence > a.occurence) return b;
    if(a.char > b.char) return b; // this might be other way round
    else return a;
} // we will fine tune this later. both >< and function pointer

```

ok, so im thinking also take another page out of Eriks book and abstract the shit out of this

```c
uint8_t is_in_array(char charTested, uint8_t length_of_array_nmb, CharOccur * charOccurArray){
    // no testing for out of bounds, happycoding
    for (uint8_t i = 0; i < length_of_array_nmb; i++){
        if (charTested == charOccurArray[i]->c) return 1;
    }
    return 0;
} // would have been better to just return the index straight away, not changing, happycoding

```

```c

uint8_t index_of_char(char charTested, uint8_t length_of_array_nmb, CharOccur * charOccurArray){
    for (uint8_t i = 0; i < length_of_array_nmb, i++){
        if ( charTested == charOccurArray[i]->c ){
            return i;
        }
    }
    printf("index was not found, should be impossible, no errorhandling for this fcn though\n"); // happy-coding
    return 0;
}

```

```c 
void print_occur(uint8_t length_of_array_nmb, CharOccur * charOccurArray){
    for(uint8_t i = 0; i < length_of_array_nmb, i++){
        printf("character: %c | %" PRIu8 "\n", )
    }
}

```

And so general structure one array and and one index/number for that array.

yeah this turned into real-ish code, not pseudocode.

```c
// macro
#define ALPHABET_LENGTH 30 

uint8_t calculate_checksum(char *checksum_calc, char *character_part_input) {

    // First we just map all the characters used, and how many times they occur
    memset(checksum_calc, 0, 6 * sizeof(char)); // checksum_calc = {0};
    char c = 0;
    CharOccur checksum_calc_big[ALPHABET_LENGTH] = {0}; // Array of characters found

    // printf("%s\n", character_part_input);
    uint8_t i_in = 0; // index of read characters from input
    uint8_t i_big_setter = 0; //index of stored CharOccur 
    while (character_part_input[i_in] != 0) {
        c = character_part_input[i_in];
        // printf("%c\n", character_part_input[i_in]);
        if (is_in_array(c, i_big_setter + 1, checksum_calc_big)){ // big +1 ind->nmb???? i think no + 1 actually
            checksum_calc_big[index_of_char(c)].occurence += 1;
        }
        else{
            checksum_calc_big[i_big_setter].c = c;
            checksum_calc_big[i_big_setter++].occurence = 1;
        }
        i_in++;
    }

    print_occur(i_big_setter, checksum_calc_big); // this time also i dont think we need +1 ind->nmb

    // Then we sort
    return 0;
}
```

### implementation of new design

So some typos but it actually worked with the indexes above
