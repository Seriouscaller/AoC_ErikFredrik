#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#define NUMBER_OF_LETTERS_IN_THE_ALPHABET 26
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day07/b/day07_input_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day07/b/day07_input_full.txt"
#define NEWLINE '\n'
#define WHITE_SPACE ' '
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'

uint16_t ssl_ips = 0;

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

struct area_broadcast_accessor{
    char aba[4];
};

struct byte_allocation_block{
    char bab[4];
    bool has_bab;
};

struct analytics{
    struct area_broadcast_accessor aba_array[10];
    struct byte_allocation_block bab_array[10];
    uint8_t number_of_babs_logged;
    uint8_t number_of_abas_logged;
};

bool is_hypernet_ready_for_processing(int left_bracket_index, int right_bracket_index);
bool locate_and_remove_hypernets(char* line_of_puzzle, uint16_t length, struct analytics* analytics);
void search_hypernet_for_bab(char* hypernet_string, int string_size, struct analytics* analytics);
bool create_aba_from_bab(struct analytics* analytics, uint8_t index_bab);
bool is_supernet_containing_aba(char* hypernet_string, int string_size, struct analytics* analytics);

bool is_hypernet_ready_for_processing(int left_bracket_index, int right_bracket_index){
    return ((-1 < left_bracket_index) && (-1 < right_bracket_index));
}

bool locate_and_remove_hypernets(char* line_of_puzzle, uint16_t length, struct analytics* analytics){
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

            // If hypernet is missing BAB, proceed to next!
            search_hypernet_for_bab(hypernet_string, end_hypernet - start_hypernet, analytics);

            start_hypernet = -1, end_hypernet = -1;
        }
    };
    return has_valid_hypernets;
};

void search_hypernet_for_bab(char* hypernet_string, int string_size, struct analytics* analytics){
    for(int i = 1; i < string_size - 1; i++){
        char first_of_three_chars, second_of_three_chars, third_of_three_chars;
        first_of_three_chars = hypernet_string[i-1]; 
        second_of_three_chars = hypernet_string[i];
        third_of_three_chars = hypernet_string[i+1];

        // If any BAB is found inside the hypernet, save it
        // if ABA is also found outside the hypernet, approve the ip-address as SSL!
        if(!(first_of_three_chars == third_of_three_chars)){
            continue;
        }
        
        if((first_of_three_chars == second_of_three_chars)){
            continue;
        }

        if(second_of_three_chars == third_of_three_chars){
            continue;
        }

        // Arrive here if we found a xyx inside the brackets: [.xyx.]
        uint8_t current_bab_index = analytics->number_of_babs_logged;
        analytics->bab_array[current_bab_index].bab[0] = first_of_three_chars;
        analytics->bab_array[current_bab_index].bab[1] = second_of_three_chars;
        analytics->bab_array[current_bab_index].bab[2] = third_of_three_chars;

        analytics->number_of_babs_logged++;
    }
}

bool create_aba_from_bab(struct analytics* analytics, uint8_t index_bab){
    if(index_bab <= analytics->number_of_babs_logged){
        char bab_sides = analytics->bab_array[index_bab].bab[0];
        char bab_middle = analytics->bab_array[index_bab].bab[1];

        analytics->aba_array[index_bab].aba[0] = bab_middle;
        analytics->aba_array[index_bab].aba[1] = bab_sides;
        analytics->aba_array[index_bab].aba[2] = bab_middle;

        analytics->number_of_abas_logged++;
        return true;
    }
    printf("Failed to create aba from bab!\n");
    return false;
}

bool is_supernet_containing_aba(char* super_net, int string_size, struct analytics* analytics){
    for(int i = 0; i < analytics->number_of_abas_logged ; i++){
        for(int j = 1; j < string_size - 1; j++){
        
            char hyp_0 = super_net[j-1]; 
            char hyp_1 = super_net[j];
            char hyp_2 = super_net[j+1];

            uint8_t aba_index = analytics->number_of_abas_logged - 1;
            char aba_0 = analytics->aba_array[i].aba[0];
            char aba_1 = analytics->aba_array[i].aba[1];
            char aba_2 = analytics->aba_array[i].aba[2];

            if((hyp_0 == aba_0) && (hyp_1 == aba_1) && (hyp_2 == aba_2)){
                return true;
            }
        }
    }
    return false;
}

void print_analytics(struct analytics* analytics){
    if(analytics->number_of_babs_logged > 0){
        for(int i = 0; i < analytics->number_of_babs_logged; i++){
            printf("B[%d]: %s A: %s\t", i, analytics->bab_array[i].bab, analytics->aba_array[i].aba);
        }
    }
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
            struct analytics analytics = {
                .aba_array = {'\0'},        
                .bab_array = {'\0'},
                .number_of_abas_logged = 0,
                .number_of_babs_logged = 0,
            };

            input[i] = '\0';
            stop = i++;
            char line_of_puzzle_code[200] = {'\0'};
            strncpy(line_of_puzzle_code, &input[start], stop - start);
            start = i;
            printf("Row[%u] Lineofpuzzle: %s ", row, line_of_puzzle_code);

            // Find hypernets
            // Locate any babs inside it
            bool has_valid_hypernets = locate_and_remove_hypernets(line_of_puzzle_code, strlen(line_of_puzzle_code), &analytics);

            if(!has_valid_hypernets){
                row++;
                printf("%c", NEWLINE);
                continue;
            }

            // Convert babs into abas
            if(has_valid_hypernets && (0 < analytics.number_of_babs_logged)){
                for(int j = 0; j < analytics.number_of_babs_logged; j++){
                    create_aba_from_bab(&analytics, j);
                }
            }

            // Search for abas in remainder of line
            bool is_ip_ssl = is_supernet_containing_aba(line_of_puzzle_code, strlen(line_of_puzzle_code), &analytics);
            if(is_ip_ssl){
                ssl_ips++;
                printf(" SSL IP found! ");
            }
            if(analytics.number_of_babs_logged > 0){
                printf("%c", NEWLINE);
                print_analytics(&analytics);
            }
            printf("%c", NEWLINE);
            row++;
        }
    }

    printf("Number of SSL IPs: %u\n", ssl_ips);
    free_allocation(input);
    return 0;
}

// Number of SSL IPs
// Incorrect: 153