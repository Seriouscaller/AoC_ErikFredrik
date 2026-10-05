/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/02/aoc02a_keypad.c
 * @author AceHole69
 * @date 2026-10-05
 * @brief Making a robot arm press a keypad from LDUR input
 */

//// OPTIONS ////
#define INPUTPATH "input/02a_input_testcase.txt"
// #define INPUTPATH "input/02a_input.txt"

// comment/uncomment sets them
#define VERIFY_INPUT
#define PRINT_INPUT_ANALYSIS

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdio.h>

//// prototypes ////
uint8_t verify_input(const char *inputpath);
uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
); // imported

int main(void) {

    // verify input
#ifdef VERIFY_INPUT
    if (verify_input(INPUTPATH)) return 1;
#endif

    // initialize
    uint8_t x = 2;
    uint8_t y = 1;

    // main loop

    return 0;
}

uint8_t verify_input(const char *inputpath) {

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
        return 0;
    }
}
