# Day 3

## part a

This challenge doesnt really interest me, its just logistics as I see it. At least the first part

I found an old rowcoltouin16matrix, probably horribly bad but i will use it.

### Implementation

- Verify input that everything fits in uint16_t
- rowcol to get number of rows and columns
- allocate
- use old rowcol16 to matrix
- for-loop over all rows
    - n1 = biggest
    - n2 n3 bigger? => biggest
    - verify triangel possible
    - if not illegals_nmb ++

### notes

the rowcoltouin16 IS horribly bad, and whats worse is that it got horribly formatted by clang-formatter also. Pure cancer. I would rewrite if it was a more interesting challenge.

## part b

Ok so now its looking down the columns instead. I could change how we store the matrix to get the memory sequential, but im not going to do that. Ill keep the memory map

### changes neeeded

- verify input has to make sure the number of rows is a multiple of 3
- illegalchecker takes 3 numbers as input instead of a pointer
- looping over all input:
```
    for (col_ind 0 -> 2):
        row_ind = 0
        while row_ind < nmb_of_rows:
            n1 = matrix[row_ind++][col_ind]
            n2 = matrix[row_ind++][col_ind]
            n3 = matrix[row_ind++][col_ind]
            is_illagal(n1, n2, n3)
```
