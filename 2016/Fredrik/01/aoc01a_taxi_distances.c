/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/01/aoc01a_taxi_distances.c
 * @date 2026-10-03
 * @brief finding taxidistance from drop
 */

//// OPTIONS ////
#define INPUTPATH "input/01a_input.txt"

// comment/uncomment sets them
// #define PRINT_INPUT_ANALYSIS

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

//// structs ////
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

//// prototypes ////
void free_alloc(AllocHolder *heap);
uint8_t direction_from_index(int64_t ind);
uint8_t analyze_input(const char *inputpath, uint64_t *number_of_opcodes);
uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
); // imported

int main(void) {

    // initialize
    AllocHolder heap = {0};
    Position pos = {0}; // Start in origo
    pos.dir_index = 1;  // Start pointing north

    // analyze input
    char *inputpath = INPUTPATH;
    uint64_t number_of_opcodes;
    if (analyze_input(inputpath, &number_of_opcodes)) goto error;
    // printf("Number of opcodes: %lu\n", number_of_opcodes);

    // transform input to opcodes:

    //     find out how many opcodes with rowcolcunt
    //     allocate an array of opcodes
    //     fill the array with a modified rowcolcunt

    // for all opcodes:
    // move(&Position, opcode_array[i])
    // do the pythagoras

    goto clean_exit;

error:
    free_alloc(&heap);
    return 1;

clean_exit:
    printf("main ran thru no problems\n");
    return 0;
}

void free_alloc(AllocHolder *heap) {

    if (!heap) return;
    // free(heap->sensor1_val_arr);
    // all other frees here
    memset(heap, 0, sizeof(AllocHolder));
}

uint8_t direction_from_index(int64_t ind) {
    static const uint8_t direction_from_ind_arr[] = {0, 1, 2, 3};     // {east, north, west, south}
    static const uint8_t neg_direction_from_ind_arr[] = {0, 3, 2, 1}; // {east, south, west, north}

    return (ind >= 0) ? direction_from_ind_arr[ind % 4] : neg_direction_from_ind_arr[(-ind) % 4];
}

uint8_t analyze_input(const char *inputpath, uint64_t *number_of_opcodes) {

    uint64_t number_of_rows;
    uint64_t number_of_cols;
    int64_t elem_max;
    int64_t elem_min;
#ifdef PRINT_INPUT_ANALYSIS
    const uint8_t print = 1;
#else
    const uint8_t print = 0;
#endif
    if (rowcol_cunt(inputpath, &number_of_rows, &number_of_cols, &elem_max, &elem_min, print)) {
        printf("Weird input, exiting program\n");
        return 1;
    } else {
        *number_of_opcodes = number_of_cols;
        return 0;
    }
}
