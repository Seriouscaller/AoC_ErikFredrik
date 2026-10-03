/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/01/aoc01a_taxi_distances.c
 * @date 2026-10-03
 * @brief finding taxidistance from drop
 */

// imports
#include <stdio.h>
#include <string.h>

// structs
typedef struct {
    // holds all allocated memory, all pointers of course
    // uint8_t* sensor1_val_arr;
} AllocHolder;

// prototypes
void free_alloc(AllocHolder *heap);

int main(void) {

    // initialize memalloc struct
    AllocHolder heap = {0};

    // initialize Position and direction_from_ind_arr
    // transform input to opcodes:
    //     find out how many opcodes with rowcolcunt
    //     allocate an array of opcodes
    //     fill the array with a modified rowcolcunt
    // for all opcodes:
    // move(&Position, opcode_array[i])
    // do the pythagoras

    goto clean_exit;

memerror:
    printf("memfail\n");
    free_alloc(&heap);
    return 7; // What is proper exitcode for this?

clean_exit:
    printf("main ran thru no problems\n");
    return 0;
}

void free_alloc(AllocHolder *heap) {

    if (!heap)
        return;
    // free(heap->sensor1_val_arr);
    // all other frees here
    memset(heap, 0, sizeof(AllocHolder));
}
