#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define ARR_SIZE 50000

struct Waypoint {
    int x;
    int y;
    int direction;
    int length;
};

struct Coordinate {
    int x;
    int y;
};

struct History {
    struct Coordinate locations[ARR_SIZE];
    int num_of_locations;
};

void set_direction(struct Waypoint* wp, char c);
int set_length(struct Waypoint* wp, char* array, int pos);
int calculate_distance(int x1, int y1, int x2, int y2);
void traverse_grid(struct Waypoint* wp, struct History* hist);
bool isVisited(struct Coordinate* c, struct History* hist);
void add_location(struct Coordinate* c, struct History* hist);

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
            printf("L ");
        }else if(c == 'R'){
            wp->direction = (wp->direction + 90) % 360;
            printf("R ");
        }else {
            printf("Setting direction failed!");
            abort();
        }

        if(wp->direction < 0){
            wp->direction += 360;
        }

        printf("D:%d ", wp->direction);
        switch (wp->direction) {
        case 0:
            printf("^");
            break;
        case 90:
            printf(">");
            break;
        case 180:
            printf("v");
            break;
        case 270:
            printf("<");
            break;
        };

}

int calculate_distance(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
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
    printf("%d\n", magnitude);
    wp->length = magnitude;

    return pos;
}

void add_location(struct Coordinate* c, struct History* hist){
    int num = hist->num_of_locations;
    hist->locations[num] = *c;
    hist->num_of_locations++;
}

void init_struct_array(struct Coordinate* arr, int len){
    for(int i = 0; i < len; i++){
        struct Coordinate c = {.x = '\0', .y = '\0'};
        arr[i] = c;
    }
    struct Coordinate c = {.x = 0, .y = 0};
    arr[0] = c;
}

bool isVisited(struct Coordinate* c, struct History* hist){
    for(int i = 0; i < hist->num_of_locations;i++){
        if(hist->locations[i].x == c->x && hist->locations[i].y == c->y){
            printf("Coordinates %d,%d visited once before!", c->x, c->y);
            return true;
        }
    }
    return false;
}

void traverse_grid(struct Waypoint* wp, struct History* hist){
    
    struct Coordinate c = {.x = wp->x, .y=wp->y};
    while(0 < wp->length){
        switch (wp->direction) {
        case 0:
            c.y++;
            break;
        case 90:
            c.x++;
            break;
        case 180:
            c.y--;
            break;
        case 270:
            c.x--;
            break;
        };

        printf("Coordinate: %d,%d\n", c.x, c.y);

        // Check if coordinate is present in locations[].
        // If it is, Calculate mahattan distance, then exit
        if(isVisited(&c, hist)){
            int dist = calculate_distance(0, 0, c.x, c.y);
            printf("Been here before! %d,%d Distance from 0,0: %d\n", c.x, c.y, dist);
            abort();
        }
        // If NOT, add location to locations and incrent num_of_locations
        add_location(&c, hist);
        wp->length--;
    }
    wp->x = c.x;
    wp->y = c.y;
}

int main(){
    char file_name[] = "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day01_input_full.txt";
    char input[ARR_SIZE];

    struct History history = {
        .num_of_locations = 1,
    };

    init_struct_array(history.locations, ARR_SIZE);
    
    int ret = read_file(file_name, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        abort();
    }else {
        printf("File read succesfully!\n");
    }

    // Looping over array content
    int str_len = strlen(input);
    char c;
    struct Waypoint wp = {.x = 0, .y=0, .direction = 0};
    for(int j = 0; j <= str_len; j++){
        c = input[j];
        
        // Skip character if no 'L' or 'R' found
        if(c != 'L' && c != 'R'){
            continue;
        };
        
        set_direction(&wp, c); // wp.Direction is set at an angle. 0,90,180,270
        set_length(&wp, input, j);  // Extracting length

        traverse_grid(&wp, &history);
        wp.length = 0;

        if(j == 100){
            printf("Pause here\n");
        }
        printf("Len:%d, j:%d\n", str_len, j);

    }

    return 0;
}

// 1: 288
// 2: 111

/*
Bugs
Problem: Didnt account for starting location. Solution: Adding 0,0 to visited locations.
*/