/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/01/aoc01a_taxi_distances.c
 * @date 2026-10-03
 * @brief finding taxidistance from drop
 */

//// OPTIONS ////
// #define INPUTPATH "input/01a_input.txt"
#define INPUTPATH "input/01a_input_testcase.txt"

// comment/uncomment sets them
// #define PRINT_INPUT_ANALYSIS
// #define PRINT_TOKENS
// #define PRINT_TOKENS_FCN
// #define PRINT_POSITIONS
// #define DEBUG_DUPLICATES
// #define LIMIT_STEPS 5
// #define PRINT_DUPLICATE
#define ALLOC_MULTIPLIER 1000

//// imports ////
#include "../lib/frallfiles.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//// structs ////
typedef struct {
    int64_t x;
    int64_t y;
    int64_t dir_index;
} Position;

typedef struct {
    int8_t turn; // -1 or 1
    uint64_t steps;
} Opcode;

typedef struct {
    // holds all allocated memory, all pointers of course
    Opcode *opcode_array;
    Position *stop_array;
} AllocHolder;

//// prototypes ////
void free_alloc(AllocHolder *heap);
int8_t direction_from_index(int64_t ind);
uint8_t analyze_input(const char *inputpath, uint64_t *number_of_opcodes);
uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
); // imported
uint8_t fill_array(const char *inputpath, Opcode *opcode_array);
uint8_t set_opcode(const uint64_t i, const char *token, Opcode *opcode_array);
void move(
    Position *pos,
    Opcode *opcode,
    uint8_t *duplicate_found,
    uint64_t *final_answer1b,
    const uint64_t op_ind,
    Position *stop_array,
    uint64_t *coord_ind
);
void second_stop_check(
    Position *pos,
    uint8_t *duplicate_found,
    const uint64_t op_ind,
    Position *stop_array,
    uint64_t *final_answer1b,
    uint64_t *coord_ind,
    uint64_t steps
);

int main(void) {

    // initialize
    AllocHolder heap = {0};
    Position pos = {0}; // Start in origo
    pos.dir_index = 1;  // Start pointing north
    uint64_t final_answer1b = 0;
    uint8_t duplicate_found = 0;

    // analyze input
    char *inputpath = INPUTPATH;
    uint64_t number_of_opcodes;
    if (analyze_input(inputpath, &number_of_opcodes)) goto error;

    // transform input to opcodes:
    heap.opcode_array = malloc(number_of_opcodes * sizeof(Opcode));
    if (!heap.opcode_array) goto error;
    Opcode *opcode_array = heap.opcode_array;
    if (fill_array(inputpath, opcode_array)) goto error;

// allocate for stops
#ifndef ALLOC_MULTIPLIER
    printf("ALLOC_MULTIPLIER needed\n");
    goto error;
#endif
    heap.stop_array = malloc((number_of_opcodes + 1) * ALLOC_MULTIPLIER * sizeof(Position));
    if (!heap.stop_array) goto error;
    Position *stop_array = heap.stop_array;
    stop_array[0] = pos; // Adding origo since we append on new stop

    // execute all movements
    uint64_t coord_ind = 0;
    for (uint64_t i = 0; i < number_of_opcodes; i++) {
        move(&pos, &(opcode_array[i]), &duplicate_found, &final_answer1b, i, stop_array, &coord_ind);
    }

    // do the taxidistance 1a and print answer 1b
    uint64_t final_answer1a = llabs(pos.x) + llabs(pos.y);
    printf("Answer 1a:% " PRId64 "\n", final_answer1a);
    if (duplicate_found)
        printf("Answer 1b: %" PRId64 "\n", final_answer1b);
    else
        printf("No HQ found\n");
    printf("\n");

    goto clean_exit;

error:
    free_alloc(&heap);
    return 1;

clean_exit:
    free_alloc(&heap);
    printf("main ran thru no problems\n");
    return 0;
}

void free_alloc(AllocHolder *heap) {

    if (!heap) return;
    free(heap->opcode_array);
    free(heap->stop_array);
    // all other frees here
    memset(heap, 0, sizeof(AllocHolder));
}

int8_t direction_from_index(int64_t ind) {
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

uint8_t fill_array(const char *inputpath, Opcode *opcode_array) {
    // This is a modified rowcolcunt that just takes every TOKEN and

    const uint64_t ONELINE_MAX = 4096; // POSIX Standard

    // Finding max length of line
    uint64_t max_line_length;
    uint8_t homogenous_line_length;
    if (line_length_finder(inputpath, &max_line_length, 0, &homogenous_line_length)) {
        return 1;
    }
    if (ONELINE_MAX <= max_line_length) {
        printf("fill_array exits because line-length is to long for fgets\n");
        return 1;
    }
    max_line_length += 2; // Just to make sure we don't break bounds
    FILE *pf;
    pf = fopen(inputpath, "r");
    if (!pf) {
        printf("Something wrong with file-opening in fill_array, mayby wrong file-name?");

        return 1;
    }

    const char *delims = ", \t\r\n";
    char *line;
    line = (char *)malloc(sizeof(char) * (max_line_length + 1));
    if (!line) {
        printf("Issues finding memory for a line, terminating fill_array");
        fclose(pf);

        return 1;
    }
    char *token;
    fgets(line, max_line_length - 1, pf);

    token = strtok(line, delims);
    uint64_t i = 0;
    while (token != NULL) {
#ifdef PRINT_TOKENS
        printf("The token: %s\n", token);
#endif
        if (set_opcode(i++, token, opcode_array)) {
            fclose(pf);
            free(line);
            return 1;
        }
        token = strtok(NULL, delims); // Continue with the next token
    }

    fclose(pf);
    free(line);
    return 0;
}

uint8_t set_opcode(const uint64_t i, const char *token, Opcode *opcode_array) {
#ifdef PRINT_TOKENS_FCN
    printf("The token: %s\n", token);
#endif
    Opcode temp_opcode = {0};
    switch (token[0]) {
    default: {
        printf("Invalid token sent to set_opcode, terminating program\n");
        return 1;
    }
    case 'R': {
        temp_opcode.turn = -1;
        break;
    }
    case 'L': {
        temp_opcode.turn = 1;
        break;
    }
    }
    temp_opcode.steps = strtoull(token + 1, NULL, 10);
#ifdef PRINT_TOKENS_FCN
    printf("The turn: %" PRId8 "\n", temp_opcode.turn);
    printf("The steps: %" PRIu64 "\n", temp_opcode.steps);
#endif
    opcode_array[i] = temp_opcode;

    return 0;
}

void move(
    Position *pos,
    Opcode *opcode,
    uint8_t *duplicate_found,
    uint64_t *final_answer1b,
    const uint64_t op_ind,
    Position *stop_array,
    uint64_t *coord_ind
) {
#ifdef PRINT_POSITIONS
    printf("Old (x, y): (%" PRId64 ", %" PRId64 ")\n", pos->x, pos->y);
#endif
#ifdef LIMIT_STEPS
    if (op_ind > LIMIT_STEPS) return;
#endif
    pos->dir_index += opcode->turn;
    switch (direction_from_index(pos->dir_index)) {
    default:
        printf("Invalid direction, can't terminate program\n");
        break;
    case 0: { // East
        pos->x += opcode->steps;
        break;
    }
    case 1: { // North
        pos->y += opcode->steps;
        break;
    }
    case 2: { // West
        pos->x -= opcode->steps;
        break;
    }
    case 3: { // South
        pos->y -= opcode->steps;
        break;
    }
    }
#ifdef PRINT_POSITIONS
    printf("New (x, y): (%" PRId64 ", %" PRId64 ")\n", pos->x, pos->y);
#endif
    if (!(*duplicate_found)) second_stop_check(pos, duplicate_found, op_ind, stop_array, final_answer1b, coord_ind, opcode->steps);
}

void second_stop_check(
    Position *pos,
    uint8_t *duplicate_found,
    const uint64_t op_ind,
    Position *stop_array,
    uint64_t *final_answer1b,
    uint64_t *coord_ind,
    uint64_t steps
) {
    const uint64_t nmb_elems_in_stop_array = *coord_ind + steps;
#ifdef DEBUG_DUPLICATES
    printf("\n");
    printf("analyzing (x, y): (%" PRId64 ", %" PRId64 ")\n", pos->x, pos->y);
#endif
    for (uint64_t i = 0; i < nmb_elems_in_stop_array; i++) {
#ifdef DEBUG_DUPLICATES
        printf("(x-pos, x-stoparr): (%" PRId64 ", %" PRId64 ")\n", pos->x, stop_array[i].x);
#endif
        if ((pos->x == (stop_array[i]).x) && (pos->y == (stop_array[i]).y)) {
#ifdef PRINT_DUPLICATE
            printf("duplicate (x, y): (%" PRId64 ", %" PRId64 ")\n", pos->x, pos->y);
#endif
            *duplicate_found = 1;
            *final_answer1b = llabs(pos->x) + llabs(pos->y);
        }
    }
    stop_array[op_ind + 1].x = pos->x;
    stop_array[op_ind + 1].y = pos->y;
    stop_array[op_ind + 1].dir_index = pos->dir_index;
}
