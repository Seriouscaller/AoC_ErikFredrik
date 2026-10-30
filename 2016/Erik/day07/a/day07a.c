#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#define NUMBER_OF_LETTERS_IN_THE_ALPHABET 26
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day07/a/day07_input_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day07/a/day07_input_full.txt"
#define NEWLINE '\n'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

bool is_hypernet_ready_for_processing(int left_bracket_index, int right_bracket_index);
bool locate_and_remove_hypernets(char* line_of_puzzle, uint16_t length);
bool is_containing_abba(char* hypernet_string, int string_size);

bool is_hypernet_ready_for_processing(int left_bracket_index, int right_bracket_index){
    return ((-1 < left_bracket_index) && (-1 < right_bracket_index));
}

bool locate_and_remove_hypernets(char* line_of_puzzle, uint16_t length){
    bool has_valid_hypernets = true;
    int start_hypernet = -1, end_hypernet = -1;

    // Parse line, char by char
    for(int i = 0; i < strlen(line_of_puzzle); i++){
        char ch = line_of_puzzle[i];
        if(ch == LEFT_BRACKET){
            start_hypernet = i;
        }
        if(ch == RIGHT_BRACKET){
            end_hypernet = i;
        }

        // Found  "[ ... ]"  Check if hypernet passes
        if(is_hypernet_ready_for_processing(start_hypernet, end_hypernet)){
            char hypernet_string[100] = {'\0'};
            strncpy(hypernet_string, &line_of_puzzle[start_hypernet], (end_hypernet - start_hypernet)+1);
            for(int j = start_hypernet; j <= end_hypernet; j++){
                line_of_puzzle[j] = WHITE_SPACE;
            }

            // If hypernet contains ABBA, fail the entire line!
            if(is_containing_abba(hypernet_string, end_hypernet - start_hypernet)){
                has_valid_hypernets = false;
                break;
            }
            start_hypernet = -1, end_hypernet = -1;
        }
    };
    return has_valid_hypernets;
};

bool is_containing_abba(char* hypernet_string, int string_size){
    for(int i = 1; i < string_size - 2; i++){
        char first_of_four_chars = hypernet_string[i-1], second_of_four_chars = hypernet_string[i];
        char third_of_four_chars = hypernet_string[i+1], fourth_of_four_chars = hypernet_string[i+2];
        int first_letter_index = i-1, second_letter_index = i, third_letter_index = i+1,fourth_letter_index = i+2;

        // If any ABBA is found inside the hypernet, fail the line!
        // if ABBA found outside the hypernet, approve the line!
        if(!(second_of_four_chars == third_of_four_chars)){
            continue;
        }
        
        if(!(first_of_four_chars == fourth_of_four_chars)){
            continue;
        }

        if(first_of_four_chars == second_of_four_chars){
            continue;
        }
        return true;

    }
    return false;
}

int main(int argc, char *argv[]){
    struct file_information file_info = get_file_information(INPUT_FILE_NAME);
    char* input =  allocate(file_info.file_size);

    if(file_info.rows == 0 || file_info.file_size == 0){
        printf("Unable to acquire file information!");
        free_allocation(input);
        abort();
    }

    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        free_allocation(input);
        abort();
    }else {
        printf("File read succesfully! Size: %lu Rows: %lu Columns: %lu\n", file_info.file_size, file_info.rows, file_info.columns);
    }

    uint32_t number_of_valid_ips = 0;
    long unsigned start = 0, stop = 0, row = 0;
    // Parse one line at a time
    for(long i = 0; i < file_info.file_size; i++){
        char ch = input[i];
        if(input[i] == NEWLINE){
            input[i] = '\0';
            stop = i++;
            char line_of_puzzle_code[200] = {'\0'};
            strncpy(line_of_puzzle_code, &input[start], stop - start);
            start = i;
            printf("Row[%u] Lineofpuzzle: %s ", row, line_of_puzzle_code);

            // Parse Line
            bool has_valid_hypernets = locate_and_remove_hypernets(line_of_puzzle_code, strlen(line_of_puzzle_code));
            printf("Valhypern: %u ", has_valid_hypernets);
            if(!has_valid_hypernets){
                row++;
                printf("%c", NEWLINE);
                continue;
            }
            // Add Function to scan for ABBAs in remaining puzzle-line
            bool is_valid_ip = is_containing_abba(line_of_puzzle_code, strlen(line_of_puzzle_code));
            if(is_valid_ip){
                number_of_valid_ips++;
            }
            printf("Valid IP: %u ", is_valid_ip);
            printf("%c", NEWLINE);
            row++;
        }
    }

    printf("Number of valid IPs: %u\n", number_of_valid_ips);
    free_allocation(input);
    return 0;
}

// Number of valid IPs: 105
// Correct!