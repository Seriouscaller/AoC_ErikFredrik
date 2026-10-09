/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/02/aoc02b_keypad.c
 * @author AceHole69
 * @date 2026-10-05
 * @brief Making a robot arm press a keypad from LDUR input
 */

//// OPTIONS ////
#define INPUTPATH "input/02_input_testcase.txt"
// #define INPUTPATH "input/02_input.txt"

// comment/uncomment sets them
// #define PRINT_INPUT_ANALYSIS

//// imports ////
#include <stdint.h>
#include <stdlib.h>

//// constants ////
#define MAX_CODE_LENGTH 1000000 // Has to be less than UINT64MAX

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdio.h>

//// prototypes ////
uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
); // imported
uint8_t startup(const char *file_name, uint64_t *code_length);

int main(void) {

    // Memory safety
    char *code = NULL;

    // malloc
    uint64_t code_length = 0;
    if (startup(INPUTPATH, &code_length)) goto error;
    code = malloc(code_length * sizeof(char));
    if (!code) goto error;

    // initialize
    uint64_t code_ind = 0;
    char pos = '5';

    FILE *pf;
    pf = fopen(INPUTPATH, "r");
    if (!pf) {
        printf("Something wrong with file-opening in main, mayby wrong file-name?");
        goto error;
    }

    // main loop
    char c;
    while ((c = getc(pf)) != EOF) {
        // printf("%c", c);
        switch (c) {
        default:
            printf("unexpected character: %c, exiting program\n", c);
            goto error_while_open_file;
        case 'L':
            // x = (x == 1) ? 1 : x - 1;
            break;
        case 'D':
            // y = (y == 2) ? 2 : y + 1;
            break;
        case 'U':
            // y = (y == 0) ? 0 : y - 1;
            break;
        case 'R':
            // x = (x == 3) ? 3 : x + 1;
            break;
        case '\n':
            code[code_ind++] = pos;
            break;
        }
    }

    // Finializing
    fclose(pf);
    code[code_ind++] = 0; // "Transform" code to a string
    printf("Answer part b: ");
    printf(code);
    printf("\n");

    goto clean_exit;

error_while_open_file:
    free(code);
    if (pf != NULL) fclose(pf);
    return 1;

error:
    free(code);
    return 1;

clean_exit:
    free(code);
    printf("program ran trough, no problems\n");
    return 0;
}

uint8_t startup(const char *inputpath, uint64_t *code_length) {

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
    }
    if (number_of_rows > MAX_CODE_LENGTH) {
        printf("Expected code output to long, exiting program\n");
        return 1;
    }

    *code_length = number_of_rows + 1; // + 1 to fit an ending 0 to be able to handle it as a string
    return 0;
}
