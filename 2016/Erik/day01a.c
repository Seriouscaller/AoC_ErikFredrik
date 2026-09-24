#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

struct Waypoint {
    int x;
    int y;
    int direction;
};

void set_direction(struct Waypoint* wp, char c);
int set_length(struct Waypoint* wp, char* array, int pos);
int calculate_distance(int x1, int y1, int x2, int y2);

int read_file(char* file_name, char* array){
    FILE *fp = fopen(file_name, "r");
    char ch;
    
    if(fp == NULL) {
        return 1;
    }

    int i = 0;
    while ((ch = fgetc(fp)) != EOF){
        array[i++] = ch;
    }
    array[++i] = '\0';
    fclose(fp);

    return 0;
}

void set_direction(struct Waypoint* wp, char c){
        if(c == 'L'){
            wp->direction = (wp->direction - 90) % 360;
        }else if(c == 'R'){
            wp->direction = (wp->direction + 90) % 360;
        }else {
            printf("Setting direction failed!");
            abort();
        }

        if(wp->direction < 0){
            wp->direction += 360;
        }

        printf("Direction: %d ", wp->direction);
}

int calculate_distance(int x1, int y1, int x2, int y2){
    return fabs(x1 - x2) + fabs(y1 - y2);
}

int set_length(struct Waypoint* wp, char* array, int pos){

    pos++; // Skiping direction letter (L,R)

    if(!isdigit(array[pos])){
        printf("Not a numeric character!");
        abort();
    }

    char num[5] = {'\0'};
    int i = 0;
    while(isdigit(array[pos])){
        num[i] = array[pos];
        pos++;i++;
    }

    int magnitude = atoi(num);
    printf("Magnitude: %d ", magnitude);
    
    switch (wp->direction) {
        case 0:
            wp->y += magnitude;
            break;
        case 90:
            wp->x += magnitude;
            break;
        case 180:
            wp->y -= magnitude;
            break;
        case 270:
            wp->x -= magnitude;
            break;
    }
    printf("Updated Location: %d,%d", wp->x, wp->y);

    return pos;
}

int main(){
    char file_name[] = "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day01_input_full.txt";
    char array[5000];
    
    int ret = read_file(file_name, array);
    if(ret == 1 ){
        printf("Unable to open file!");
        abort();
    }else {
        printf("File read succesfully!");
    }

    // Looping over array content
    int str_len = strlen(array);
    char c;
    struct Waypoint wp = {.x = 0, .y=0, .direction = 0};
    for(int j = 0; j <= str_len; j++){
        c = array[j];
        
        // Skip character if no 'L' or 'R' found
        if(c != 'L' && c != 'R'){
            continue;
        };
        
        set_direction(&wp, c);
        j = set_length(&wp, array, j);
        printf("\nUpdated Location: %d,%d", wp.x, wp.y);
    }

    int manhattan_distance = calculate_distance(0,0,wp.x, wp.y);
    printf("\nManhattan Distance: %d", manhattan_distance);
    return 0;
}

// 1: 318 Incorrect
// 2: 288 Correct

/*
Bugs:

Problem: When decrementing direction into the negative, the modulo operation (% 360) didnt affect negative values
Solution: It was solved by an if statement (dir < 0) then dir+360, effectivly capping the dir 0-360

Problem: The algorithm stopped short.
Solution: Array was too small to fit full input. Increased size from 500 to 5000

Problem: The for loop jumped by large steps causing missed characters.
Solution: Misuse of += instead of using =. Skipped steps in loop. The variable 
was a index counter, so adding it to eachother was an error.

*/