#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#define NUMBER_OF_LETTERS_IN_THE_ALPHABET 26
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day08/a/day08_input_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day08/a/day08_input_full.txt"
#define NEWLINE '\n'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

int main(int argc, char *argv[]){
    struct file_information file_info = get_file_information(INPUT_FILE_NAME);
    char* input =  allocate(file_info.file_size);

    if(file_info.rows == 0 || file_info.file_size == 0){
        printf("Unable to acquire file information!");
        free_allocation(input);
        abort();
    }

    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        free_allocation(input);
        abort();
    }else {
        printf("File read succesfully! Size: %lu Rows: %lu Columns: %lu\n", file_info.file_size, file_info.rows, file_info.max_columns);
    }

    /****** ERIKS PLAN ********
     * Define X and Y display resolution
     * 
    - Parse one line at a time
    - LINE
        - Separate content of line using WHITESPACE delimiter
        - Create a struct: 
            (rotate column x=1 by 2)
            Struct instruction {
                char operation;     // Enum: RECT, ROTATE
                char direction;     // Enum: Column using x-val, Row using y-val
                uint16_t index;     // =1
                uint16 magnitude;   // by 2
                uint16 width;       // rect x value
                uint16 height;      // rect y value
            }
        first word determines operation
            if rect, new delimiter x
                separate values into width and height
            if rotate
                determine direction
                extract index and magnitude
    
    Functionality:
    - Function to display all pixels of display
    - Function to shift array using paramenters. rotate(Type=COLUMN, ORIGIN=3, MAGNITUDE=2); create_rekt(width=3 height=1);
    - Function to count lit pixels
    */

    free_allocation(input);
    return 0;
}
