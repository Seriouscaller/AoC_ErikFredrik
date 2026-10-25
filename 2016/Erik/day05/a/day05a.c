#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include "md5.h"

#define NEWLINE '\n'
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'
#define DASH '-'

char* puzzle_input = "uqwqemis";
char* search_pattern = "00000";

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

void unsigned_number_to_string(uint64_t number, char* string);
bool is_pattern_found(uint8_t* string);

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

void print_hash(uint8_t *p){
    for(unsigned int i = 0; i < 16; ++i){
        printf("%02x", p[i]);
    }
    printf("\n");
}

bool is_pattern_found(uint8_t* string){
    int index = 0;
    while(string[index] == '\0'){
        index++;
    }

    if(5 <= index){
        return true;
    }else{
        return false;
    }
}

int main(int argc, char *argv[]){

    char input[30] = {'\0'};
    
    //uint64_t counter = 0;
    uint64_t counter = 3231929;

    uint8_t remaining_password_characters = 8;
    //char puzzle_input[50] = "uqwqemis";
    char puzzle_input[50] = "abc";
    while(remaining_password_characters <= 8){
        strcpy(input, puzzle_input);    // Reset the puzzleinput each hash

        char counter_string[20] = {'\0'};
        unsigned_number_to_string(counter, counter_string);

        strcat(input, counter_string);
        //printf("Input: %s Counter: %s Cat: %s\n", puzzle_input, counter_string, input);
        uint8_t result[16];
        uint8_t bitpattern[5];
        
        md5String("abc5017308", result);
        //md5String(input, result);
        
        bitpattern[0] = result[0];
        bitpattern[1] = result[1];
        bitpattern[2] = result[2] & 0b11110000;
        
        printf("%d\n", bitpattern[2]);

        if(is_pattern_found(result)){
            printf("Hash: %s\n", result);
            printf("Pass char: %c\n", result[6]);
            remaining_password_characters--;
        }

        print_hash(result);
        
        counter++;
    }

    return 0;
}

/*
00 00 08 f8 2c 5b 39 24 a1 ec be bf 60 34 4e 00
*/