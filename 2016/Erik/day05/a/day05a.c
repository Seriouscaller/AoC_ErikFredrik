#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include "md5.h"

char* puzzle_input = "uqwqemis";

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

void unsigned_number_to_string(uint64_t number, char* string);
void extract_bitpattern_from_hash(uint8_t* result, uint32_t* first_five_digits_hash);
void extract_password_char_from_hash(uint8_t* result, char* password);

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

void extract_bitpattern_from_hash(uint8_t* result, uint32_t* first_five_digits_hash){
    *first_five_digits_hash = (result[0] << 4) | (result[1] << 2) | (result[2] & 0b11110000);
}

void extract_password_char_from_hash(uint8_t* result, char* password){
    char string[4] = {'\0'};
    *password = result[2] & 0b00001111;
    sprintf(string, "%x", result[2] & 0b00001111);
    printf("Extracted char: %c \n", string[0]);
}

int main(int argc, char *argv[]){

    char input[30] = {'\0'};
    char password[20] = {'\0'};
    
    uint64_t counter = 0;

    uint8_t password_characters_found = 0;
    char puzzle_input[50] = "uqwqemis";
    while(password_characters_found < 8){
        strcpy(input, puzzle_input);    // Reset the puzzleinput each hash

        char counter_string[20] = {'\0'};
        unsigned_number_to_string(counter, counter_string);

        strcat(input, counter_string);
        uint8_t result[16];
        uint32_t first_five_digits_hash = UINT32_MAX;
        
        md5String(input, result);
        extract_bitpattern_from_hash(result, &first_five_digits_hash);

        if(first_five_digits_hash == 0){
            // We found a valid hash
            print_hash(result);
            extract_password_char_from_hash(result, password);
            password_characters_found++;
        };
        counter++;
    }
    return 0;
}
