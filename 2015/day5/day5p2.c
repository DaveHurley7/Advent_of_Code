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

int verify_nice(char string[], int s, int e){
    int passes = 0;
    for(int p = s; p < e-3; p++){
        for(int c = p+2; c < e-1; c++){
            if(string[p] == string[c] && string[p+1] == string[c+1]){
                passes++;
                break;
            }
        }
    }
    for(int c = s; c < e-2; c++){
        if(string[c] == string[c+2]){
            passes++;
            break;
        }
    }
    if(passes < 2) return 0;
    else return 1;
}

int main(){
    content input = read_input();
    int c = 0;
    int nice_strings = 0;
    while(input.data[c]){
        int start = c;
        while(input.data[c] != '\n'){
            c++;
        }
        int end = c;
        if(verify_nice(input.data,start,end)) nice_strings++;
        c++;
    }
    printf("Nice strings %d",nice_strings);
    free(input.data);
}