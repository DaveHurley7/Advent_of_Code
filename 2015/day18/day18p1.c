#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    long size;
    char *data;
} content;

content read_input(){
    FILE *input;
    fopen_s(&input,"input.txt","rb");
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1); }
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    content ctnt = { end_pos, calloc(end_pos+1,1) };
    fread(ctnt.data,1,end_pos,input);
    fclose(input);
    return ctnt;
}

#define GRID_LEN 100

void init_state(char *data, char grid[GRID_LEN][GRID_LEN]){
    int c = 0;
    int r = 0;
    while(data[c]){
        if(data[c+GRID_LEN] == '\n'){
            memcpy(grid[r],data+c,GRID_LEN);
        }
        c += GRID_LEN+1;
        r++;
    }
}

int count_neighbours_on(char curr[GRID_LEN][GRID_LEN],int x, int y){
    int total = 0;
    if(x > 0 && y > 0 && curr[y-1][x-1] == '#') total++;
    if(y > 0 && curr[y-1][x] == '#') total++;
    if(x+1 < GRID_LEN && y > 0 && curr[y-1][x+1] == '#') total++;
    if(x+1 < GRID_LEN && curr[y][x+1] == '#') total++;
    if(x+1 < GRID_LEN && y+1 < GRID_LEN && curr[y+1][x+1] == '#') total++;
    if(y+1 < GRID_LEN && curr[y+1][x] == '#') total++;
    if(x > 0 && y+1 < GRID_LEN && curr[y+1][x-1] == '#') total++;
    if(x > 0 && curr[y][x-1] == '#') total++;
    return total;
}

void animate_state(char curr[GRID_LEN][GRID_LEN], char next[GRID_LEN][GRID_LEN]){
    for(int y = 0; y < GRID_LEN; y++){
        for(int x = 0; x < GRID_LEN; x++){
            int neightbour_lights = count_neighbours_on(curr,x,y);
            if(curr[y][x] == '#'){
                next[y][x] = neightbour_lights == 2 || neightbour_lights == 3 ? '#' : '.';
            }else{
                next[y][x] = neightbour_lights == 3 ? '#' : '.';
            }
        }
    }
    for(int r = 0; r < GRID_LEN; r++){
        memcpy(curr[r],next[r],GRID_LEN);
    }
}

int count_lights_on(char grid[GRID_LEN][GRID_LEN]){
    int lights = 0;
    for(int y = 0; y < GRID_LEN; y++){
        for(int x = 0; x < GRID_LEN; x++){
            if(grid[y][x] == '#') lights++;
        }
    }
    return lights;
}

int main(){
    content input = read_input();
    char current_state[GRID_LEN][GRID_LEN];
    char next_state[GRID_LEN][GRID_LEN];
    init_state(input.data,current_state);
    for(int s = 0; s < 100; s++){
        animate_state(current_state,next_state);
    }
    int lights_on = count_lights_on(current_state);
    printf("Number of lights on: %d\n",lights_on);
    free(input.data);
}