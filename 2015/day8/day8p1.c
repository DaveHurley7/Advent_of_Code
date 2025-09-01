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

int main(){
    content input = read_input();
    int c = 0;
    int total_chars = 0;
    int actual_chars = 0;
    int mem_chars = 0;
    while(input.data[c]){
        if(input.data[c] == '"'){
            actual_chars++;
        }else if(input.data[c] == '\\'){
            c++;
            switch(input.data[c]){
                case '"': case '\\':
                    actual_chars+=2;
                    mem_chars++;
                    break;
                case 'x':
                    c+=2;
                    actual_chars+=4;
                    mem_chars++;
                    break;
            }
        }else if(input.data[c] == '\n'){
            total_chars += actual_chars - mem_chars;
            actual_chars = 0;
            mem_chars = 0;
        }else{
            actual_chars++;
            mem_chars++;
        }
        c++;
    }
    printf("Total difference: %d\n",total_chars);
    free(input.data);
}