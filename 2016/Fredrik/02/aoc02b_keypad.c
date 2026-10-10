/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/02/aoc02b_keypad.c
 * @author AceHole69
 * @date 2026-10-05
 * @brief Making a robot arm press a keypad from LDUR input
 */

//// OPTIONS ////
// #define INPUTPATH "input/02_input_testcase.txt"
#define INPUTPATH "input/02_input.txt"

// comment/uncomment sets them
// #define PRINT_INPUT_ANALYSIS

//// constants ////
#define MAX_CODE_LENGTH 1000000 // Has to be less than UINT64MAX
#define STARTING_POS '5'

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//// prototypes ////

/**
 * @brief Verifies input and finds out how long the answer code is
 * @param file_name
 * @param code_length adr outside return
 * @return 0 success, 1 failure or weird input
 */
uint8_t startup(const char *file_name, uint64_t *code_length);
/**
 * @brief Moves robotarm over keypad
 * @param pos position before movement, ex. '5'
 * @return position after movement, ex 'A'
 */
char move_L(const char pos);
/**
 * @brief Moves robotarm over keypad
 * @param pos position before movement, ex. '5'
 * @return position after movement, ex 'A'
 */
char move_D(const char pos);
/**
 * @brief Moves robotarm over keypad
 * @param pos position before movement, ex. '5'
 * @return position after movement, ex 'A'
 */
char move_U(const char pos);
/**
 * @brief Moves robotarm over keypad
 * @param pos position before movement, ex. '5'
 * @return position after movement, ex 'A'
 */
char move_R(const char pos);

int main(void) {
    uint8_t returncode = 1;

    // Memory safety
    char *code = NULL;
    FILE *pf = NULL;

    // calloc
    uint64_t code_length = 0;
    if (startup(INPUTPATH, &code_length)) goto out;
    code = calloc(code_length + 1, sizeof(char)); // + 1 and calloc leaves trailing zero = string
    if (!code) goto out;

    // initialize
    uint64_t code_ind = 0;
    char pos = STARTING_POS;

    pf = fopen(INPUTPATH, "r");
    if (!pf) {
        fprintf(stderr, "Something wrong with file-opening in main, mayby wrong file-name?, exiting program\n");
        goto out;
    }

    // main loop
    int c;
    while ((c = getc(pf)) != EOF) {
        switch (c) {
        case 'L':
            pos = move_L(pos);
            break;
        case 'D':
            pos = move_D(pos);
            break;
        case 'U':
            pos = move_U(pos);
            break;
        case 'R':
            pos = move_R(pos);
            break;
        case '\n':
            if (code_ind < code_length) code[code_ind++] = pos;
            break;
        default:
            fprintf(stderr, "unexpected character: %c, exiting program\n", c);
            goto out;
        }
    }

    // Finializing
    printf("Answer part b: %s\n", code);
    returncode = 0;

out:
    free(code);
    code = NULL;

    if (pf != NULL) {
        fclose(pf);
        pf = NULL;
    }

    return returncode;
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
        fprintf(stderr, "Weird input, exiting program\n");
        return 1;
    }
    if (number_of_rows > MAX_CODE_LENGTH) {
        fprintf(stderr, "Expected code output to long, exiting program\n");
        return 1;
    }

    *code_length = number_of_rows;
    return 0;
}

char move_L(const char pos) {
    switch (pos) {
    case '1':
    case '2':
    case '5':
    case 'A':
    case 'D':
        // These are the left edges
        return pos;
    default:
        return (pos - 1);
    }
}

char move_D(const char pos) {
    switch (pos) {
    case '5':
    case 'A':
    case 'D':
    case 'C':
    case '9':
        // These are the down edges
        return pos;
    case '1':
        return '3';
    case '2':
    case '3':
    case '4':
        return pos + 4;
    case '6':
    case '7':
    case '8':
        // I hope I read the ASCII right
        // '6' = 54
        // 'A' = 65
        return pos + 11;
    case 'B':
        return 'D';
    default:
        fprintf(stderr, "SISO\n");
        return pos;
    }
}

char move_U(const char pos) {
    switch (pos) {
    case '5':
    case '2':
    case '1':
    case '4':
    case '9':
        // These are the up edges
        return pos;
    case 'D':
        return 'B';
    case 'A':
    case 'B':
    case 'C':
        return pos - 11;
    case '6':
    case '7':
    case '8':
        return pos - 4;
    case '3':
        return '1';
    default:
        fprintf(stderr, "SISO\n");
        return pos;
    }
}

char move_R(const char pos) {
    switch (pos) {
    case '1':
    case '4':
    case '9':
    case 'C':
    case 'D':
        // These are the right edges
        return pos;
    default:
        return (pos + 1);
    }
}
