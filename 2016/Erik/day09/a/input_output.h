#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct file_information {
    uint64_t file_size;
    uint64_t rows;
    uint16_t max_columns;
};

struct file_information get_file_information(char* file_name);
void* allocate(size_t size_of_text_file);
void free_allocation(void* ptr);
int read_file(char* file_name, char* array);
int8_t split_puzzle_into_rows(uint16_t rows, uint16_t columns, char lines[rows][columns], char* puzzle_string, struct file_information* file_info);