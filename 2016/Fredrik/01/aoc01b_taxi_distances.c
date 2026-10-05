/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/01/aoc01b_taxi_distances.c
 * @date 2026-10-03
 * @brief finding taxidistance from drop
 */

// make the movestepper a function
// make the movestepper call function looking for duplicate

//// OPTIONS ////
// #define INPUTPATH "input/01a_input.txt"
#define INPUTPATH "input/01a_input_testcase.txt"

// comment/uncomment sets them
// #define PRINT_ANSWER_A
// #define PRINT_INPUT_ANALYSIS
// #define PRINT_TOKENS
// #define PRINT_TOKENS_FCN
#define PRINT_POSITIONS
// #define DEBUG_DUPLICATES
#define LIMIT_STEPS 1
// #define PRINT_DUPLICATE
#define ALLOC_MULTIPLIER 150

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
    Position *pos_crawler,
    Opcode *opcode,
    uint8_t *duplicate_found,
    uint64_t *final_answer1b,
    const uint64_t op_ind,
    Position *stop_array,
    uint64_t *stop_arr_ind
);
uint8_t crawl(
    Position *stop_array,
    uint64_t *stop_arr_ind,
    Position *pos,
    Position *pos_crawler,
    int8_t x_change,
    int8_t y_change,
    uint8_t *duplicate_found,
    uint64_t *final_answer1b
);

int main(void) {

    // initialize
    AllocHolder heap = {0};
    Position pos = {0};         // Start in origo, pos is the full-stepper
    pos.dir_index = 1;          // Start pointing north
    Position pos_crawler = {0}; // Start in origo, pos_crawler is the crawler
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
#ifndef ALLOC_MULTIPLIER // this really should check >= 1
    printf("ALLOC_MULTIPLIER needed\n");
    goto error;
#endif
    heap.stop_array = malloc((number_of_opcodes + 1) * ALLOC_MULTIPLIER * sizeof(Position));
    if (!heap.stop_array) goto error;
    Position *stop_array = heap.stop_array;
    stop_array[0] = pos;       // Adding origo since we append on new stop
    uint64_t stop_arr_ind = 1; // origo

    // execute all movements
    for (uint64_t i = 0; i < number_of_opcodes; i++) {
        move(&pos, &pos_crawler, &(opcode_array[i]), &duplicate_found, &final_answer1b, i, stop_array, &stop_arr_ind);
    }

    // do the taxidistance 1a and print answer 1b
    uint64_t final_answer1a = llabs(pos.x) + llabs(pos.y);
#ifdef PRINT_ANSWER_A
    printf("Answer 1a:% " PRId64 "\n", final_answer1a);
#endif
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
    Position *pos_crawler,
    Opcode *opcode,
    uint8_t *duplicate_found,
    uint64_t *final_answer1b,
    const uint64_t op_ind,
    Position *stop_array,
    uint64_t *stop_arr_ind
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
        if (!*duplicate_found) {
            while (stop_array[*stop_arr_ind].x != pos->x && stop_array[*stop_arr_ind].y != pos->y) {
                *stop_arr_ind = *stop_arr_ind + 1;
                pos_crawler->x += 1;
                stop_array[*stop_arr_ind].x = pos_crawler->x;
                stop_array[*stop_arr_ind].y = pos_crawler->y;
                if (!*duplicate_found) {
                    // DUPLICATE FCN Goes here
                    if (*duplicate_found) { // has to be double if, both have to be able to run, and dup can be fnd in while
                        *final_answer1b = llabs(pos_crawler->x) + llabs(pos_crawler->y);
                    }
                }
            }
        }
        // for (uint64_t i = *stop_arr_ind; i < nmb_elems_in_moved_stop_array; i++) {
        //     stop_array[i].x += 1;
        //     stop_array[i].y = pos_crawler->y;
        //     for (uint64_t j = 0; j < i; j++) {
        //         if (((stop_array[i]).x == (stop_array[j]).x) && ((stop_array[i]).y == (stop_array[j]).y)) {
        //             printf("DUP\n");
        //             printf("duplicate (x, y): (%" PRId64 ", %" PRId64 ")\n", (stop_array[i]).x, (stop_array[i]).x);
        //             *duplicate_found = 1;
        //             *final_answer1b = llabs(pos->x) + llabs(pos->y);
        //         }
        //     }
        // }
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
}

uint8_t crawl(
    Position *stop_array,
    uint64_t *stop_arr_ind,
    Position *pos,
    Position *pos_crawler,
    int8_t x_change,
    int8_t y_change,
    uint8_t *duplicate_found,
    uint64_t *final_answer1b
) {
    while (stop_array[*stop_arr_ind].x != pos->x && stop_array[*stop_arr_ind].y != pos->y) {
        *stop_arr_ind = *stop_arr_ind + 1;
        pos_crawler->x += x_change;
        pos_crawler->y += y_change;
        stop_array[*stop_arr_ind].x = pos_crawler->x;
        stop_array[*stop_arr_ind].y = pos_crawler->y;
        if (!*duplicate_found) {
            // DUPLICATE FCN Goes here
            if (*duplicate_found) { // has to be double if, both have to be able to run, and dup can be fnd in while
                *final_answer1b = llabs(pos_crawler->x) + llabs(pos_crawler->y);
            }
        }
    }
    return 0;
}
