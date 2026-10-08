#include <stdio.h>
#include <stdlib.h>
#include "input_output.h"
#include <string.h>

#define PRINT_DEBUG 1

#define ARR_SIZE 5000
#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day02/day02_input_sample.txt"
//#define INPUT_FILE_NAME "C:/Users/ralli/Documents/Code/Github/AoC_ErikFredrik/2016/Erik/day02/day02_input_full.txt"

struct room {
    char room_name;
    struct room* exit_n;
    struct room* exit_e;
    struct room* exit_s;
    struct room* exit_w;
};

struct dungeon{
    int size;
    struct room dungeon[14];
};

struct crawler {
    struct room* location;
};

void create_rooms(struct dungeon* d){
    char* room_ids = "123456789ABCD";
    int dungeon_size = strlen(room_ids);

    for(int i = 1; i <= dungeon_size; i++){

        struct room r = {
            .room_name = room_ids[i-1], 
            .exit_n = NULL, 
            .exit_e = NULL, 
            .exit_s = NULL, 
            .exit_w = NULL
        };

        d->dungeon[i] = r;
    }
    d->size = dungeon_size;
}

void move_crawler(char direction, struct crawler* cr, struct dungeon* d){
    switch (direction) {
        case 'U':
            if(cr->location->exit_n){
                cr->location = cr->location->exit_n;
                printf("Moved to room: %c\n", cr->location->room_name);
            }else{
                printf("No exit north! Staying at room %c\n", cr->location->room_name);
            }
            break;
        case 'R':
            if(cr->location->exit_e){
                cr->location = cr->location->exit_e;
                printf("Moved to room: %c\n", cr->location->room_name);
            }else{
                printf("No exit east! Staying at room %c\n", cr->location->room_name);
            }
            break;
        case 'D':
            if(cr->location->exit_s){
                cr->location = cr->location->exit_s;
                printf("Moved to room: %c\n", cr->location->room_name);
            }else{
                printf("No exit south! Staying at room %c\n", cr->location->room_name);
            }
            break;
        case 'L':
            if(cr->location->exit_w){
                cr->location = cr->location->exit_w;
                printf("Moved to room: %c\n", cr->location->room_name);
            }else{
                printf("No exit west! Staying at room %c\n", cr->location->room_name);
            }
            break;
        default:
           
            break;
    }
}

void connect_rooms(struct dungeon* d){
    // Room 1
    d->dungeon[1].exit_s = &d->dungeon[3];

    // Room 2
    d->dungeon[2].exit_e = &d->dungeon[3];
    d->dungeon[2].exit_s = &d->dungeon[6];

    // Room 3
    d->dungeon[3].exit_n = &d->dungeon[1];
    d->dungeon[3].exit_e = &d->dungeon[3];
    d->dungeon[3].exit_s = &d->dungeon[6];
    d->dungeon[3].exit_w = &d->dungeon[2];

    // Room 4
    d->dungeon[4].exit_w = &d->dungeon[3];
    d->dungeon[4].exit_s = &d->dungeon[8];

    // Room 5
    d->dungeon[5].exit_e = &d->dungeon[6];

    // Room 6
    d->dungeon[6].exit_n = &d->dungeon[2];
    d->dungeon[6].exit_e = &d->dungeon[7];
    d->dungeon[6].exit_s = &d->dungeon[10];
    d->dungeon[6].exit_w = &d->dungeon[5];

    // Room 7
    d->dungeon[7].exit_n = &d->dungeon[3];
    d->dungeon[7].exit_e = &d->dungeon[8];
    d->dungeon[7].exit_s = &d->dungeon[11];
    d->dungeon[7].exit_w = &d->dungeon[6];

    // Room 8
    d->dungeon[8].exit_n = &d->dungeon[4];
    d->dungeon[8].exit_e = &d->dungeon[9];
    d->dungeon[8].exit_s = &d->dungeon[12];
    d->dungeon[8].exit_w = &d->dungeon[7];

    // Room 9
    d->dungeon[9].exit_w = &d->dungeon[8];

    // Room A (10)
    d->dungeon[10].exit_n = &d->dungeon[6];
    d->dungeon[10].exit_e = &d->dungeon[11];

    // Room B (11)
    d->dungeon[11].exit_n = &d->dungeon[7];
    d->dungeon[11].exit_e = &d->dungeon[12];
    d->dungeon[11].exit_s = &d->dungeon[13];
    d->dungeon[11].exit_w = &d->dungeon[10];

    // Room C (12)
    d->dungeon[12].exit_n = &d->dungeon[8];
    d->dungeon[12].exit_w = &d->dungeon[11];

    // Room D (13)
    d->dungeon[13].exit_n = &d->dungeon[11];
}

int main(){

    long size = get_file_size(INPUT_FILE_NAME);
    
    char* input =  allocate(size);
    int ret = read_file(INPUT_FILE_NAME, input);
    if(ret == 1 ){
        printf("Unable to open file!\n");
        abort();
    }else {
        printf("File read succesfully!\n");
    }

    #ifdef PRINT_DEBUG 
        printf("Filesize: %lu\n", size);
        printf("%s", input);
    #endif
    
    struct dungeon d;
    create_rooms(&d);
    connect_rooms(&d);

    struct crawler crawler;
    crawler.location = &d.dungeon[5];   // Starting position Room 5
    printf("Crawler start: %c\n", crawler.location->room_name);

    for(int i = 0; i < size; i++){
        printf("i[%d] Character: %c ASCII: %d\n", i, input[i], (int)input[i]);

        if(input[i] == '\n'){
            printf("End of row. Crawler at: %c\n\n", crawler.location->room_name);
        }
        move_crawler(input[i], &crawler, &d);
    }

    free_allocation(input);
    return EXIT_SUCCESS;
}