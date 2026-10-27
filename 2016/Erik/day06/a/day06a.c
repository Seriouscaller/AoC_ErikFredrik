#include <stdio.h>
#include "input_output.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

char* puzzle_input = "uqwqemis";

enum RETURNCODE {
    OK = 0,
    ERROR = -1,
};

void unsigned_number_to_string(uint64_t number, char* string){

    if(number == 0){
        string[0] = '0';
        return;
    }

    int i = 0;
    while(number > 0){
        string[i++] = number % 10 + '0';
        number /= 10;
    }

    for(int j = 0,  k = i - 1; j < k; j++, k--){
        char temp = string[j];
        string[j] = string[k];
        string[k] = temp;
    }
}

int main(int argc, char *argv[]){

    
    return 0;
}
