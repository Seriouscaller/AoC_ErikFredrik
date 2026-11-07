#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day09/a/day09_input_sample.txt"
// #define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day09/a/day09_input_full.txt"
#define NEWLINE '\n'
#define WHITE_SPACE ' '
#define MAX_LINE_LENGTH 3000
#define MAX_MARKERS_IN_ARRAY 10
#define MAX_STRING_LENGTH_TO_DUPLICATE 200
#define MAX_MARKER_LENGTH 8
#define MAX_MARKER_STRING_LENGTH (MAX_MARKER_LENGTH + 1)

enum RETURNCODE
{
    OK = 0,
    ERROR = -1,
};

enum ACTION
{
    MARKER_START = 0,
    MARKER_END = 1,
    DECRYPTED_VALUE = 2,
};

struct marker
{
    char argv[3][10];
    char marker_string[MAX_MARKER_STRING_LENGTH];
    char duplication_string[MAX_STRING_LENGTH_TO_DUPLICATE];
    uint8_t argc;
    uint16_t length;
    uint16_t range;
    uint16_t number_of_repeats;
    uint64_t global_start_index;
    uint64_t global_end_index;
};

/*  PROTOTYPES  */
bool is_numeric(char *str, uint8_t length);
uint8_t determine_action(char ch);
bool iterate_to_find_marker_end(char *puzzle_string, uint64_t file_size, uint64_t global_index, uint16_t *local_index);
void extract_marker(struct marker *marker);

bool is_numeric(char *str, uint8_t length)
{
    bool is_numeric = true;
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(str[i]))
        {
            is_numeric = false;
        }
    }
    return is_numeric;
}

uint8_t determine_action(char ch)
{
    switch (ch)
    {
    case '(':
        return MARKER_START;
    case ')':
        return MARKER_END;
    case WHITE_SPACE:
        return MARKER_END;
    default:
        return DECRYPTED_VALUE;
    }
};

bool iterate_to_find_marker_end(char *puzzle_string, uint64_t file_size, uint64_t global_index, uint16_t *local_index)
{
    if (global_index + MAX_MARKER_LENGTH > file_size)
    {
        printf("Index outside of filebounds!\n");
        return false;
    }

    for (int i = 0; i < MAX_MARKER_LENGTH; i++)
    {
        if (puzzle_string[global_index + i] == ')')
        {
            // Found end of marker!
            *local_index = i;
            return true;
        }
    }
    return false;
};

void extract_marker(struct marker *marker)
{
    char marker_string_cpy[MAX_MARKER_STRING_LENGTH];
    memset(marker_string_cpy, '\0', sizeof(char) * MAX_MARKER_STRING_LENGTH);
    strncpy(marker_string_cpy, marker->marker_string, strlen(marker->marker_string));

    char *token = strtok(marker_string_cpy, "x");
    if (token == NULL)
    {
        printf("Failed to extract marker!\n");
        return;
    }

    while (token != NULL)
    {
        // printf("token: %s ", token);
        uint8_t argument_index = marker->argc;
        strncpy(marker->argv[argument_index], token, strlen(token));

        if (!is_numeric(token, strlen(token)))
        {
            printf("Failed to convert number: %s%c", token, NEWLINE);
            continue;
        }
        uint16_t number = atoi(token);
        if (0 < number)
            marker->argc++;

        if (1 == marker->argc)
            marker->range = number;

        if (2 == marker->argc && 0 < number)
            marker->number_of_repeats = number;

        token = strtok(NULL, "x");
    }
};

void print_marker(struct marker *marker)
{
    printf("Marker (%sx%s) Repeat %u times %s%c",
           marker->argv[0], marker->argv[1], marker->number_of_repeats, marker->duplication_string, NEWLINE);
}

int main(int argc, char *argv[])
{
    struct file_information file_info = get_file_information(INPUT_FILE_NAME);
    char *puzzle_string = allocate(file_info.file_size);

    if (file_info.rows == 0 || file_info.file_size == 0)
    {
        printf("Unable to acquire file information!");
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }

    if (file_info.max_columns > MAX_LINE_LENGTH)
    {
        printf("Puzzleline too long! Unable to fit inside string-array!");
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }

    int ret = read_file(INPUT_FILE_NAME, puzzle_string);
    if (ret == 1)
    {
        printf("Unable to open file!\n");
        free_allocation(puzzle_string);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("File read succesfully! Size: %lu Rows: %lu Columns: %lu\n", file_info.file_size, file_info.rows, file_info.max_columns);
    }

    struct marker markers[MAX_MARKERS_IN_ARRAY] = {'\0'};
    uint16_t number_of_markers_stored = 0;

    /* ### MAIN LOOP ### */
    uint32_t length_decrypted_data = 0;
    uint64_t global_index = 0;
    bool found_start_marker = false, found_end_marker = false, is_marker_ready = false;
    char ch;
    while (global_index < file_info.file_size)
    {
        ch = puzzle_string[global_index];
        uint8_t action = determine_action(ch);

        if (action == MARKER_START)
        {
            found_start_marker = true;
        }
        uint16_t local_index = 0;
        while (found_start_marker)
        {
            struct marker marker = {'\0'};

            // Markers are max 8 chars (MMMxNNN)
            bool is_end_found = iterate_to_find_marker_end(puzzle_string, file_info.file_size, global_index, &local_index);

            if (is_end_found)
            {
                marker.global_start_index = global_index;
                marker.global_end_index = global_index + local_index;
                marker.length = local_index + 1;

                strncpy(marker.marker_string, &puzzle_string[global_index + 1], marker.length - 2);

                // Parse marker to extract info
                extract_marker(&marker);
                global_index = global_index + marker.length;
            }
            markers[number_of_markers_stored++] = marker;
            found_start_marker = false, is_end_found = false, is_marker_ready = true;
        }

        if (is_marker_ready)
        {
            // Deal with the marker in the globalstring
            uint16_t m = markers[number_of_markers_stored - 1].number_of_repeats;
            uint16_t n = markers[number_of_markers_stored - 1].range;

            // Create a string large enough for duplication
            char string[m * n + 1];
            memset(string, '\0', m * n * sizeof(char) + 1);

            // Pattern repetition
            uint16_t string_index = 0;
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    string[string_index++] = puzzle_string[global_index + j];
                }
                // Save the repeating sequence into the marker array
                if (i == 0)
                {
                    strncpy(markers[number_of_markers_stored - 1].duplication_string, string, strlen(string));
                }
            }

            global_index += markers[number_of_markers_stored - 1].range;
            is_marker_ready = false;
            length_decrypted_data += strlen(string);
            print_marker(&markers[number_of_markers_stored - 1]);
            continue;
        }

        // Already decrypted data -> count letter
        if (action == DECRYPTED_VALUE)
        {
            length_decrypted_data++;
            printf("%llu: %c%c", length_decrypted_data, puzzle_string[global_index], NEWLINE);
        }
        global_index++;
    }

    free_allocation(puzzle_string);
    return 0;
}
/*
A(1x5)BC
ABBBBBC

(3x3)XYZ
XYZXYZXYZ

A(2x2)BCD(2x2)EFG
ABCBCDEFEFG

(6x1)(1x3)A
(1x3)A

X(8x2)(3x3)ABCY
X(3x3)ABC(3x3)ABCY

#### Eriks PLAN ####
(MxN)
uint32_t length_decrypted_data
Keep counting (no whitespace) the length parsed so far of decrypted data

IF character is '('
    EXTRACT MARKER
    Found valid marker!
    ELSE keep counting

Parse marker
    Select as many characters (M) as the marker says
    Duplicate the string (N) number of times
    add string length to length_decrypted_data

## Functions
 - Function to dynamically allocate memory for the decrypted string
 - Is Valid Marker?
    Check if it has a closing parenthesis, an 'x', and the rest is numerical
 -
*/