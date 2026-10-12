/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/02/aoc02a_keypad.c
 * @author AceHole69
 * @date 2026-10-05
 * @brief Making a robot arm press a keypad from LDUR input
 */

//// OPTIONS ////
// #define INPUTPATH "input/02_input_testcase.txt"
// #define INPUTPATH "input/02_input_testcase_nonaccepted_tolonginput.txt"
#define INPUTPATH "input/02_input.txt"

// comment/uncomment sets them
// #define VERIFY_INPUT
// #define PRINT_INPUT_ANALYSIS

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

//// prototypes ////

/**
 * @brief Verifies that the output code fits inside a uint64_t
 * @param inputpath
 * @return 0 on it fits, 1 on error or doesnt fit.
 */
uint8_t verify_input(const char *inputpath);
uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
);                                          // imported
uint8_t n_digits_uint64_t(uint64_t number); // imported

int main(void) {

    FILE *pf = NULL;
    uint8_t ret = 1;

    // verify input
#ifdef VERIFY_INPUT
    if (verify_input(INPUTPATH)) return 1;
#endif

    // initialize
    uint8_t x = 2;
    uint8_t y = 1;
    uint64_t code = 0;

    pf = fopen(INPUTPATH, "r");
    if (!pf) {
        printf("Something wrong with file-opening in main, mayby wrong file-name?");
        goto out;
    }

    // main loop
    int c;
    while ((c = getc(pf)) != EOF) {
        // printf("%c", c);
        switch (c) {
        default:
            printf("unexpected character: %c, exiting program\n", c);
            goto out;
        case 'L':
            x = (x == 1) ? 1 : x - 1;
            break;
        case 'D':
            y = (y == 2) ? 2 : y + 1;
            break;
        case 'U':
            y = (y == 0) ? 0 : y - 1;
            break;
        case 'R':
            x = (x == 3) ? 3 : x + 1;
            break;
        case '\n':
            code = code * 10 + (x + 3 * y);
            break;
        }
    }

    // Finializing
    // printf("program ran trough, no problems\n");
    printf("Answer part a: %" PRIu64 "\n", code);
    ret = 0;

out:
    fclose(pf);

    return ret;
}

uint8_t verify_input(const char *inputpath) {

    uint64_t max_int = UINT64_MAX;
    uint64_t number_of_rows;
    uint8_t number_of_rows_allowed = n_digits_uint64_t(max_int) - 1; // - 1 bc 9999
    uint64_t number_of_cols;
    int64_t elem_max;
    int64_t elem_min;

#ifdef PRINT_INPUT_ANALYSIS
    const uint8_t print = 1;
    printf("uint64_t max: %" PRIu64 "\n", max_int);
    printf("max number of rows allowed: %" PRIu8 "\n", number_of_rows_allowed);
#else
    const uint8_t print = 0;
#endif
    if (rowcol_cunt(inputpath, &number_of_rows, &number_of_cols, &elem_max, &elem_min, print)) {
        printf("Weird input, exiting program\n");
        return 1;
    }
    if (number_of_rows > number_of_rows_allowed) {
        printf("Expected code output to long, exiting program\n");
        return 1;
    } else {
        return 0;
    }
}
