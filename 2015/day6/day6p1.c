#include <stdio.h>
#include <stdlib.h>

typedef struct{
    long size;
    char *data;
} content;

content read_input(){
    FILE *input;
    fopen_s(&input,"input.txt","rb");
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1);}
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    content ctnt = { end_pos, calloc(end_pos+1,1) };
    fread(ctnt.data,1,end_pos,input);
    fclose(input);
    return ctnt;
}

typedef enum _action{
    TURN_OFF,
    TURN_ON,
    TOGGLE
} action;

typedef struct _pnt{
    short x;
    short y;
} point;

typedef struct _instr{
    action cmd;
    point tl;
    point br;
} instr;

point parse_point(char data[], int *ci){
    int x = 0;
    while(data[*ci] != ','){
        x = (x*10) + (data[*ci]-'0');
        (*ci)++;
    }
    (*ci)++;
    int y = 0;
    while(data[*ci] >= '0' && data[*ci] <= '9'){
        y = (y*10) + (data[*ci]-'0');
        (*ci)++;
    }
    (*ci)++;
    return (point) {x,y};
}

instr parse_instruction(char data[], int s, int e){
    int c = s;
    while(data[c] != ' ') c++;
    if(data[c-1] == 'n'){
        c++;
        while(data[c] != ' ') c++;
    }
    action act;
    switch(c-s){
        case 8:
            act = TURN_OFF;
            break;
        case 7:
            act = TURN_ON;
            break;
        case 6:
            act = TOGGLE;
            break;
    }
    s = c+1;
    point spt = parse_point(data,&s);
    s++;
    while(data[s] != ' ') s++;
    s++;
    point ept = parse_point(data,&s);
    return (instr){ act , spt , ept };
}

int main(){
    content input = read_input();
    char **grid = (char**) calloc(1000,sizeof(char*));
    for(int r = 0; r < 1000; r++){
        grid[r] = (char*) calloc(1000,sizeof(char));
    }
    int i = 0;
    while(input.data[i]){
        int instr_s = i;
        while(input.data[i] != '\n') i++;
        int instr_e = i;
        instr ins = parse_instruction(input.data,instr_s,instr_e);
        point s = ins.tl, e = ins.br;
        for(int y = s.y; y <= e.y; y++){
            for(int x = s.x; x <= e.x; x++){
                switch(ins.cmd){
                    case TURN_ON:
                        grid[y][x] = 1;
                        break;
                    case TURN_OFF:
                        grid[y][x] = 0;
                        break;
                    case TOGGLE:
                        grid[y][x] ^= 1;
                        break;
                }
            }
        }
        i++;
    }
    int lights = 0;
    for(int y = 0; y < 1000; y++){
        for(int x = 0; x < 1000; x++){
            if(grid[y][x]) lights++;
        }
    }
    printf("Lights on: %d",lights);
    for(int r = 0; r < 1000; r++){
        free(grid[r]);
    }
    free(grid);
    free(input.data);
}