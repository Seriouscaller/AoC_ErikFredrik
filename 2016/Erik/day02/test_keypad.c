#include <assert.h>
#include "keypad.h"

void test_move_within_bounds(void){
    assert(move('U', 5) == 2);
    assert(move('D', 5) == 8);
    assert(move('R', 5) == 6);
    assert(move('L', 5) == 4);
}

void test_move_boundrary_guards(void){
    // Top side boundrary
    assert(move('U', 1) == 1);
    assert(move('U', 2) == 2);
    assert(move('U', 3) == 3);

    // Bottom side boundrary
    assert(move('D', 7) == 7);
    assert(move('D', 8) == 8);
    assert(move('D', 9) == 9);
    
    // Right side boundrary
    assert(move('R', 3) == 3);
    assert(move('R', 6) == 6);
    assert(move('R', 9) == 9);

    // Left side boundrary
    assert(move('L', 1) == 1);
    assert(move('L', 4) == 4);
    assert(move('L', 7) == 7);
}

void test_move_invalid_char(void){
    assert(move('A', 5) == 5);
    assert(move('&', 1) == 1);
    assert(move(' ', 3) == 3);
    assert(move('\n', 6) == 6);
}

int main(void) {

    test_move_within_bounds();
    test_move_boundrary_guards();
    test_move_invalid_char();

    return 0;
}