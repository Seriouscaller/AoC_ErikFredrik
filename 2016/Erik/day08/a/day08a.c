#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day08/a/day08_input_sample.txt"
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day08/a/day08_input_full.txt"
#define NEWLINE '\n'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'
#define RESOLUTION_WIDTH 10
#define RESOLUTION_HEIGHT 4
#define MAX_LINE_LENGTH 100

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

enum OPERATION {
    RECT = 0,
    ROTATE_COLUMN = 1,
    ROTATE_ROW = 2,
};

enum PIXEL_STATUS {
    OFF = 0,
    ON = 1,
};

struct instruction {
    char argv[5][10];
    uint8_t argc;
    uint8_t operation;
    uint16_t index;
    uint16_t magnitude;
    uint16_t width;
    uint16_t height;
};

/*  PROTOTYPES  */

int main(int argc, char *argv[]){
    struct file_information file_info = get_file_information(INPUT_FILE_NAME);
    char* puzzle_string =  allocate(file_info.file_size);

    if(file_info.rows == 0 || file_info.file_size == 0){
        printf("Unable to acquire file information!");
        free_allocation(puzzle_string);
        abort();
    }

    if(file_info.max_columns > MAX_LINE_LENGTH){
        printf("Puzzleline too long! Unable to fit inside string-array!");
        free_allocation(puzzle_string);
        abort();
    }

    int ret = read_file(INPUT_FILE_NAME, puzzle_string);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        free_allocation(puzzle_string);
        abort();
    }else {
        printf("File read succesfully! Size: %lu Rows: %lu Columns: %lu\n", file_info.file_size, file_info.rows, file_info.max_columns);
    }

    uint8_t display_matrix[RESOLUTION_WIDTH][RESOLUTION_HEIGHT] = {'\0'};

    char lines[file_info.rows][MAX_LINE_LENGTH];
    memset(lines, '\0', file_info.rows * MAX_LINE_LENGTH * sizeof(char));

    uint8_t result = split_puzzle_into_rows(file_info.rows, MAX_LINE_LENGTH, lines, puzzle_string, &file_info);
    if(result == ERROR){
        printf("Failed to split puzzle into rows!\n");
        free_allocation(puzzle_string);
        abort();
    }

    // Outer Loop -> Parsing each instruction
    for(uint16_t i = 0; i < file_info.rows; i++){
        struct instruction instruction = {'\0'};

        char* token = strtok(lines[i], " ");
        while(token != NULL){
            printf("%s*", token);
            strncpy(instruction.argv[instruction.argc], token, strlen(token));
            instruction.argc++;
            token = strtok(NULL, " ");
        }
        printf("%c", NEWLINE);
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
        Check the number of arguments to make sure the 
            if rect, new delimiter x
                separate values into width and height
            if rotate
                determine direction
                extract index and magnitude
    
    Functionality:
    - Function to display all pixels of display
    - Function to shift array using parameters. rotate(Type=COLUMN, ORIGIN=3, MAGNITUDE=2); create_rekt(width=3 height=1);
    - Function to count lit pixels
    */

    free_allocation(puzzle_string);
    return 0;
}
