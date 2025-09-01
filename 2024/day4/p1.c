#include <stdio.h>
#include <stdlib.h>

char* read_input(){
    FILE *input = fopen("input.txt","rb");
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
    int width;
    int height;
}shape;

shape get_shape(char input[]){
    int c = 0;
    while(input[c] != '\r') c++;
    int width = c;
    int height = 0;
    while(input[c] != '\0'){
        if(input[c] == '\r') height++;
        c++;
    }
    return (shape){ width , height };
}

void fill_grid(char grid[], char input[], shape dimens){
    int c = 0, p = 0;
    while(input[c]){
        if(input[c] == '\r') c+=2;
        else{
            grid[p] = input[c];
            c++;
            p++;
        }
    }
}

int check_for_word(char grid[], int c1, int c2, int c3){
    if(grid[c1] != 'M') return 0;
    if(grid[c2] != 'A') return 0;
    if(grid[c3] != 'S') return 0;
    return 1;
}

int find_word_count(char grid[], shape dimens){
    int len = dimens.height * dimens.width;
    int x = 0, y = 0;
    int words = 0;
    for(int c = 0; c < len; c++){
        if(grid[c] == 'X'){
            int diag_distp = dimens.width+1;
            int diag_distm = dimens.width-1;
            int vdist = dimens.width;
            if(x >= 3 && y >= 3) 
                if(check_for_word(grid,c-diag_distp,c-(2*diag_distp),c-(3*diag_distp))) words++;
            if(x <= dimens.width-3 && y >= 3)
                if(check_for_word(grid,c-diag_distm,c-(2*diag_distm),c-(3*diag_distm))) words++;
            if(x >= 3 && y <= dimens.height-3)
                if(check_for_word(grid,c+diag_distm,c+(2*diag_distm),c+(3*diag_distm))) words++;
            if(x <= dimens.width-3 && y <= dimens.height-3) 
                if(check_for_word(grid,c+diag_distp,c+(2*diag_distp),c+(3*diag_distp))) words++;
            if(y >= 3)
                if(check_for_word(grid,c-vdist,c-(2*vdist),c-(3*vdist))) words++;
            if(y <= dimens.height-3)
                if(check_for_word(grid,c+vdist,c+(2*vdist),c+(3*vdist))) words++;
            if(x >= 3)
                if(check_for_word(grid,c-1,c-2,c-3)) words++;
            if(x <= dimens.width-3)
                if(check_for_word(grid,c+1,c+2,c+3)) words++;
        }
        x++;
        if(x % dimens.width == 0){
            x = 0;
            y++;
        }
    }
    return words;
}

int main(){
    char* input = read_input();
    shape dimens = get_shape(input);
    char grid[dimens.height*dimens.width];
    fill_grid(grid,input,dimens);
    int count = find_word_count(grid,dimens);
    printf("Total word count: %d\n",count);
    free(input);
}