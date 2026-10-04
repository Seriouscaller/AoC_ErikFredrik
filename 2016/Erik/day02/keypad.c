#include <stdio.h>
#include "keypad.h"
#include <assert.h>

int move(char c, int digit){
    switch (c) {
        case 'U':
            if((3 < digit)){
                digit -= 3;
            }
            break;
        case 'D':
            if((7 > digit)){
                digit += 3;
            }
            break;
        case 'R':
            if(!(digit % 3 == 0)){
                digit++;
            }
            break;
        case 'L':
            if(!(digit % 3 == 1)){
                digit--;
            }
        break;
        default:
            printf("move() - Invalid Character: %c\n", c);
        break;
    };

    return digit;
}