/**
 * @file /home/grisen/projects/advent_of_code/2016/AoC_ErikFredrik/2016/Fredrik/05/aoc05a_hash.c
 * @date 2026-10-30
 * @brief Writing a hash and then finding the correct character, really probably only works on linux, as printf, md5sum and cut are needed programs, and cut needs cli with arguments.
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

// Settings
#define INPUTPATH "input/05_input_testcase.txt"
// #define INPUTPATH "input/05_input.txt"
#define STARTING_INDEX 3230000
#define MAX_PROMPTS 10000
// #define PRINT_HASHES

// Constants
#define HASH_MAX_LENGTH 64
#define TERMINAL_COMMAND_MAX_LENGTH 256
#define INPUT_MAX_LENGTH 32

uint8_t startup(char *filename, char *input);
uint8_t hash_from_string(char *string, char *hash);

int main(void) {
    int ret = 1;
    char hash[HASH_MAX_LENGTH] = {0};
    char input[INPUT_MAX_LENGTH] = {0};
    char string_to_test[INPUT_MAX_LENGTH] = {0};
    char integer_as_string[INPUT_MAX_LENGTH] = {0};
    char answer_a[8] = {0};
    if (startup(INPUTPATH, &input[0])) goto out;

    printf("input is: %s and no newline\n", input);

    uint8_t answer_ind = 0;
    for (uint64_t iteration_ind = STARTING_INDEX; iteration_ind < STARTING_INDEX + MAX_PROMPTS; iteration_ind++) {
        strcpy(string_to_test, input);
        snprintf(integer_as_string, INPUT_MAX_LENGTH * sizeof(char), "%" PRIu64, iteration_ind); // Converting integer to str
        strcat(string_to_test, integer_as_string);                                               // conCatting on the integer
        if (hash_from_string(string_to_test, hash)) goto out;
        if (hash[0] == '0' && hash[1] == '0' && hash[2] == '0' && hash[3] == '0' && hash[4] == '0') {
            answer_a[answer_ind++] = hash[5];
            if (answer_ind == 7) break;
        }
#ifdef PRINT_HASHES
        printf("hash: %s\n", hash);
#endif
    }

    printf("Answer part a: %s\n", answer_a);

out:
    return ret;
}

uint8_t hash_from_string(char *string, char *hash) {
    FILE *pf = NULL;
    uint8_t ret = 1;
    memset(hash, 0, HASH_MAX_LENGTH * sizeof(char));

    char cmd[TERMINAL_COMMAND_MAX_LENGTH] = {0};
    const char *part1 = "printf '%s' '";
    // char *string = "abc3231929"; //this comes from caller now
    const char *part2 = "' | md5sum | cut -d' ' -f1";

    if (strlen(part1) + strlen(string) + strlen(part2) >= sizeof cmd) {
        fprintf(stderr, "command too long\n");
        goto out;
    }

    strcpy(cmd, part1);  /* cmd now holds part1 */
    strcat(cmd, string); /* append the data */
    strcat(cmd, part2);  /* append the rest */

    pf = popen(cmd, "r");

    if (pf == NULL) {
        fprintf(stderr, "error in popen\n");
        goto out;
    }

    uint8_t hash_ind = 0;
    int c;
    while ((c = getc(pf)) != '\n' && c != EOF) {
        hash[hash_ind++] = (char)c;
    }
    hash[hash_ind++] = 0;

    ret = 0;

out:
    if (pf != NULL) pclose(pf);
    return ret;
}

uint8_t startup(char *filename, char *input) {
    uint8_t ret = 1;
    FILE *pf = NULL;

    pf = fopen(filename, "r");
    if (pf == NULL) {
        fprintf(stderr, "error in fileopening, perhaps wrong path?\n");
        goto out;
    }
    // fgets(input, INPUT_MAX_LENGTH, pf);

    uint8_t input_ind = 0;
    int c;
    while ((c = getc(pf)) != '\n' && c != EOF) {
        input[input_ind++] = (char)c;
    }
    input[input_ind++] = 0;

    ret = 0;

out:
    if (pf != NULL) fclose(pf);
    return ret;
}
