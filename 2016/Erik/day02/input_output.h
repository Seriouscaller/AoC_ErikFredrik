#pragma once
#include <stdio.h>
#include <stdlib.h>

long get_file_size(char* file_name);
void* allocate(size_t size_of_text_file);
void free_allocation(void* ptr);
int read_file(char* file_name, char* array);
