#include <stdio.h>
#include <stdlib.h>

typedef struct{
    long size;
    char *data;
} content;

content get_input(){
    FILE *f = fopen("input1.txt","r");
    if(!f){
        printf("Couldn't open input file\n");
        exit(1);
    }
    fseek(f,0,SEEK_END);
    long f_len = ftell(f);
    rewind(f);
    content input = { f_len , malloc(sizeof(char)*f_len) };
    fread(input.data,sizeof(char),f_len,f);
    return input;
}

int main(){
    content input = get_input();
    int level = 0;
    int basement_pos = -1;
    for(int i = 0; i < input.size; i++){
        switch(input.data[i]){
            case '(':
                level++;
                break;
            case ')':
                level--;
                break;
            default:
                break;
        }
    }
    printf("Level %d\n",level);
    free(input.data);
}