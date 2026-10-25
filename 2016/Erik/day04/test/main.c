#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define NUMBER_OF_LETTERS_IN_ALPHABET 26
#define ASCII_OFFSET 97
#define DASH '-'
#define WHITE_SPACE ' '

const char* alphabet = "abcdefghijklmnopqrstuvwxyz";
const char* search_string_1 = "north";
const char* search_string_2 = "pole";

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

char decrypt_room();
uint8_t get_offset_from_sector_id(uint16_t sector_id);
int8_t find_letter_index_in_alphabet(char c);

char decrypt_room(){
    int sector = 1;
    char* string = "qzmt-zixmtkozy-ivhz-";
    int string_length = strlen(string);
    char string_cpy[string_length];
    strncpy(string_cpy, string, string_length);

    int shift = get_offset_from_sector_id(sector);
    for(int i = 0; i < strlen(string); i++){
        char new_char = '\0';
        if(string[i] == DASH){
            new_char = WHITE_SPACE;
        }else {
            int8_t char_index = find_letter_index_in_alphabet(string[i]);
            int8_t transformed_index = (char_index + shift) % NUMBER_OF_LETTERS_IN_ALPHABET;
            new_char = alphabet[transformed_index];
        }
        printf("Old_char: %c New_char: %c \n", string[i], new_char);
        string_cpy[i] = new_char;
    };

    printf("Old String: %s\nNew String: %s\n", string, string_cpy);
}

uint8_t get_offset_from_sector_id(uint16_t sector_id){
    return sector_id % NUMBER_OF_LETTERS_IN_ALPHABET;
}

int8_t find_letter_index_in_alphabet(char c){
    int8_t letter_index;
    for(letter_index = 0; letter_index < NUMBER_OF_LETTERS_IN_ALPHABET; letter_index++){
        if(c == alphabet[letter_index]){
            return letter_index;
        }
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    //decrypt_room();
    return 0;
}
