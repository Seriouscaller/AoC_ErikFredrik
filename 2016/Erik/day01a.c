#include <stdio.h>
#include <stdlib.h>

int read_file(char* file_name, char* array){
    FILE *fp = fopen(file_name, "r");
    char ch;
    
    if(fp == NULL) {
        return 1;
    }

    int i = 0;
    while ((ch = fgetc(fp)) != EOF){
        array[i++] = ch;
    }
    array[++i] = '\0';
    fclose(fp);

    return 0;
}

int main(){
    char* file_name = "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day01_input_sample.txt";
    char array[500];
    
    int ret = read_file(file_name, array);
    if(ret == 1 ){
        printf("Unable to open file!");
        exit(1);
    }

    printf("%s", array);
    
    for(int i = 0; i < 20; i++){
        printf("");
    }
    
    return 0;
}
    