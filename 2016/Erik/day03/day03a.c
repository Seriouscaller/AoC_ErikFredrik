#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "input_output.h"
#include <string.h>


//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day03/day03_input_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day03/day03_input_full.txt"
#define NEWLINE '\n'
#define ARRAY_SIZE 40000

struct triangle {
    int sides[3];
};

void init_triangles(struct triangle tris[ARRAY_SIZE], int size){
    for(int i = 0; i < size; i++){
        tris[i].sides[0] = 0;
        tris[i].sides[1] = 0;
        tris[i].sides[2] = 0;
    }
}

int sum_triangle_sides(struct triangle tri){
    int sum = 0;
    for(int i = 0; i < 3; i++){
        sum += tri.sides[i];
    }
    return sum;
}

int max(int numbers[3]){
    int max = -1;
    
    for(int i = 0; i < 3; i++){
        if(numbers[i] > max){
            max = numbers[i];
        }
    }
    return max;
}

int main(){
    struct triangle triangles[2000];
    init_triangles(triangles, 2000);

    long size = get_file_size(INPUT_FILE_NAME);
    
    char* input =  allocate(size);
    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        abort();
    }else {
        printf("File read succesfully!\n");
    }

    int side_index = 0, triangles_index = 0;
    printf("%s", input);
    char* token = NULL;
    token = strtok(input, " ");
    while(token != NULL){
        printf("token: %s\n", token);
        int number = atoi(token);
        printf("number: %d\n", number);
        
        triangles[triangles_index].sides[side_index] = number;
        
        if(side_index < 2){
            side_index++;
        }else {
            triangles_index++;
            side_index = 0;
        }
       
        token = strtok(NULL, " ");
    }

    int i = 0, number_of_possible_trianges = 0;
    while(triangles[i].sides[0] != 0){
        printf("Row[%d]: ", i);
        printf("%d %d %d ",
        triangles[i].sides[0],
        triangles[i].sides[1],
        triangles[i].sides[2]);

        int sum_of_sides = sum_triangle_sides(triangles[i]);
        int longest_side = max(triangles[i].sides);

        if((sum_of_sides - longest_side) > longest_side){
            number_of_possible_trianges++;
        }

        printf("Sum: %d Longest: %d \n", sum_of_sides, longest_side);
        i++;
    }
    printf("Number of possible tris: %d \n", number_of_possible_trianges);
    
    free_allocation(input);
    return EXIT_SUCCESS;
}

