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

int parse_num(char *input, int *i){
    int c = *i;
    int is_neg = input[c-1] == '-' ? 1 : 0;
    int val = 0;
    while(input[c] >= 0x30 && input[c] <= 0x39){
        val = val*10 + (input[c++]-'0');
    }
    if(is_neg) val = -val;
    *i = c;
    return val;
}

int main(){
    content input = read_input();
    int i = 0;
    int total = 0;
    while(input.data[i]){
        if(input.data[i] >= 0x30 && input.data[i] <= 0x39){
            total += parse_num(input.data,&i);
        }else i++;
    }
    printf("Sum of numbers: %d\n",total);
    free(input.data);
}