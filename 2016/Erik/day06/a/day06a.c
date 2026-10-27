#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day06/a/day06_input_sample.txt"
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day06/a/day06_input_full.txt"

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

void split_puzzle_input_into_array(uint16_t rows, uint16_t columns, char array[rows][columns]);

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

void split_puzzle_input_into_array(uint16_t rows, uint16_t columns, char array[rows][columns]){
    ;
};

int main(int argc, char *argv[]){
    struct file_information file_info = get_file_information(INPUT_FILE_NAME);
    char* input =  allocate(file_info.file_size);

    if(file_info.rows == 0 || file_info.file_size == 0){
        printf("Unable to acquire file information!");
        free_allocation(input);
        abort();
    }

    char array[file_info.rows][file_info.columns];
    memset(array, 0, file_info.rows * file_info.columns * sizeof(array[0][0]));
    split_puzzle_input_into_array(file_info.rows, file_info.columns, array);

    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        free_allocation(input);
        abort();
    }else {
        printf("File read succesfully! Size: %lu Rows: %lu Columns: %lu\n", file_info.file_size, file_info.rows, file_info.columns);
    }



    free_allocation(input);
    return 0;
}
