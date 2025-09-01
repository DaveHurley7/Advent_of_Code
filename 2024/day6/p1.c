#include <stdio.h>
#include <stdlib.h>

char* read_input(){
    FILE *input = fopen("input.txt","rb");
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1); }
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    char *content = (char*) malloc(end_pos+1);
    fread(content,1,end_pos,input);
    content[end_pos] = '\0';
    fclose(input);
    return content;
}

typedef struct{
    int w;
    int h;
} dimens;

typedef struct{
    int x;
    int y;
} cell;

dimens get_dimens(char grid[]){
    int g = 0;
    int w = 0;
    int h = 0;
    while(grid[g]){
        while(!h && grid[g] != '\r'){
            w++;
            g++;
        }
        if(grid[g] == '\r'){
            h++;
            g+=2;
        }else g++;
    }
    return (dimens) { w , h };
}

void form_grid(char input[], dimens size, char grid[], cell* guard){
    int dta_idx = 0;
    for(int j = 0; j < size.h; j++){
        int rows_down = j*size.w;
        for(int i = 0; i < size.w; i++){
            if(input[dta_idx] == '^'){
                *guard = (cell) { i , j };
                input[dta_idx] = '.';
            }
            grid[rows_down+i] = input[dta_idx++];
        }
        dta_idx += 2;
    }
}

void print_grid(char grid[], dimens grid_sz){
    for(int j = 0; j < grid_sz.h; j++){
        int r = j*grid_sz.w;
        for(int i = 0; i < grid_sz.w; i++){
            printf("%c",grid[r+i]);
        }
        printf("\n");
    }
}
typedef enum guard_dir{
    GD_LEFT,
    GD_RIGHT,
    GD_UP,
    GD_DOWN,
} guard_dir;

int main(){
    char* input = read_input();
    cell guard;
    dimens grid_sz = get_dimens(input);
    char *grid = (char*) calloc(1,grid_sz.w*grid_sz.h);
    form_grid(input,grid_sz,grid,&guard);
    guard_dir gdir = GD_UP;
    int tiles = 0;
    int g_idx = (guard.y*grid_sz.w) + guard.x;
    while(guard.x >= 0 && guard.x < grid_sz.w && guard.y >= 0 && guard.y < grid_sz.h){
        if(grid[g_idx] == '.'){
            tiles++;
            grid[g_idx] = 'X';
        }
        switch(gdir){
            case GD_LEFT:
                if(guard.x > 0 && grid[g_idx-1] == '#'){
                    gdir = GD_UP;
                    guard.y--;
                    g_idx -= grid_sz.w;
                }else{ guard.x--; g_idx--; }
                break;
            case GD_UP:
                if(guard.y > 0 && grid[g_idx-grid_sz.w] == '#'){
                    gdir = GD_RIGHT;
                    guard.x++;
                    g_idx++;
                }else{ guard.y--; g_idx -= grid_sz.w; }
                break;
            case GD_RIGHT:
                if(guard.x < grid_sz.w-1 && grid[g_idx+1] == '#'){
                    gdir = GD_DOWN;
                    guard.y++;
                    g_idx += grid_sz.w;
                }else{ guard.x++; g_idx++; } 
                break;
            case GD_DOWN:
                if(guard.y < grid_sz.h-1 && grid[g_idx+grid_sz.w] == '#'){
                    gdir = GD_LEFT;
                    guard.x--;
                    g_idx--;
                }else{ guard.y++; g_idx += grid_sz.w; }
                break;
        }
    }
    printf("Distinct tiles: %d\n",tiles);
    //print_grid(grid,grid_sz);
    free(grid);
    free(input);
}
