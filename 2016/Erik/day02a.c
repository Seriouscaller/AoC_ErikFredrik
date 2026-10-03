#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARR_SIZE 5000

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

int move(char c, int digit){
    switch (c) {
        case 'U':
            if((3 < digit)){
                digit -= 3;
            }
            break;
        case 'D':
            if((7 > digit)){
                digit += 3;
            }
            break;
        case 'R':
            if(!(digit % 3 == 0)){
                digit++;
            }
            break;
        case 'L':
            if(!(digit % 3 == 1)){
                digit--;
            }
        break;
    };

    return digit;
}

int main(){

    char file_name[] = "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day02_input_full.txt";
    char input[ARR_SIZE];

    int ret = read_file(file_name, input);
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