#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct file_information {
    long file_size;
    long rows;
    long columns;
};

struct file_information get_file_information(char* file_name){
    FILE *fp = fopen(file_name, "r");
    struct file_information file_info = {.file_size = 0, .rows = 0, .columns = 0};

    if(fp == NULL) {
        printf("Failed to open file!\n");
        return file_info;
    }

    int columns = 0; int ch;
    while ((ch = fgetc(fp)) != EOF){
        columns++;
        if(ch == '\n'){
            file_info.rows++;
            file_info.columns = columns - 1;
            columns = 0;
        }
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