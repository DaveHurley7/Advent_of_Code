#include <stdio.h>
#include <stdlib.h>

char* read_input(){
    FILE *input = fopen("input.txt","rb");
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1);}
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    char *content = (char*) calloc(end_pos+1,1);
    fread(content,1,end_pos,input);
    fclose(input);
    return content;
}


int main(){
    char* input = read_input();
    
    free(input);
}