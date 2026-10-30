/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/04/aoc04a_decode_decoys.c
 * @date 2026-10-18
 * @brief decoding valid rooms from decoys using checksum
 */

//// options ////
// #define INPUTPATH "input/04_input_testcase.txt"
#define INPUTPATH "input/04_input.txt"

#define ALPHABET_LENGTH 30

// comment/uncomment sets them
// #define PRINT_INPUT_ANALYSIS
// #define PRINT_PARTS
// #define PRINT_CSUM_UNSORTED
// #define PRINT_CSUM_SORTED
// #define PRINT_CSUM_CALC
#define PRINT_OUTPUT_B

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//// structs ////

typedef struct {
    char c;
    uint8_t occurence;
} CharOccur;

//// prototypes ////

/**
 * @brief Starting up program, verifiying input, and finding how many inputs and max length of lines
 * @param file_name
 * @param line_length_max
 * @param nmb_of_lines
 * @return errorcode
 */
uint8_t startup(const char *file_name, uint64_t *line_length_max, uint64_t *nmb_of_lines);

/**
 * @brief The real execution of program
 * @param pf
 * @param line_input
 * @param character_part_input
 * @param nmb_of_lines
 * @param line_length_max
 * @param finalanswer_a
 * @return error code
 */
uint8_t main_execution(
    FILE *pf,
    char *line_input,
    char *character_part_input,
    uint64_t nmb_of_lines,
    uint64_t line_length_max,
    uint64_t *finalanswer_a
);

/**
 * @brief seperates the lines of input into strings and an integer for the number part
 * @param pf
 * @param line_input
 * @param character_part_input
 * @param line_length_max
 * @param checksum_input
 * @param number
 * @return errorcode
 */
uint8_t seperate_line(
    FILE *pf,
    char *line_input,
    char *character_part_input,
    uint64_t line_length_max,
    char *checksum_input,
    uint64_t *number

);

/**
 * @brief calculates the checksum from the given input
 * @param checksum_calc
 * @param character_part_input
 * @return errorcode
 */
uint8_t calculate_checksum(char *checksum_calc, char *character_part_input);

/**
 * @brief Sorting function for qsort
 * @param pa pointer to CharOccur 1
 * @param pb pointer to CharOccur 2
 * @return
 */
int character_sort(const void *pa, const void *pb);

/**
 * @brief Finds if the character is stored in the array of CharOccur
 * @param charTested
 * @param length_of_array_nmb number of CharOccur in the array
 * @param charOccurArray
 * @return 1 / 0 true / false
 */
uint8_t is_in_array(char charTested, uint8_t length_of_array_nmb, CharOccur *charOccurArray);

/**
 * @brief Finds the index of a given character in an array of CharOccur, needs it to be in there, otherwise returns 0
 * @param charTested
 * @param length_of_array_nmb
 * @param charOccurArray
 * @return index if its in there, otherwise 0
 */
uint8_t index_of_char(char charTested, uint8_t length_of_array_nmb, CharOccur *charOccurArray);

/**
 * @brief Prints the array of CharOccur to stdout
 * @param length_of_array_nmb
 * @param charOccurArray
 */
void print_occur(uint8_t length_of_array_nmb, CharOccur *charOccurArray);

/**
 * @brief Checks if two strings of length 5 are the same
 * @param checksum_calc
 * @param checksum_input
 * @return 1 if they are the same, 0 otherwise
 */
uint8_t checksums_are_equal(char *checksum_calc, char *checksum_input);

uint8_t decode(char *character_part_input, uint64_t integer);

char caesar(char c, uint64_t integer);

int main(void) {

    // initializing
    uint8_t ret = 1;
    char *line_input = NULL;
    char *character_part_input = NULL;
    FILE *pf = NULL;

    uint64_t line_length_max = 0;
    uint64_t nmb_of_lines = 0;
    uint64_t finalanswer_a = 0;

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
    if (main_execution(pf, line_input, character_part_input, nmb_of_lines, line_length_max, &finalanswer_a)) goto out;

    // Exiting
    // printf("\nprgrm ran thru, no prob\n");
    printf("Final answer part a: %" PRIu64 " \n", finalanswer_a);
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
    uint64_t line_length_max,
    uint64_t *finalanswer_a
) {

    // trying to store all the lines as their own parts
    char checksum_input[6] = {0};
    char checksum_calc[6] = {0};

    // Seperate all lines, and handle them seperately
    uint64_t integer = 0;
    for (uint64_t line_ind = 0; line_ind < nmb_of_lines; line_ind++) {
        seperate_line(
            pf,
            line_input,
            character_part_input,
            line_length_max,
            checksum_input,
            &integer
        );
        calculate_checksum(checksum_calc, character_part_input);

        if (checksums_are_equal(checksum_calc, checksum_input)) {
            *finalanswer_a += integer;
            decode(character_part_input, integer);
        }
    }

    // fclose(pf); // it's ok to double close since we set NULL
    // pf = NULL;

    return 0;
}
uint8_t seperate_line(
    FILE *pf,
    char *line_input,
    char *character_part_input,
    uint64_t line_length_max,
    char *checksum_input,
    uint64_t *number
) {

    // resetting all parts to 0
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
    enum search_part {
        PART_NAME,     // Character part
        PART_SECTOR,   // Integer part
        PART_CHECKSUM, // Checksum part
        PART_DONE      // after ]
    };
    enum search_part part_of_search = PART_NAME;

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

uint8_t calculate_checksum(char *checksum_calc, char *character_part_input) {

    // First we just map all the characters used, and how many times they occur
    memset(checksum_calc, 0, 6 * sizeof(char)); // checksum_calc = {0};
    char c = 0;
    CharOccur checksum_calc_big[ALPHABET_LENGTH] = {0}; // Array of characters found

    uint8_t i_in = 0;         // index of read characters from input
    uint8_t i_big_setter = 0; // nmb of stored CharOccur
    while (character_part_input[i_in] != 0) {
        c = character_part_input[i_in];
        if (is_in_array(c, i_big_setter + 1, checksum_calc_big)) {
            checksum_calc_big[index_of_char(c, i_big_setter, checksum_calc_big)].occurence += 1;
        } else {
            checksum_calc_big[i_big_setter].c = c;
            checksum_calc_big[i_big_setter++].occurence = 1;
        }
        i_in++;
    }

#ifdef PRINT_CSUM_UNSORTED
    printf("Printing them unsorted: \n");
    print_occur(i_big_setter, checksum_calc_big);
#endif

    // Then we sort
    qsort(checksum_calc_big, i_big_setter, sizeof(CharOccur), character_sort);

#ifdef PRINT_CSUM_SORTED
    printf("Printing them sorted: \n");
    print_occur(i_big_setter, checksum_calc_big);
#endif

    // Then we return the calculated checksum
    for (uint8_t i = 0; i < 5; i++) { // 5 for checksum length
        checksum_calc[i] = checksum_calc_big[i].c;
    }

    return 0;
}

int character_sort(const void *pa, const void *pb) {

    const CharOccur *a = pa;
    const CharOccur *b = pb;

    if (a->occurence > b->occurence) return -1;
    if (b->occurence > a->occurence) return 1;
    if (a->c > b->c)
        return 1; // this might be other way round
    else if (a->c < b->c) {
        return -1;
    } else {
        return 0;
    }
} // for getting the syntax working i needed ai...

uint8_t is_in_array(char charTested, uint8_t length_of_array_nmb, CharOccur *charOccurArray) {
    // no testing for out of bounds, happycoding
    for (uint8_t i = 0; i < length_of_array_nmb; i++) {
        if (charTested == charOccurArray[i].c) return 1;
    }
    return 0;
} // would have been better to just return the index straight away, not changing, happycoding

uint8_t index_of_char(char charTested, uint8_t length_of_array_nmb, CharOccur *charOccurArray) {
    for (uint8_t i = 0; i < length_of_array_nmb; i++) {
        if (charTested == charOccurArray[i].c) {
            return i;
        }
    }
    printf("index was not found, should be impossible, no errorhandling for this fcn though\n"); // happy-coding
    return 0;
}

void print_occur(uint8_t length_of_array_nmb, CharOccur *charOccurArray) {
    for (uint8_t i = 0; i < length_of_array_nmb; i++) {
        printf("character: %c | %" PRIu8 "\n", charOccurArray[i].c, charOccurArray[i].occurence);
    }
}

uint8_t checksums_are_equal(char *checksum_calc, char *checksum_input) {
#ifdef PRINT_CSUM_CALC
    printf("%s\n", checksum_calc);
#endif
    for (uint8_t i = 0; i < 5; i++) { // 5 for checksumlength
        if (checksum_calc[i] != checksum_input[i]) {
            return 0;
        }
    }
    return 1;
}

uint8_t decode(char *character_part_input, uint64_t integer) {
    // printf("%s\n", character_part_input);

    // Making an outputstring, and i cant be bothered to allocate it and send it all the way down here...
    char output_string[2 * ALPHABET_LENGTH] = {0}; // This is not good enough but i remember all lines were smaller than this...

    //- go through each character in input until we hit 0
    char c;
    uint8_t i = 0;
    while (1) { // This probably could be written nicer as a for-loop
        c = character_part_input[i];
        if (c == 0) break;
        // printf("%c\n", c);
        if (c == '-') {
            output_string[i] = ' ';
        } else {
            output_string[i] = caesar(c, integer);
        }
        i++;
    }

#ifdef PRINT_OUTPUT_B
    if (output_string[0] == 'n') {
        printf("%s %" PRIu64 "\n", output_string, integer);
    }
#endif
    return 0;
}

char caesar(char c, uint64_t integer) {
    for (uint64_t i = 0; i < integer; i++) {
        c++;
        if (c > 'z') {
            c -= 26; // real english alphabet length
        }
    }
    return c;
}
