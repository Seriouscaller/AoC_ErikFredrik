#include <stdio.h>
#include <stdlib.h>
#include "input_output.h"

#define PRINT_DEBUG 1

#define ARR_SIZE 5000
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day02/day02_input_sample.txt"
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day02/day02_input_full.txt"

int main(){

    long size = get_file_size(INPUT_FILE_NAME) + 1;
    
    char* input =  allocate(size);
    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        abort();
    }else {
        printf("File read succesfully!\n");
    }

    #ifdef PRINT_DEBUG 
        printf("Filesize: %lu\n", size);
        printf("%s", input);
    #endif
    
    free_allocation(input);
    return EXIT_SUCCESS;
}