#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day09/a/day09_input_sample.txt"
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day09/a/day09_input_full.txt"
#define NEWLINE '\n'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'
#define MAX_LINE_LENGTH 3000

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

struct marker {
    char argv[3][10];
    uint8_t argc;
    uint16_t length;
    uint16_t number_of_repeats;
};

/*  PROTOTYPES  */

int main(int argc, char *argv[]){
    struct file_information file_info = get_file_information(INPUT_FILE_NAME);
    char* puzzle_string =  allocate(file_info.file_size);

    if(file_info.rows == 0 || file_info.file_size == 0){
        printf("Unable to acquire file information!");
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }

    if(file_info.max_columns > MAX_LINE_LENGTH){
        printf("Puzzleline too long! Unable to fit inside string-array!");
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }

    int ret = read_file(INPUT_FILE_NAME, puzzle_string);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }else {
        printf("File read succesfully! Size: %lu Rows: %lu Columns: %lu\n", file_info.file_size, file_info.rows, file_info.max_columns);
    }

    /*### MAIN LOOP ###*/
    uint32_t length_decrypted_data = 0;
    uint64_t global_index = 0;
    char ch;
    while(global_index < file_info.file_size){
        ch = puzzle_string[global_index];

        if(ch == '('){
            ; // Start validating marker
        }

        parse_character(&puzzle_string[global_index]);


        global_index++;
    }

    free_allocation(puzzle_string);
    return 0;
}
/*
A(1x5)BC
ABBBBBC

(3x3)XYZ
XYZXYZXYZ

A(2x2)BCD(2x2)EFG
ABCBCDEFEFG

(6x1)(1x3)A
(1x3)A

X(8x2)(3x3)ABCY
X(3x3)ABC(3x3)ABCY

#### Eriks PLAN ####
(MxN)
uint32_t length_decrypted_data
Keep counting (no whitespace) the length parsed so far of decrypted data

IF character is '('
    EXTRACT MARKER
    Found valid marker!
    ELSE keep counting

Parse marker
    Select as many characters (M) as the marker says
    Duplicate the string (N) number of times
    add string length to length_decrypted_data

## Functions
 - Function to dynamically allocate memory for the decrypted string
 - Is Valid Marker?
    Check if it has a closing parenthesis, an 'x', and the rest is numerical
 - 
*/