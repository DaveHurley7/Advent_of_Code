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

int check_for_word(char grid[], int LUc, int RUc, int LDc, int RDc){
    if(grid[LUc] ==  'M' && grid[RUc] ==  'M' && grid[LDc] ==  'S' && grid[RDc] ==  'S')
        return 1;
    if(grid[LUc] ==  'S' && grid[RUc] ==  'S' && grid[LDc] ==  'M' && grid[RDc] ==  'M')
        return 1;
    if(grid[LUc] ==  'M' && grid[RUc] ==  'S' && grid[LDc] ==  'M' && grid[RDc] ==  'S')
        return 1;
    if(grid[LUc] ==  'S' && grid[RUc] ==  'M' && grid[LDc] ==  'S' && grid[RDc] ==  'M')
        return 1;
    return 0;
}

int find_word_count(char grid[], shape dimens){
    int len = dimens.height * dimens.width;
    int x = 0, y = 0;
    int words = 0;
    for(int c = 0; c < len; c++){
        if(grid[c] == 'A'){
            int diag_distp = dimens.width+1;
            int diag_distm = dimens.width-1;
            if(x >= 1 && y >= 1 && x <= dimens.width-1 && y <= dimens.height-1){
                if(check_for_word(grid,c-diag_distp,c-diag_distm,c+diag_distm,c+diag_distp)){
                    words++;
                }
            }
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