/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/04/aoc04a_decode_decoys.c
 * @date 2026-10-18
 * @brief decoding valid rooms from decoys using checksum
 */

//// options ////
#define INPUTPATH "input/04_input_testcase.txt"
// #define INPUTPATH "input/04_input.txt"

// comment/uncomment sets them
// #define PRINT_INPUT_ANALYSIS
#define PRINT_PARTS

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//// prototypes ////

enum search_part {
    PART_NAME,     // Character part
    PART_SECTOR,   // Integer part
    PART_CHECKSUM, // Checksum part
    PART_DONE      // after ]
};

uint8_t startup(const char *file_name, uint64_t *line_length_max, uint64_t *nmb_of_lines);
uint8_t main_execution(
    FILE *pf,
    char *line_input,
    char *character_part_input,
    uint64_t nmb_of_lines,
    uint64_t line_length_max
);

uint8_t seperate_line(
    FILE *pf,
    char *line_input,
    char *character_part_input,
    uint64_t nmb_of_lines,
    uint64_t line_length_max,
    enum search_part part_of_search,
    char *checksum_input,
    uint64_t *number

);

int main(void) {

    // initializing
    uint8_t ret = 1;
    char *line_input = NULL;
    char *character_part_input = NULL;
    FILE *pf = NULL;

    uint64_t line_length_max = 0;
    uint64_t nmb_of_lines = 0;

    // Finding max linelength and number of lines
    if (startup(INPUTPATH, &line_length_max, &nmb_of_lines)) goto out;

    // Allocating
    line_input = calloc(line_length_max, sizeof(char));
    if (!line_input) {
        fprintf(stderr, "error calloc, exiting\n");
        goto out;
    }

    character_part_input = calloc(line_length_max, sizeof(char));
    if (!character_part_input) {
        fprintf(stderr, "error calloc, exiting\n");
        goto out;
    }

    pf = fopen(INPUTPATH, "r");
    if (!pf) {
        fprintf(stderr, "Something wrong with file-opening, mayby wrong file-name?\n");
        goto out;
    }

    // Main program
    if (main_execution(pf, line_input, character_part_input, nmb_of_lines, line_length_max)) goto out;

    // Exiting
    printf("\nprgrm ran thru, no prob\n");
    ret = 0;

out:
    free(line_input);
    line_input = NULL;

    free(character_part_input);
    character_part_input = NULL;

    fclose(pf);
    pf = NULL;

    return ret;
}

uint8_t startup(const char *file_name, uint64_t *line_length_max, uint64_t *nmb_of_lines) {

#ifdef PRINT_INPUT_ANALYSIS
    uint8_t print = 1;
#else
    uint8_t print = 0;
#endif
    uint8_t homogenous = 0;
    if (line_length_finder(file_name, line_length_max, print, &homogenous)) return 1;
    *line_length_max += 1; // just to be safe
#ifdef PRINT_INPUT_ANALYSIS
    printf("Max line length variable: %" PRId64 "\n", *line_length_max);
#endif

    int64_t elem_min = 0;
    int64_t elem_max = 0;
    uint64_t nmb_of_cols = 0;

    if (rowcol_cunt(file_name, nmb_of_lines, &nmb_of_cols, &elem_max, &elem_min, print)) return 1;

    return 0;
}
uint8_t main_execution(
    FILE *pf,
    char *line_input,
    char *character_part_input,
    uint64_t nmb_of_lines,
    uint64_t line_length_max
) {
    // trying to store all the lines as their own parts
    char checksum_input[6] = {0};
    char checksum_calc[6] = {0};
    enum search_part part_of_search = PART_NAME;

    // Seperate all lines, and handle them seperately
    uint64_t integer = 0;
    for (uint64_t line_ind = 0; line_ind < nmb_of_lines; line_ind++) {
        seperate_line(
            pf,
            line_input,
            character_part_input,
            nmb_of_lines,
            line_length_max,
            part_of_search,
            checksum_input,
            &integer
        );
        // printf("character_part_input: %s\n", character_part_input);
        // printf("checksum_input: %s\n", checksum_input);
    }
    fclose(pf); // it's ok to double close since we set NULL
    pf = NULL;

    return 0;
}
uint8_t seperate_line(
    FILE *pf,
    char *line_input,
    char *character_part_input,
    uint64_t nmb_of_lines,
    uint64_t line_length_max,
    enum search_part part_of_search,
    char *checksum_input,
    uint64_t *number

) {

    // resetting all parts to 0
    part_of_search = PART_NAME;
    memset(line_input, 0, line_length_max * sizeof(char));
    memset(character_part_input, 0, line_length_max * sizeof(char));
    memset(checksum_input, 0, 6 * sizeof(char));
    uint8_t char_input_ind = 0;
    uint8_t char_output_ind = 0;
    char c = 0;

    // getting the full line
    fgets(line_input, line_length_max, pf);
#ifdef PRINT_PARTS
    printf("%s", line_input);
#endif

    // 0 is character part, 1 is integer part, 2 is checksumpart

    while ((c = line_input[char_input_ind++]) != 0) {
        switch (part_of_search) {
        case (PART_NAME):
            if (c != '-') {
                if ((c < '0') || (c > '9')) {
                    character_part_input[char_output_ind++] = c;
                    // printf("%c", c);
                } else {
                    part_of_search = PART_SECTOR;
                    *number = c - 48; // ascii conversion hopefully
                }
            }
            break;
        case (PART_SECTOR):
            if (c != '[') {
                *number = *number * 10 + c - 48; // ascii conversion hopefully
            } else {
                part_of_search = PART_CHECKSUM;
                char_output_ind = 0;
            }
            break;
        case (PART_CHECKSUM):
            if (c != ']') {
                checksum_input[char_output_ind++] = c;
            } else {
                part_of_search = PART_DONE;
            }
            break;
        case (PART_DONE):
            break;
        }
    }
#ifdef PRINT_PARTS
    printf("\n");
    // trying to print the character part as a string
    printf("character_part_input: %s\n", character_part_input);

    // trying to print the number part as a number
    printf("number: %" PRIu64 "\n", *number);

    // trying to print the checksum
    printf("checksum_input: %s\n", checksum_input);
    printf("\n");
#endif

    return 0;
}
