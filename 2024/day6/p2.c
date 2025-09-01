#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char* read_input(){
    FILE *input = fopen("input.txt","rb");
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1); }
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    char *content = (char*) calloc(1,end_pos+1);
    fread(content,1,end_pos,input);
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

void track_path(char *grid, dimens gdsz){
    FILE *ctnt = fopen("track.txt","wb");
    int gdh = gdsz.h, gdw = gdsz.w;
    char lbrk[] = "\r\n";
    for(int r = 0; r < gdh; r++){
        fwrite(grid+(r*gdw),sizeof(char),gdw,ctnt);
        fwrite(lbrk,sizeof(char),2,ctnt);
    }
    fclose(ctnt);
}

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
    GD_UP,
    GD_RIGHT,
    GD_DOWN,
    GD_LEFT
} guard_dir;

char scan_cell_symbol(char grid[], int gridw, cell scan){
    return grid[(scan.y*gridw)+scan.x];
}

int has_space_forward(cell guard, guard_dir gdir, dimens gdsz){
    switch(gdir){
        case GD_UP:
            return guard.y > 0;
        case GD_DOWN:
            return guard.y < gdsz.h-1;
        case GD_LEFT:
            return guard.x > 0;
        case GD_RIGHT:
            return guard.x < gdsz.w-1;
    }
}

int has_space_to_right(cell guard, guard_dir gdir, dimens gdsz){
    switch(gdir){
        case GD_UP:
            return guard.x < gdsz.w-1;
        case GD_DOWN:
            return guard.x > 0;
        case GD_LEFT:
            return guard.y > 0;
        case GD_RIGHT:
            return guard.y < gdsz.h-1;
    }
}

int is_same_rel(guard_dir gdir, char cell_sym){
    switch(gdir){
        case GD_UP:
            return cell_sym == 'U';
        case GD_RIGHT:
            return cell_sym == 'R';
        case GD_DOWN:
            return cell_sym == 'D';
        case GD_LEFT:
            return cell_sym == 'L';
    }
}

int main(){
    const char dirs[] = {'U','R','D','L'};
    char* input = read_input();
    cell guard;
    dimens grid_sz = get_dimens(input);
    char *grid = (char*) calloc(1,grid_sz.w*grid_sz.h);
    form_grid(input,grid_sz,grid,&guard);
    guard_dir gdir = GD_UP;
    int obst_pos = 0;
    int g_idx = (guard.y*grid_sz.w) + guard.x;
    int can_scan = 1;
    const int mv_unit[] = {-grid_sz.w,1,grid_sz.w,-1};
    const int mv_coord[] = {-1,1,1,-1};
    while(guard.x >= 0 && guard.x < grid_sz.w && guard.y >= 0 && guard.y < grid_sz.h){
        int est_gdir = (gdir+1)&3;
        if(has_space_forward(guard,gdir,grid_sz)){
           if(grid[g_idx+mv_unit[gdir]] == '#'){
                gdir = est_gdir;
                can_scan = 0;
            }else if(grid[g_idx+mv_unit[gdir]] == dirs[est_gdir]){
                obst_pos++;
                track_path(grid,grid_sz);
            }
        }
        if(can_scan && has_space_to_right(guard,gdir,grid_sz)){
            cell scan = { guard.x, guard.y };
            if(gdir == GD_UP || gdir == GD_DOWN) scan.x += mv_coord[est_gdir];
            else scan.y += mv_coord[est_gdir];
            char cell_sym = scan_cell_symbol(grid,grid_sz.w,scan);
            char right_rel = dirs[(est_gdir+1)&3];
            int passed_right_rel = 0;
            while(scan.x >= 0 && scan.x < grid_sz.w && scan.y >= 0 && scan.y < grid_sz.h){
                if(cell_sym == right_rel){
                    passed_right_rel = 1;
                }else if(is_same_rel(est_gdir,cell_sym)){
                    obst_pos++;
                    track_path(grid,grid_sz);
                    break;
                }
                if(est_gdir == GD_UP || est_gdir == GD_DOWN) scan.x += mv_coord[est_gdir];
                else scan.y += mv_coord[est_gdir];
                if(passed_right_rel){
                    passed_right_rel = 0;
                    if(scan_cell_symbol(grid,grid_sz.w,scan) == '#'){
                        obst_pos++;
                        track_path(grid,grid_sz);
                        break;
                    }
                }
                cell_sym = scan_cell_symbol(grid,grid_sz.w,scan);
            }
        }else can_scan = 1;
        switch(gdir){
            case GD_UP:
                guard.y--;
                g_idx -= grid_sz.w;
                break;
            case GD_DOWN:
                guard.y++;
                g_idx += grid_sz.w;
                break;
            case GD_LEFT:
                guard.x--;
                g_idx--;
                break;
            case GD_RIGHT:
                guard.x++;
                g_idx++;
                break;
            }
        grid[g_idx] = dirs[gdir];    
    }
    printf("Possible obstacles: %d\n",obst_pos);
    print_grid(grid,grid_sz);
    free(grid);
    free(input);
}
