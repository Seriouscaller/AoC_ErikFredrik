#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day04/a/puzzle_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day04/a/puzzle_full.txt"
#define CHECKSUM_LENGTH 5
#define CHECKSUM_STRING_LENGTH (CHECKSUM_LENGTH + 1)
#define LETTERS_IN_ALPHABET 26
#define NEWLINE '\n'
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'
#define DASH '-'

const char* alphabet = "abcdefghijklmnopqrstuvwxyz";

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

struct encrypted_data {
    char checksum[CHECKSUM_STRING_LENGTH];
    char room_data[CHECKSUM_STRING_LENGTH];
    int sector_id;
    uint8_t letters[LETTERS_IN_ALPHABET];
    int number_of_letters_stored;
    uint8_t max_value;
};

void init_encrypted_data(struct encrypted_data* data);
void split_puzzle_into_lines(char* input, char puzzle_strings[1200][80]);
void print_puzzle_strings(char puzzle_strings[1200][80], int start_index, int end_index);
int extract_checksum(char* puzzle_strings, int length, struct encrypted_data* data);
int8_t find_letter_index(char c);
int8_t count_letters(char* puzzle_strings, int length, struct encrypted_data* data);
int8_t find_highest_occurance(struct encrypted_data* data);
int8_t find_five_most_frequent_letters(struct encrypted_data* data);

void init_encrypted_data(struct encrypted_data* data){
    *data->checksum = '\0';
    *data->room_data = '\0';
    data->sector_id = 0;
    for(int i = 0; i < LETTERS_IN_ALPHABET; i++){
        data->letters[i] = 0;
    }
}

void split_puzzle_into_lines(char* input, char puzzle_strings[1200][80]){
    int number_of_lines = 0;
    char* token = NULL;
    token = strtok(input, "\n");
    while(token != NULL){
        //printf("token: %s\n", token);
        strcpy(puzzle_strings[number_of_lines++], token);
        token = strtok(NULL, "\n");
    }
}

void print_puzzle_strings(char puzzle_strings[1200][80], int start_index, int end_index){
    for(int index = start_index; index < end_index; index++){
        if(puzzle_strings[index][0] != '\0'){
            printf("Rows[%d]: %s\n", index, puzzle_strings[index]);
        }
    }
}

int extract_checksum(char* puzzle_strings, int length, struct encrypted_data* data){
    
    int pos_left_backet = -1, pos_right_backet = -1;
    for(int i = 0; i < length; i++){
        char c = puzzle_strings[i];
        if(c == LEFT_BRACKET){
            pos_left_backet = i;
        }
        if(c == RIGHT_BRACKET){
            pos_right_backet = i;
        }
    }

    if((pos_left_backet == -1) || (pos_right_backet == -1)){
        return ERROR;
    }

    if((pos_right_backet - pos_left_backet) != (CHECKSUM_STRING_LENGTH)){
        return ERROR;
    }

    strncpy(data->checksum, &puzzle_strings[pos_left_backet+1], CHECKSUM_LENGTH);
    puzzle_strings[pos_left_backet] = '\0';

    return strlen(puzzle_strings);
}

int extract_sector_id(char* puzzle_strings, int length, struct encrypted_data* data){
    // Check first char from end
    int i = length-1;
    if(!isdigit(puzzle_strings[i])){
        return -1;
    }
    // Keep stepping from end, until not numeric character
    char sector_string[10] = {'\0'};
    while(isdigit(puzzle_strings[i])){
        i--;
    };

    strncpy(sector_string, &puzzle_strings[i+1], (length - i));
    int sector = atoi(sector_string);
    
    if(sector == 0){
        return ERROR;
    }
    data->sector_id = sector;
    // Remove SectorID from old string
    puzzle_strings[i+1] = '\0';
    return strlen(puzzle_strings);
}

int8_t find_letter_index(char c){
    int8_t letter_index;
    for(letter_index = 0; letter_index < LETTERS_IN_ALPHABET; letter_index++){
        if(c == alphabet[letter_index]){
            return letter_index;
        }
    }
    return -1;
}

void print_letter_occurance(struct encrypted_data* data){
    for(int i = 0; i < LETTERS_IN_ALPHABET; i++){
        if(data->letters[i] > 0){
            printf("'%c':%u\n", 
                alphabet[i], 
                data->letters[i]);
        }
    }
}

int8_t count_letters(char* puzzle_strings, int length, struct encrypted_data* data){
    for(int i = 0; i < length-1; i++){
        if(puzzle_strings[i] == DASH){
            continue;
        };

        char c = puzzle_strings[i];
        int8_t letter_index = find_letter_index(c);
        if(letter_index == ERROR){
            return ERROR;
        }

        data->letters[letter_index] = data->letters[letter_index] + 1;
        /*
        printf("Letter: %c Count: %u\n", 
            alphabet[letter_index], 
            data->letters[letter_index]);*/
    }
}

int8_t find_highest_occurance(struct encrypted_data* data){
    int8_t max_value = -1;
    for(int i = 0; i < LETTERS_IN_ALPHABET; i++){
        if(data->letters[i] > max_value){
            max_value = data->letters[i];
            data->max_value = max_value;
        }
    }
    return max_value;
}

int8_t find_five_most_frequent_letters(struct encrypted_data* data){
    char string[6] = {'\0'};
    uint8_t max = data->max_value;
    uint8_t letters_found = 0, string_index = 0;
    while((letters_found <= 5) && (max > 0)){
        for(int j = 0; j < LETTERS_IN_ALPHABET; j++){
            if((data->letters[j] == max) && (letters_found < 5)){
                printf("'%c':%u\n", 
                    alphabet[j], 
                    data->letters[j]);
                string[letters_found++] = alphabet[j];
            }
        }
        max--;
    }
    if(strlen(string) == 5){
        strncpy(data->room_data, string, 5);
        return OK;
    }else{
        return ERROR;
    }
}

bool is_room_valid(struct encrypted_data* data){
    int8_t res = strcmp(data->room_data, data->checksum);
    printf("Com res: %d ", res);

    if(res == 0){
        return true;
    }else {
        return false;
    }
}

int main(int argc, char const *argv[])
{
    // Read file to an array
    long size = get_file_size(INPUT_FILE_NAME);    
    char* input =  allocate(size);
    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        abort();
    }else {
        printf("File read succesfully! Size: %lu\n", size);
    }
    
    // Split array into lines
    char puzzle_strings[1200][80] = {'\0'};
    split_puzzle_into_lines(input, puzzle_strings);

    // Start parsing
    int string_index = 0; 
    uint64_t sum_sectorid_valid_rooms = 0;
    while(puzzle_strings[string_index][0] != '\0'){
        struct encrypted_data data;
        init_encrypted_data(&data);
        printf("Puzzle: %s\n", puzzle_strings[string_index]);

        // Function -> Extract checksum
        int length = strlen(puzzle_strings[string_index]);
        length = extract_checksum(puzzle_strings[string_index], length, &data);

        if(length == ERROR){
            printf("Failed to locate checksum position!\n");
            abort();
        }else{
            printf("Checksum: %s\t\t", data.checksum);
        }

        // Function -> Extract sectorID
        length = extract_sector_id(puzzle_strings[string_index], length, &data);

        if(length == ERROR){
            printf("Failed to extract sector-id!\n");
            abort();
        }else{
            printf("Sector-id: %d\t\t\n", data.sector_id);
        }

        // Function -> Count letter occurance;
        int8_t ret = count_letters(puzzle_strings[string_index], length, &data);
        if(ret == ERROR){
            printf("Failed to extract count letters!\n");
            abort();
        }else{
            print_letter_occurance(&data);
        }

        int8_t res = find_highest_occurance(&data);
        if(res == ERROR){
            printf("Failed to return letter occurance!\n");
            abort();
        }else{
            printf("Highest Count: %u\n", res);
        }
        
        // Find 5 most freq letters
        res = find_five_most_frequent_letters(&data);

        if(res == ERROR){
            printf("Failed to extract encrypted room info!\n");
            abort();
        }else{
            printf("Encr. Room: %s Checksum: %s\n", data.room_data, data.checksum);
        }

        // Compare extracted enc. room data with checksum 
        if(is_room_valid(&data)){
            sum_sectorid_valid_rooms += data.sector_id;
            printf("Valid room!\n");
        }else{
            printf("Invalid room!\n");
        }

        string_index++;
        printf("%c%c", NEWLINE, NEWLINE);
    }
    printf("\nSUM SECTORID: %u ", sum_sectorid_valid_rooms);

    free_allocation(input);
    return 0;
}

// Correct: 158835