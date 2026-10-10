/**
 * @file /home/grisen/programming/c/frallfiles.c
 * @date 2026-09-28
 * @brief A library of useful fuctions, some are old and not great, but tested.
 */

#include "frallfiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t line_length_finder(const char *file_name, uint64_t *line_length, const uint8_t print, uint8_t *homogenous) {
    FILE *pf;
    pf = fopen(file_name, "r");
    if (!pf) {
        printf("Something wrong with file-opening in line_length-finder, mayby wrong file-name?");

        return 1;
    }
    uint64_t line_min = UINT64_MAX; // We all know we should do priming read, but we are lazy.
    uint64_t line_max = 0;
    uint64_t count = 0;
    uint64_t count_tot = 0;
    int c;
    while ((c = getc(pf)) != EOF) {
        // printf("char: %c\n", c);
        count++;
        count_tot++;
        if (UINT64_MAX - 5 <= count_tot) {
            printf("line_length_finder breaks becacuse to long file.\n");

            fclose(pf);
            return 1;
        }
        if (c == '\n') {
            // printf("Line-Length: %lu\n", count);
            if (count > line_max) {
                line_max = count;
            }
            if (count < line_min) {
                line_min = count;
            }
            count = 0;
        }
    }
    if (count > 0) { // last line had no '\n'
        count++;     // count it as if it had one, so it matches the other lines
        if (count > line_max) {
            line_max = count;
        }
        if (count < line_min) {
            line_min = count;
        }
    }
    fclose(pf);

    *line_length = line_max;
    if (print) {
        if (line_min == line_max) {
            printf("Homogenous length of lines = %lu\n", line_max);
        } else {
            printf("Nonhomogenous! Max = %lu, Min = %lu\n", line_max, line_min);
        }
    }

    if (line_min == line_max) {
        *homogenous = 1;
        return 0;
    } else {
        *homogenous = 0;
        return 0;
    }
}

void clearInputBuffer(void) {
    // remove all chars in the input buffer, made by Eric Chen
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
) {
    const uint64_t ONELINE_MAX = 4096; // POSIX Standard

    // Counting the rows
    uint64_t max_line_length;
    uint8_t homogenous_line_length;
    if (line_length_finder(file_name, &max_line_length, 0, &homogenous_line_length)) {
        return 1;
    }
    if (ONELINE_MAX <= max_line_length) {
        printf("rowcol_cunt exits because line-length is to long for fgets\n");
        return 1;
    }
    max_line_length += 2; // Just to make sure we don't break bounds
    FILE *pf;
    pf = fopen(file_name, "r");
    if (!pf) {
        printf("Something wrong with file-opening in rowcol_cunt, mayby wrong file-name?");

        return 1;
    }

    uint64_t temp_number_of_rows = 0;
    int c;
    int prev = '\n'; // so an empty file gives 0 rows
    while ((c = fgetc(pf)) != EOF) {
        if ('\n' == c) {
            temp_number_of_rows += 1;
        }
        prev = c;
    }
    if (prev != '\n') { // last line had no '\n'
        temp_number_of_rows += 1;
    }
    fclose(pf);

    // Counting the columns
    const char *delims = ", \t\r\n";
    pf = fopen(file_name, "r");
    if (!pf) {
        printf("Something wrong with file-opening in rowcol_cunt, mayby wrong file-name?");

        return 1;
    }
    int64_t element_min = INT64_MAX; // We all know we should do priming read, but we are lazy.
    int64_t element_max = INT64_MIN;
    int64_t element;
    uint64_t cols_min = INT64_MAX;
    uint64_t cols_min_row = 0;
    uint64_t cols_max = 0;
    uint64_t cols_max_row = 0;
    uint64_t count = 0;
    char *line;
    line = (char *)malloc(sizeof(char) * (max_line_length + 1));
    if (!line) {
        printf("Issues finding memory for a line, terminating rowcol_cunt");
        fclose(pf);

        return 1;
    }

    char *token;
    for (uint64_t row = 0; row < temp_number_of_rows; row++) {
        // printf("The string: %s\n",line);
        fgets(line, max_line_length - 1, pf);
        // printf("The string: %s\n",line);
        token = strtok(line, delims);
        count = 0;
        while (token != NULL) {
            // printf("The token: %s\n",token);
            count++;
            element = (int64_t)atol(token);
            if (element > element_max) {
                element_max = element;
            }
            if (element < element_min) {
                element_min = element;
            }
            // printf("The count is increased, token: %s\n", token);
            token = strtok(NULL, delims); // Continue with the next token
        }
        // printf("The count is done, count: %d\n",count);
        if (count > cols_max) {
            cols_max = count;
            cols_max_row = row;
        }
        if (count < cols_min) {
            cols_min = count;
            cols_min_row = row;
        }
    }
    fclose(pf);

    // Finalizing output
    *number_of_rows = temp_number_of_rows;
    *number_of_cols = cols_max;
    *elem_max = element_max;
    *elem_min = element_min;
    if (cols_min == cols_max) {
        if (print) {
            printf("Number of rows: %lu\n", *number_of_rows);
            printf("Number of columns(homogenous): %lu\n", *number_of_cols);
            printf("Biggest element (if all integers) is: %ld\n", element_max);
            printf("Smallest element (if all integers) is: %ld\n", element_min);
            printf("\n");
        }
        free(line);

        return 0;
    } else {
        if (print) {
            printf("Number of rows: %lu\n", *number_of_rows);
            printf("Max number of columns: %lu, found on row index: %lu\n", cols_max, cols_max_row);
            printf("Min number of columns: %lu, found on row index: %lu\n", cols_min, cols_min_row);
            printf("Biggest element (if all integers) is: %ld\n", element_max);
            printf("Smallest element (if all integers) is: %ld\n", element_min);
            printf("\n");
        }
        free(line);

        return 2; // Non-homogenous
    }
}

uint8_t n_digits_uint16_t(uint16_t number) {
    if (number == 0)
        return 1;
    uint8_t n = 0;
    while (number) {
        number /= 10;
        n++;
    }
    return n;
}

uint8_t n_digits_uint64_t(uint64_t number) {
    if (number == 0)
        return 1;
    uint8_t n = 0;
    while (number) {
        number /= 10;
        n++;
    }
    return n;
}

uint8_t txtfile_to_charmatrix_2d(const char *file_name, char *matrix, const uint64_t rows, const uint64_t cols) {
    FILE *pf;
    pf = fopen(file_name, "r");
    if (!pf) {
        printf("Something wrong with file-opening in txtfile_to_charmatrix_2d, mayby wrong file-name?");

        return 1;
    }
    int c;
    uint64_t index = 0;
    while (((c = fgetc(pf)) != EOF) && index < rows * cols) {
        if (c != '\n' && c != '\r') {
            matrix[index] = (char)c;
            index++;
        }
    }
    fclose(pf);
    if (index < rows * cols) {
        printf("file had fewer characters than the matrix needs");
        return 1; // file had fewer characters than the matrix needs
    }
    return 0;
}
