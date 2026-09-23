#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

struct Waypoint {
    int x;
    int y;
    int direction;
};

void set_direction(struct Waypoint* wp, char c);
void set_length(struct Waypoint* wp, char c);

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
        printf("Direction: %d ", wp->direction);
}

void set_length(struct Waypoint* wp, char c){
    c++;
    
    if(!isdigit(c)){
        printf("Not a numeric character!");
        abort();
    }

    char num[5] = {'\0'};
    int i = 0;
    while(isdigit(c)){
        num[i] = c;
        c++;i++;
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

}

int main(){
    char file_name[] = "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day01_input_sample.txt";
    char array[500];
    
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
        set_length(&wp, c);
        printf("\nUpdated Location: %d,%d", wp.x, wp.y);
    }

    return 0;
}
    