#include <stdio.h>
#include <stdlib.h>

long get_file_size(char* file_name){
    FILE *fp = fopen(file_name, "r");
    
    if(fp == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }
    int ret = fseek(fp, 0 , SEEK_END);
    if(ret){
        printf("fseek failed!\n");
        abort();
    }
    long size = ftell(fp);
    fclose(fp);
    return size;
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