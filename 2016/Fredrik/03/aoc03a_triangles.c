/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/03/aoc03a_triangles.c
 * @date 2026-10-17
 * @brief counting number of illegal triangles, only valid for uint16 input
 */

//// options ////
#define INPUTPATH "input/03_input_testcase.txt"
// #define INPUTPATH "input/03_input.txt"

// comment/uncomment sets them
// #define PRINT_INPUT_ANALYSIS
// #define PRINT_ROWS
// #define PRINT_ILLEGALS

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//// prototypes ////

/**
 * @brief verifying input is between 1 and UINT16_MAX and finds number of rows and columns
 * @param filepath relative path to inputfile
 * @param nmb_of_rows adress to pointeroutput
 * @param nmb_of_cols adress to pointeroutput
 * @return 1 for failure, 0 for success
 */
uint8_t startup(char *filepath, uint64_t *nmb_of_rows, uint64_t *nmb_of_cols);

/**
 * @brief transfers input to matrix, should not be relied upon as its the first iteration of rowcol_cunt.
 * @param file_name
 * @param matrix pointer
 * @param number_of_rows actual number
 * @param number_of_cols actual number
 * @return errorcode, 0 on success
 */
uint8_t rowcol_to_matrix_uint16(char *file_name, uint16_t *matrix, uint64_t number_of_rows, uint64_t number_of_cols);

/**
 * @brief Goes through full Xx3 Matrix and finds all illegal triangles
 * @param matrix adress to Xx3 matrix
 * @param nmb_of_rows the nmb of rows, X
 * @param nmb_of_illegals adress to pointeroutput
 * @return errorcode, 0 on success
 */
uint8_t execute_all_rows(uint16_t *matrix, uint64_t nmb_of_rows, uint64_t *nmb_of_illegals);

/**
 * @brief takes next 3 uint16 from adress to array and finds if they are illegal triangle, no checks for out of bounds
 * @param col_array adress to first number in array
 * @return 0 on legal, 1 on illegal
 */
uint8_t is_illegal(uint16_t *col_array);

int main(void) {

    // initializing
    uint8_t ret = 1;
    uint16_t *matrix = NULL;

    uint64_t nmb_of_rows = 0;
    uint64_t nmb_of_cols = 0;
    uint64_t nmb_of_illegals = 0;

    // verifying input is ok, and finding how many inputs
    if (startup(INPUTPATH, &nmb_of_rows, &nmb_of_cols)) goto out;

    // allocating
    matrix = calloc(nmb_of_rows * nmb_of_cols, sizeof(uint16_t));
    if (!matrix) {
        fprintf(stderr, "error allocating, exiting\n");
        goto out;
    }

    // transferring input to matrix
    if (rowcol_to_matrix_uint16(INPUTPATH, matrix, nmb_of_rows, nmb_of_cols)) goto out;

    // Main execution
    if (execute_all_rows(matrix, nmb_of_rows, &nmb_of_illegals)) goto out;

    // Exiting
    uint64_t answer_3a = nmb_of_rows - nmb_of_illegals;
    printf("Answer part a: %" PRIu64 "\n", answer_3a);
    // printf("prgrm ran thru, no prob\n");
    ret = 0;

out:
    free(matrix);
    return ret;
}

uint8_t startup(char *filepath, uint64_t *nmb_of_rows, uint64_t *nmb_of_cols) {

    int64_t elem_min = 0;
    int64_t elem_max = 0;

#ifdef PRINT_INPUT_ANALYSIS
    uint8_t print = 1;
#else
    uint8_t print = 0;
#endif

    if (rowcol_cunt(filepath, nmb_of_rows, nmb_of_cols, &elem_max, &elem_min, print)) return 1;

    if (elem_min <= 0) {
        fprintf(stderr, "Triangle side has to be bigger than 0, exiting\n");
        return 1;
    }
    if (elem_max > UINT16_MAX) {
        fprintf(stderr, "Triangle side larger than UINT16_MAX, out of program scope, exiting\n");
        return 1;
    }

    return 0;
}

uint8_t rowcol_to_matrix_uint16(char *file_name, uint16_t *matrix, uint64_t number_of_rows, uint64_t number_of_cols) {
    const uint32_t MAX_LINE_LENGTH = 4096; // POSIX Standard

    FILE *pf;
    char *input_file = file_name;
    pf = fopen(input_file, "r");
    if (!pf) {
        printf("Something wrong with file-opening in rowcol_to_matrix, mayby "
               "wrong file-name?");
        fclose(pf);

        return 1;
    }
    char *line;
    line = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (!line) {
        printf(
            "Issues finding memory for a line, terminating rowcol_to_matrix"
        );
        fclose(pf);
        free(line);

        return 1;
    }
    uint64_t col;
    for (uint64_t row = 0; row < number_of_rows; row++) {
        // printf("The string: %s\n",line);
        fgets(line, MAX_LINE_LENGTH - 1, pf);
        // printf("The string: %s\n",line);
        char *token = strtok(
            line,
            ", \t\n"
        ); // Use " \t" to split by both space and tab, also ','
        col = 0;
        while (token != NULL) {
            if (*token != '\0' && *token != ' ' && *token != '\t' &&
                *token != ',' && *token != '\n' && *token != EOF) {
                // printf("The token as string: %s\n",token);
                uint16_t element = (uint16_t)atol(token);
                // printf("The token as element: %d\n",element);
                matrix[row * number_of_cols + col] = element;
                col++;
            }
            token = strtok(NULL, ", \t"); // Continue with the next token
        }
    }
    fclose(pf);
    free(line);

    return 0;
}

uint8_t execute_all_rows(uint16_t *matrix, uint64_t nmb_of_rows, uint64_t *nmb_of_illegals) {

    for (uint64_t row_ind = 0; row_ind < nmb_of_rows; row_ind++) {
        if (is_illegal(&matrix[row_ind * 3])) {      // *3 bc 3 columns
            *nmb_of_illegals = *nmb_of_illegals + 1; // Avoiding ++ on pointers
        }
    }
    return 0;
}

uint8_t is_illegal(uint16_t *col_array) {
#ifdef PRINT_ROWS
    printf("row: %" PRId16 " %" PRId16 " %" PRId16 "\n", col_array[0], col_array[1], col_array[2]); // Now thats a mouthful
#endif

    uint8_t ind_of_biggest = 0;
    uint32_t sum = col_array[0];
    for (uint8_t col_ind = 1; col_ind < 3; col_ind++) { // starts at 1 bc ind=0 is baseline. Also mayby stupid forloop?
        sum += col_array[col_ind];
        if (col_array[col_ind] > col_array[ind_of_biggest]) ind_of_biggest = col_ind;
    }
    if (col_array[ind_of_biggest] >= sum - col_array[ind_of_biggest]) {
#ifdef PRINT_ILLEGALS
        printf("illegal: %" PRId16 " %" PRId16 " %" PRId16 "\n", col_array[0], col_array[1], col_array[2]); // Now thats a mouthful
#endif
        return 1;
    } else {
        return 0;
    }
}
