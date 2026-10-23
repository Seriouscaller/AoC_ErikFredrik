#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <ctype.h>

#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day04/a/puzzle_sample.txt"
#define CHECKSUM_LENGTH 5
#define NEWLINE '\n'
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'
#define DASH '-'

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

struct letter_item {
    char letter;
    int occurance;
};

struct encrypted_data {
    char checksum[6];
    int sector_id;
    struct letter_item letters[30];
};

void init_encrypted_data(struct encrypted_data* data);
void split_puzzle_into_lines(char* input, char puzzle_strings[1200][80]);
void print_puzzle_strings(char puzzle_strings[1200][80], int start_index, int end_index);
int extract_checksum(char* puzzle_strings, int length, struct encrypted_data* data);
void count_letters(char* puzzle_strings, int length, struct encrypted_data* data);

void init_encrypted_data(struct encrypted_data* data){
    *data->checksum = '\0';
    data->sector_id = 0;
    for(int i = 0; i < 30; i++){
        data->letters[i].letter = '\0';
        data->letters[i].occurance = '\0';
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

    if((pos_right_backet - pos_left_backet) != (CHECKSUM_LENGTH + 1)){
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

void count_letters(char* puzzle_strings, int length, struct encrypted_data* data){


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
            printf("Sector-id: %d\t\t", data.sector_id);
        }

        // Function -> Count letter occurance;
        count_letters(puzzle_strings[string_index], length, &data);
        // Function -> Sort letter array

        // Function -> Compare 5 most common letters with checksum

        string_index++;
        printf("%c%c", NEWLINE, NEWLINE);
    }

    //print_puzzle_strings(puzzle_strings, 0, 10);
    free_allocation(input);
    return 0;
}