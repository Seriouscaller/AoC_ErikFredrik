#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdint.h>
#include "md5.h"
#include <stdbool.h>

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

void unsigned_number_to_string(uint64_t number, char* string);
void print_hash(uint8_t *p);
void extract_bitpattern_from_hash(uint8_t* result, uint32_t* first_five_digits_hash);
char hex_digit_to_char(uint8_t hex_digit);
bool is_valid_hash(uint32_t* first_five_digits_hash);

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

char hex_digit_to_char(uint8_t hex_digit){
    if(hex_digit < 10){
        return (hex_digit + '0');
    }

    switch (hex_digit)
    {
    case 10:
        return 'a';
        break;
    case 11:
        return 'b';
        break;
    case 12:
        return 'c';
        break;
    case 13:
        return 'd';
        break;
    case 14:
        return 'e';
        break;
    case 15:
        return 'f';
        break;
    default:
        printf("Error converting hex-digit to character! Value: %02x\n", hex_digit);
        return 'X';
        break;
    }
}

bool is_valid_hash(uint32_t* first_five_digits_hash){
    if(*first_five_digits_hash == 0){
        return true;
    }else{
        return false;
    }
};

int main(int argc, char *argv[]){
    char input[50] = {'\0'};
    char puzzle_input[9] = "uqwqemis";
    char password[9] = {'\0'};
    
    uint64_t counter = 0;
    uint8_t password_characters_found = 0;
    while(password_characters_found < 8){
        strcpy(input, puzzle_input);

        char counter_string[20] = {'\0'};
        unsigned_number_to_string(counter, counter_string);
        counter++;

        strcat(input, counter_string);
        uint8_t result[16];
        uint32_t first_five_digits_hash = UINT32_MAX;
        
        md5String(input, result);
        extract_bitpattern_from_hash(result, &first_five_digits_hash);

        // Continue searching if hash is not starting with '00000'
        if(!is_valid_hash(&first_five_digits_hash)){
            continue;
        }
        
        print_hash(result);
        uint8_t pass_index = (result[2] & 0b00001111);
        uint8_t pass_digit = result[3] >> 4;
        printf("Passindex: %d Passchar: %d\n", pass_index, pass_digit);

        if(!(pass_index < 8)){
            // Check for invalid indexes
            continue;
        }

        char pass_char = hex_digit_to_char(pass_digit);

        if(password[pass_index] == '\0'){
            // New password letter found! Adding it to password-string
            password[pass_index] = pass_char;
            password_characters_found++;
        }
    }
    printf("Final Password: %s", password);
    return 0;
}

// Final password: 694190cd