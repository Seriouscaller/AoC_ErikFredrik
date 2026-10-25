#pragma once

// Imports
#include <inttypes.h>

// Prototypes

/**
 * @brief Clears the input buffer. Made by Eric Chen.
 */
void clearInputBuffer(void);

/*!
 * @brief finds the length of lines in a txt-file. Will return length INCLUDING
 * newlines and EOF.
 *
 * @param[in] file_name  The filename as a string
 * @param[in] line_length  The address to a uint64_t variable to store the
 * line_length
 * @param[in] print 1 / 0 to print or not to print
 * @param[in] homogenous The address to a uint8_t variable to store if
 * line-lengths are homogenous or not
 *
 * @return a uint8_t.  1 is unsuccesful operation , 0 succesful
 * Last updated feb 2026
 */
uint8_t line_length_finder(const char *file_name, uint64_t *line_length, const uint8_t print, uint8_t *homogenous);

/*!
 * @brief A function for finding the number of digits in a u16 number in base10. for
 * example 12345 has 5 digits.
 * @param[in] number The number in question
 * @return the number of digits
 * Last updated feb 2026
 */
uint8_t n_digits_uint16_t(uint16_t number);

/*!
 * @brief A function for finding the number of digits in a u64 number in base10. for
 * example 12345 has 5 digits.
 * @param[in] number The number in question
 * @return the number of digits
 * Last updated oct 2026
 */
uint8_t n_digits_uint64_t(uint64_t number);

/**
 * @brief
 * @param file_name path to file to analyze, typical .csv or alike
 * @param number_of_rows adr to outside return
 * @param number_of_cols adr to outside return
 * @param elem_max adr to outside return
 * @param elem_min adr to outside return
 * @param print u8 1 or 0, whether to print info or not
 * @return 0 on sucess. 1 on error OR different nmb of columns
 */
uint8_t rowcol_cunt(
    const char *file_name,
    uint64_t *number_of_rows,
    uint64_t *number_of_cols,
    int64_t *elem_max,
    int64_t *elem_min,
    const uint8_t print
);

/**
 * @brief takes a txtfile and converts it to a 2d matrix of chars. Don't remember: probably requires rectangular input
 * @param file_name path to file
 * @param adr matrix preallocated / set 2dmatrix
 * @param rows u64
 * @param cols u64
 * @return 0 success, 1 fail
 */
uint8_t txtfile_to_charmatrix_2d(const char *file_name, char *matrix, const uint64_t rows, const uint64_t cols);
