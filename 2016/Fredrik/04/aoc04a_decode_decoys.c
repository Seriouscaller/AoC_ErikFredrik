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

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//// prototypes ////

uint8_t startup(const char *file_name, uint64_t *line_length_max, uint64_t *nmb_of_lines);

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

    // trying to print all the lines as their own parts
    char checksum_input[6] = {0};
    char checksum_calc[6] = {0};
    uint8_t char_input_ind = 0;
    uint8_t char_output_ind = 0;
    char c = 0;

    for (uint64_t line_ind = 0; line_ind < nmb_of_lines; line_ind++) {

        // resetting all parts to 0
        memset(line_input, 0, line_length_max * sizeof(char));
        memset(character_part_input, 0, line_length_max * sizeof(char));
        memset(checksum_input, 0, 6 * sizeof(char));
        memset(checksum_calc, 0, 6 * sizeof(char));
        char_input_ind = 0;
        char_output_ind = 0;
        c = 0;

        // getting the full line
        fgets(line_input, line_length_max, pf);
        printf("%s", line_input);

        // trying to seperate its parts
        // 0 is character part, 1 is integer part, 2 is checksumpart
        uint8_t part_of_search = 0;
        uint64_t number = 0;
        while ((c = line_input[char_input_ind++]) != 0) {
            if (part_of_search == 0) {
                if (c != '-') {
                    if ((c < '0') || (c > '9')) {
                        character_part_input[char_output_ind++] = c;
                        // printf("%c", c);
                    } else {
                        part_of_search = 1;
                        number = c - 48; // ascii conversion hopefully
                    }
                }
            } else if (part_of_search == 1) {
                if (c != '[') {
                    number = number * 10 + c - 48; // ascii conversion hopefully
                } else {
                    part_of_search = 2;
                    char_output_ind = 0;
                }
            } else if (part_of_search == 2) {
                if (c != ']') {
                    checksum_input[char_output_ind++] = c;
                } else {
                    part_of_search = 3;
                }
            }
        }
        printf("\n");
        // trying to print the character part as a string
        printf("character_part_input: %s\n", character_part_input);

        // trying to print the number part as a number
        printf("number: %" PRIu64 "\n", number);

        // trying to print the checksum
        printf("checksum_input: %s\n", checksum_input);
        printf("\n");
    }

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
