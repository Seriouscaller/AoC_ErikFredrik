#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keypad.h"

#define ARR_SIZE 5000
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day02_input_sample.txt"
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day02_input_full.txt"

int determine_input_size(char* file_name){
    FILE *fp = fopen(file_name, "r");
    
    if(fp == NULL) {
        return 1;
    }
    int ret = fseek(fp, sizeof(char), SEEK_END);
    return ret;
}

int read_file(char* file_name, char* array){
    FILE *fp = fopen(file_name, "r");
    
    if(fp == NULL) {
        return 1;
    }

    int i = 0; int ch;
    while ((ch = fgetc(fp)) != EOF){
        array[i++] = ch;
    }
    array[++i] = '\0';
    fclose(fp);

    return 0;
}

int main(){

    int size = determine_input_size(INPUT_FILE_NAME);
    printf("SIZE: %d\n", size);

    char input[ARR_SIZE];

    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        abort();
    }else {
        printf("File read succesfully!\n");
    }

    int digit = 5;

    int input_size = strlen(input);
    for(int i = 0; i < input_size; i++){

        if(input[i] != '\n'){
            digit = move(input[i], digit);
        }else{
            printf("Button End-of-Line: %d \n", digit);
        }
    }

    return 0;
}