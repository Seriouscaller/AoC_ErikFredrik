/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/01/aoc01a_taxi_distances.c
 * @date 2026-10-03
 * @brief finding taxidistance from drop
 */

// imports
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

// structs
typedef struct {
    // holds all allocated memory, all pointers of course
    // uint8_t* sensor1_val_arr;
} AllocHolder;

typedef struct {
    int64_t x;
    int64_t y;
    int64_t dir_index;
} Position;

typedef struct {
    int8_t turn; // -1 or 1
    uint64_t steps;
} Opcode;

// prototypes
void free_alloc(AllocHolder *heap);
uint8_t direction_from_index(int64_t ind);

int main(void) {

    // initializing
    AllocHolder heap = {0};
    Position pos = {0};
    pos.dir_index = 1;

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

uint8_t direction_from_index(int64_t ind) {
    static const uint8_t direction_from_ind_arr[] = {0, 1, 2, 3};     // {east, north, west, south}
    static const uint8_t neg_direction_from_ind_arr[] = {0, 3, 2, 1}; // {east, south, west, north}

    return (ind >= 0) ? direction_from_ind_arr[ind % 4] : neg_direction_from_ind_arr[(-ind) % 4];
}
