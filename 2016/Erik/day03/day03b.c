#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "input_output.h"
#include <string.h>

//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day03/day03_input_sample.txt"
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day03/day03_input_full.txt"
#define NEWLINE '\n'
#define INPUT_SIZE 40000
#define TRIANGLE_ARRAY_SIZE 1000
#define ARRAY_ROWS 3
#define ARRAY_COLUMNS 3

struct triangle {
    int sides[ARRAY_ROWS][ARRAY_COLUMNS];
};

void init_triangles(struct triangle tris[TRIANGLE_ARRAY_SIZE]){
    int width = 3, height = 3;
    for(int elements = 0; elements < 1000; elements++){
        for(int rows = 0; rows < 3; rows++){
            for(int columns = 0; columns < 3; columns++){
                tris[elements].sides[rows][columns] = 0;
            }
        }
    }
}

int sum_triangle_sides(struct triangle tri){
    int sum = 0;
    for(int i = 0; i < 3; i++){
        //sum += tri.sides[i][i];
        ;
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
    struct triangle triangles[TRIANGLE_ARRAY_SIZE];
    init_triangles(triangles);

    long size = get_file_size(INPUT_FILE_NAME);
    
    char* input =  allocate(size);
    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        abort();
    }else {
        printf("File read succesfully!\n");
    }
    printf("%s", input);

    int array_row_index = 0, array_column_index = 0;
    int side_index = 0, triangle_index = 0;
    char* token = NULL;
    token = strtok(input, " ");
    while(token != NULL){
        printf("token: %s\n", token);
        int number = atoi(token);
        printf("number: %d\n", number);
        
        triangles[triangle_index].sides[array_column_index][array_row_index] = number;
        
        // Next Row
        if(array_row_index < 2){
            array_row_index++;
        // A full 3x3 array!
        }else if(array_row_index == 2 && array_column_index == 2){
            triangle_index++;
            array_row_index = 0;
            array_column_index = 0;
        // Reaching third row, move to next column, reset row
        }else{
            array_column_index++;
            array_row_index = 0;
        }
        token = strtok(NULL, " ");
    }

    int i = 0, number_of_possible_trianges = 0;
    while(triangles[i].sides[0][0] != 0){
        printf("\nRow[%d]: ", i);
        printf("%d %d %d ",
        triangles[i].sides[0][0],
        triangles[i].sides[1][0],
        triangles[i].sides[2][0]);

        for(int j = 0; j < 3; j++){
            int sum_of_sides = triangles[i].sides[0][j] + triangles[i].sides[1][j]+ triangles[i].sides[2][j];
            int int_array[3] = {triangles[i].sides[0][j], triangles[i].sides[1][j], triangles[i].sides[2][j]};
            int longest_side = max(int_array);

            if((sum_of_sides - longest_side) > longest_side){
                number_of_possible_trianges++;
            }
            printf("Sum: %d Longest: %d \n", sum_of_sides, longest_side);
        }
        i++;
    }
    printf("Number of possible tris: %d \n", number_of_possible_trianges);
    
    free_allocation(input);
    return EXIT_SUCCESS;
}

