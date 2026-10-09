#pragma once

// Imports
#include <inttypes.h>

// Prototypes
void clearInputBuffer(void);
uint8_t line_length_finder(const char *file_name, uint64_t *line_length, const uint8_t print, uint8_t *homogenous);
uint8_t n_digits_uint16_t(uint16_t number);
uint8_t n_digits_uint64_t(uint64_t number);
uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
);
uint8_t txtfile_to_charmatrix_2d(const char *file_name, char *matrix, const uint64_t rows, const uint64_t cols);
