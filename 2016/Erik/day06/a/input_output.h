#pragma once
#include <stdio.h>
#include <stdlib.h>

struct file_information {
    long file_size;
    long rows;
    long columns;
};

struct file_information get_file_information(char* file_name);
void* allocate(size_t size_of_text_file);
void free_allocation(void* ptr);
int read_file(char* file_name, char* array);
