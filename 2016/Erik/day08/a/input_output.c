#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

struct file_information {
    uint64_t file_size;
    uint64_t rows;
    uint16_t max_columns;
};

struct file_information get_file_information(char* file_name){
    FILE *fp = fopen(file_name, "r");
    struct file_information file_info = {.file_size = 0, .rows = 0, .max_columns = 0};

    if(fp == NULL) {
        printf("Failed to open file!\n");
        return file_info;
    }

    int max_columns = 0; int ch;
    while ((ch = fgetc(fp)) != EOF){
        max_columns++;
        if(ch != '\n'){
            continue;
        }
        
        file_info.rows++;
        if((max_columns - 1) > file_info.max_columns){
            file_info.max_columns = max_columns - 1;
        }
        max_columns = 0;
        
    }

    file_info.file_size = ftell(fp);
    fclose(fp);
    return file_info;
}

void* allocate(size_t size_of_text_file){
    if (size_of_text_file == 0){
        return NULL;
    }

    void* ptr = malloc(size_of_text_file * sizeof(char));
    if(ptr == NULL){
        printf("Memory Allocation failed!");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void free_allocation(void* ptr){
    if(ptr){
        free(ptr);
        ptr = NULL;
    }
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
    array[i++] = '\0';
    fclose(fp);

    return 0;
}

int8_t split_puzzle_into_rows(uint16_t rows, uint16_t columns, char lines[rows][columns], char* puzzle_string, struct file_information* file_info){
    uint64_t i = 0, start = 0, end = 0, index_row = 0;
    while(i < file_info->file_size){
        if(puzzle_string[i] == '\n'){
            end = i;
            uint64_t length_of_string_to_copy = end - start;
            if(length_of_string_to_copy <= columns && 0 < length_of_string_to_copy){
                strncpy(lines[index_row++], &puzzle_string[start], length_of_string_to_copy);
            }else{
                printf("Unable to copy line! Too long?\n");
            }
            start = end + 1;
        }
        i++;
    }
};