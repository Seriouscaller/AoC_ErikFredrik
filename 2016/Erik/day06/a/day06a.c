#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day06/a/day06_input_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day06/a/day06_input_full.txt"
#define NEWLINE '\n'
#define NUMBER_OF_LETTERS_IN_THE_ALPHABET 26

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

char* alphabet = "abcdefghijklmnopqrstuvwxyz";

void unsigned_number_to_string(uint64_t number, char* string);
void split_puzzle_input_into_array(uint16_t rows, uint16_t columns, char array[rows][columns], char* input, struct file_information* file_info);
void parse_array_column(long rows, long columns, char array[rows][columns], uint16_t* occurances, uint16_t occurances_size);
void print_array(uint16_t rows, uint16_t columns, char array[rows][columns]);
char find_most_occurring_letter(uint16_t* occurances, uint16_t length);

void unsigned_number_to_string(uint64_t number, char* string){

    if(number == 0){
        string[0] = '0';
        return;
    }

    int i = 0;
    while(number > 0){
        string[i++] = number % 10 + '0';
        number /= 10;
    }

    for(int j = 0,  k = i - 1; j < k; j++, k--){
        char temp = string[j];
        string[j] = string[k];
        string[k] = temp;
    }
}

void split_puzzle_input_into_array(uint16_t rows, uint16_t columns, char array[rows][columns], char* input, struct file_information* file_info){
    int input_array_index = 0;
    for(int row = 0; row < file_info->rows; row++){
        for(int column = 0; column < file_info->columns; column++){
            if(input[input_array_index] == NEWLINE){
                input_array_index++;
            }
            array[row][column] = input[input_array_index++];
        }
    };
};

void print_array(uint16_t rows, uint16_t columns, char array[rows][columns]){
    for(int row = 0; row < rows; row++){
        for (int column = 0; column < columns; column++)
        {
            printf("%c", array[row][column]);
            if(column >= columns -1){
                printf("%c", NEWLINE);
            }
        }
    }
}

void parse_array_column(long rows, long columns, char array[rows][columns], uint16_t* occurances, uint16_t occurances_size){
    for(int i = 0; i < columns; i++){
        for(int j = 0; j < rows; j++){
            char ch = array[j][i];
            if(ch >= 'a' && ch <= 'z'){
                // count letter
                uint8_t letter_index = ch - 'a';    // Remove offset. E.g. a = 0, b = 1
                occurances[letter_index]++;
                //printf("%c:%u%c", ch, occurances[letter_index], NEWLINE);
            }
        }
        char most_occurring_letter = find_most_occurring_letter(occurances, occurances_size);
        printf("Col[%u]L: %c%c%c", i, most_occurring_letter, NEWLINE, NEWLINE);
        memset(occurances, 0, NUMBER_OF_LETTERS_IN_THE_ALPHABET * sizeof(occurances[0]));    // Reset count before next column
    };
};

char find_most_occurring_letter(uint16_t* occurances, uint16_t length){
    uint16_t max = 0;
    char most_occuring_letter = '\0';
    for(uint8_t index = 0; index < length; index++){
        if(occurances[index] > max){
            max = occurances[index];
            most_occuring_letter = alphabet[index];
        }
    }
    return most_occuring_letter;
}

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
        printf("File read succesfully! Size: %lu Rows: %lu Columns: %lu\n", file_info.file_size, file_info.rows, file_info.columns);
    }

    char array[file_info.rows][file_info.columns];
    memset(array, 0, file_info.rows * file_info.columns * sizeof(array[0][0]));

    split_puzzle_input_into_array(file_info.rows, file_info.columns,  array, input, &file_info);
    //print_array(file_info.rows, file_info.columns, array);

    uint16_t letter_occurances[NUMBER_OF_LETTERS_IN_THE_ALPHABET] = {'\0'};
    uint16_t letter_occurances_size = sizeof(letter_occurances) / sizeof(letter_occurances[0]);
    parse_array_column(file_info.rows, file_info.columns, array, letter_occurances, letter_occurances_size);


    free_allocation(input);
    return 0;
}
