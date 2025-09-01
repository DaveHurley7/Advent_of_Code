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

int verify_nice(char string[],int s, int e){
    int vowels = 0;
    for(int c = s; c < e; c++){
        switch(string[c]){
            case 'a': case 'e': case 'i': case 'o': case 'u':
                vowels++;
        }
    }
    if(vowels < 3) return 0;
    for(int c = s; c < e-1; c++){
        switch(string[c]){
            case 'a':
                if(string[c+1] == 'b') return 0;
                break;
            case 'c':
                if(string[c+1] == 'd') return 0;
                break;
            case 'p':
                if(string[c+1] == 'q') return 0;
                break;
            case 'x':
                if(string[c+1] == 'y') return 0;
                break;
        }
    }
    char prevChar = 0x00;
    for(int c = s; c < e; c++){
        if(prevChar == string[c]) return 1;
        else prevChar = string[c];
    }
    return 0;
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