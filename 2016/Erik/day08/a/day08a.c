#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day08/a/day08_input_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day08/a/day08_input_full.txt"
#define NEWLINE '\n'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'
#define RESOLUTION_WIDTH 50
#define RESOLUTION_HEIGHT 6
#define MAX_LINE_LENGTH 100
#define NUMBER_OF_RECT_DIMENSIONS 2

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
    uint16_t origin;
    uint16_t magnitude;
    uint16_t dimensions[NUMBER_OF_RECT_DIMENSIONS];
};

/*  PROTOTYPES  */
bool is_numeric(char* str, uint8_t length);
void print_arguments_from_instructions(struct instruction* instructions, uint16_t length);
void print_instruction_array(struct instruction* instructions, uint16_t length);
bool extract_rect_dimensions(struct instruction* instruction);
bool extract_origin(struct instruction* instruction);
bool extract_magnitude(struct instruction* instruction);
bool parse_instruction(struct instruction* instruction);
void display_all_pixels(uint16_t height, uint16_t width, uint8_t display_matrix[height][width]);
void init_pixel_array(uint16_t width, uint8_t* pixel_row);
uint32_t count_pixels(uint16_t height, uint16_t width, uint8_t display_matrix[width][height]);
void copy_pixels_onto_display(uint16_t height, uint16_t width, uint8_t display_matrix[height][width], uint16_t row_index, uint8_t* pixel_array, uint8_t type_of_shift);
bool shift_pixels(uint16_t height, uint16_t width, uint8_t display_matrix[height][width], uint16_t index_origin, uint16_t magnitude, uint8_t type_of_shift);

bool is_numeric(char* str, uint8_t length){
    bool is_numeric = true;
    for(int i = 0; i < length; i++){
        if(!isdigit(str[i])){
            is_numeric = false;
        }
    }
    return is_numeric;
}

void print_arguments_from_instructions(struct instruction* instructions, uint16_t length){
    printf("## Arguments ##\n");
    for(int i = 0; i < length; i++){
        printf("[%d] Argc: %u ", i, instructions[i].argc);
        for(int j = 0; j < instructions[i].argc;j++){
            printf("Arg[%d]: %s  ", j, instructions[i].argv[j]);
        }
        printf("%c", NEWLINE);
    }
};

void print_instruction(struct instruction* instruction){
    printf("Type: ");
    switch (instruction->operation)
    {
    case RECT:
        printf("REKT %ux%u%c", instruction->dimensions[0], instruction->dimensions[1], NEWLINE);
        break;
    case ROTATE_ROW:
        printf("ROTATE_ROW Origin: %u Magnitude: %u%c", instruction->origin, instruction->magnitude, NEWLINE);
        break;    
    case ROTATE_COLUMN:
        printf("ROTATE_COLUMN Origin: %u Magnitude: %u%c", instruction->origin, instruction->magnitude, NEWLINE);
        break;    
    default:
        printf("NO-TYPE%c", NEWLINE);
        break;
    }


}

void print_instruction_array(struct instruction* instructions, uint16_t length){
    printf("## Instructions Array ##\n");
    for(int i = 0; i < length; i++){
        if(0 >= instructions[i].argc){continue;}

        if(instructions[i].operation == RECT){
            printf("REKT width: %u height: %u ", instructions[i].dimensions[0], instructions[i].dimensions[1]);
            printf("%c", NEWLINE);
            continue;
        }

        if(instructions[i].operation == ROTATE_COLUMN){
            printf("ROTATE COLUMN ");
        }
        if(instructions[i].operation == ROTATE_ROW){
            printf("ROTATE ROW ");
        }

        printf("Origin: %u Magnitude: %u", instructions[i].origin, instructions[i].magnitude);
        printf("%c", NEWLINE);
    }
};

bool extract_rect_dimensions(struct instruction* instruction){
    // Extract x and y size of rect
    uint8_t index_number = 0;
    char* token = strtok(instruction->argv[1], "x");
    if(token == NULL) return false;

    while(token != NULL){
        if(!is_numeric(token, strlen(token))) return false;
        int number = atoi(token);

        if(0 >= number || UINT16_MAX < number) return false;
        if(NUMBER_OF_RECT_DIMENSIONS <= index_number) return false;
        instruction->dimensions[index_number] = number;
        index_number++;
        token = strtok(NULL, "x");
    }
    return true;
}

bool extract_origin(struct instruction* instruction){
    char* token = strtok(instruction->argv[2], "=");
    while(token != NULL){
        int number = 0;

        if(!is_numeric(token, strlen(token))){
            token = strtok(NULL, "=");
            continue;
        }
        number = atoi(token);

        if(0 >= number || UINT16_MAX < number) return false;
        instruction->origin = number;
        return true;
    }
    return false;
}

bool extract_magnitude(struct instruction* instruction){
    if(is_numeric(instruction->argv[4], strlen(instruction->argv[4]))){
        int number = atoi(instruction->argv[4]);
        if(number <= 0 || UINT16_MAX < number) return false;
        
        instruction->magnitude = number;
        return true;
    }
    return false;
}

bool parse_instruction(struct instruction* instruction){
    // Each instruction has only one branch it can take
    // rotate -> column or row, 
    // rect

    int res = strcmp(instruction->argv[0], "rotate");
    if((instruction->argc == 5) && (res == 0)){

        res = strcmp(instruction->argv[1], "column");
        if(res == 0){
            instruction->operation = ROTATE_COLUMN;
            // Get origin origin & magnitude
            bool success_origin = extract_origin(instruction);
            bool success_magnitude = extract_magnitude(instruction);
            return (success_origin && success_magnitude);
        }

        res = strcmp(instruction->argv[1], "row");
        if(res == 0){
            instruction->operation = ROTATE_ROW;
            bool success_origin = extract_origin(instruction);
            bool success_magnitude = extract_magnitude(instruction);
            return (success_origin && success_magnitude);
        }
    }

    res = strcmp(instruction->argv[0], "rect");
    if((instruction->argc == 2) && (res == 0)){
        return (extract_rect_dimensions(instruction));
    }

    return false;
};

void display_all_pixels(uint16_t height, uint16_t width, uint8_t display_matrix[height][width]){
    printf("--- PIXELDISPLAY ---%c", NEWLINE);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(display_matrix[i][j] == ON){
              printf("#");  
            } else{
                printf(".");
            }
        }
        printf("%c", NEWLINE);
    }
};

void init_pixel_array(uint16_t width, uint8_t* pixel_row){
    for(int j = 0; j < width; j++){
        pixel_row[j] = '\0';
    }
}

uint32_t count_pixels(uint16_t height, uint16_t width, uint8_t display_matrix[width][height]){
    uint32_t number_of_lit_pixels = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(display_matrix[j][i] == ON){
              number_of_lit_pixels++;
            } 
        }
    }
    return number_of_lit_pixels;
}

void copy_pixels_onto_display(uint16_t height, uint16_t width, uint8_t display_matrix[height][width], uint16_t row_index, uint8_t* pixel_array, uint8_t type_of_shift){
    
    uint16_t length = 0;
    if(type_of_shift == ROTATE_ROW){
        length = width;
    }else{
        length = height;
    }

    for(int i = 0; i < length; i++){
        if(type_of_shift == ROTATE_ROW){
            display_matrix[row_index][i] = pixel_array[i];
        }else{
            display_matrix[i][row_index] = pixel_array[i];
        }
    }
}

bool shift_pixels(uint16_t height, uint16_t width, uint8_t display_matrix[height][width], uint16_t index_origin, uint16_t magnitude, uint8_t type_of_shift){
    
    if((type_of_shift == ROTATE_ROW) && ((height - 1) < index_origin)){
        printf("Invalid parameters for shifting the row!");
        return false;
    }else if((type_of_shift == ROTATE_COLUMN) && ((width - 1) < index_origin)){
        printf("Invalid parameters for shifting the column!");
        return false;
    }

    if(UINT16_MAX < magnitude){
        printf("Magnitude larger than UINT16_MAX!\n");
        return false;
    }

    uint8_t pixel_value;
    uint8_t pixel_array[width]; 
    uint16_t length = 0;
    if(type_of_shift == ROTATE_ROW){
        length = width;
    }else{
        length = height;
    }

    init_pixel_array(length, pixel_array); 

    for(int i = 0; i < length; i++){
        if(type_of_shift == ROTATE_ROW){
            pixel_value = display_matrix[index_origin][i];
        }else{
            pixel_value = display_matrix[i][index_origin];
        }
        uint16_t shifted_index = (i + magnitude) % length;
        pixel_array[shifted_index] = pixel_value;
    }

    copy_pixels_onto_display(height, width, display_matrix, index_origin, pixel_array, type_of_shift); 
    return true;
};

bool rect(uint16_t height, uint16_t width, uint8_t display_matrix[height][width], uint16_t size_x, uint16_t size_y){
    if((width < size_x) || (height < size_y) || (0 >= size_x) || (0 >= size_y)){
        return false;
    }
    
    for(int i = 0; i < size_y; i++){
        for(int j = 0; j < size_x; j++){
            display_matrix[i][j] = ON;
        }
    };
    return true;
};

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

    uint8_t display_matrix[RESOLUTION_HEIGHT][RESOLUTION_WIDTH] = {'\0'};

    char lines[file_info.rows][MAX_LINE_LENGTH];
    memset(lines, '\0', file_info.rows * MAX_LINE_LENGTH * sizeof(char));

    uint8_t result = split_puzzle_into_rows(file_info.rows, MAX_LINE_LENGTH, lines, puzzle_string, &file_info);
    if(result == ERROR){
        printf("Failed to split puzzle into rows!\n");
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }

    struct instruction* instructions = calloc(file_info.rows, sizeof(struct instruction));
    if(instructions == NULL){
        printf("Memory Allocation failed!");
        free_allocation(instructions);
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }

    // Creating instructions
    for(uint16_t i = 0; i < file_info.rows; i++){
        struct instruction instruction = {'\0'};

        char* token = strtok(lines[i], " ");
        while(token != NULL){
            printf("%s*", token);
            strncpy(instruction.argv[instruction.argc], token, strlen(token));
            instruction.argc++;
            token = strtok(NULL, " ");
        }
        instructions[i] = instruction;
        printf("%c", NEWLINE);
    }

    print_arguments_from_instructions(instructions, file_info.rows);

    // Extract further information from instruction
    // Like width, height, direction, magnitude etc.
    for(int j = 0; j < file_info.rows; j++){
        if(0 < instructions[j].argc){
            parse_instruction(&instructions[j]);
        }
    }
    print_instruction_array(instructions, file_info.rows);

    for(int k = 0; k < file_info.rows; k++){
        printf("k:%d ", k);
        if(0 < instructions[k].argc){
            print_instruction(&instructions[k]);
            switch (instructions[k].operation)
            {
            case RECT:
                if(!rect(RESOLUTION_HEIGHT, RESOLUTION_WIDTH, display_matrix, instructions[k].dimensions[0], instructions[k].dimensions[1])){
                    printf("Turning on pixels failed!%c", NEWLINE);
                }
                break;
            case ROTATE_ROW:
                if(!shift_pixels(RESOLUTION_HEIGHT, RESOLUTION_WIDTH, display_matrix, instructions[k].origin, instructions[k].magnitude, ROTATE_ROW)){
                    printf("Failed to shift pixel-row!%c", NEWLINE);
                }
                break;
            case ROTATE_COLUMN:
                if(!shift_pixels(RESOLUTION_HEIGHT, RESOLUTION_WIDTH, display_matrix, instructions[k].origin, instructions[k].magnitude, ROTATE_COLUMN)){
                    printf("Failed to shift pixel-column!%c", NEWLINE);
                }
                break;
            default:
                printf("Invalid operation!%c", NEWLINE);
                break;
            };
        }else{
            printf("Too few arguments in instruction!%c", NEWLINE);
        }
        display_all_pixels(RESOLUTION_HEIGHT, RESOLUTION_WIDTH, display_matrix);
    }
    
    uint32_t number_of_lit_pixels = count_pixels(RESOLUTION_HEIGHT, RESOLUTION_WIDTH, display_matrix);

    printf("Number of lit pixels: %lu\n", number_of_lit_pixels);

    free_allocation(instructions);
    free_allocation(puzzle_string);
    return 0;
}

/*

###..#..#.###..#..#..##..####..##..####..###.#....
#..#.#..#.#..#.#..#.#..#.#....#..#.#......#..#....
#..#.#..#.#..#.#..#.#....###..#..#.###....#..#....
###..#..#.###..#..#.#....#....#..#.#......#..#....
#.#..#..#.#.#..#..#.#..#.#....#..#.#......#..#....
#..#..##..#..#..##...##..####..##..####..###.####.
Number of lit pixels: 121

Day08B: RURUCEOEIL
*/