#include <stdio.h>
#include <stdlib.h>

#define IS_CHAR(text,idx,chr) if(text[idx] == chr) idx++; else return 0;

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

int scan_int(char input[], int *c){
    int ch = *c;
    int val = input[ch++] - '0';
    while(input[ch] >= '0' && input[ch] <= '9'){
        val = (val*10) + (input[ch] - '0');
        ch++;
    }
    *c = ch;
    return val;
}

int scan_mul(char input[], int *c){
    (*c)++;
    int ch = *c;
    int op1 = 0, op2 = 0;
    IS_CHAR(input,ch,'u')
    IS_CHAR(input,ch,'l')
    IS_CHAR(input,ch,'(')
    if(input[ch] >= '0' && input[ch] <= '9'){
        *c = ch;
        op1 = scan_int(input,c);
        ch = *c;
    }else return 0;
    IS_CHAR(input,ch,',')
    if(input[ch] >= '0' && input[ch] <= '9'){
        *c = ch;
        op2 = scan_int(input,c);
        ch = *c;
    }else return 0;
    IS_CHAR(input,ch,')')
    *c = ch;
    return op1 * op2;
}


int main(){
    char* input = read_input();
    int c = 0;
    int total = 0;
    while(input[c]){
        if(input[c] == 'm') total += scan_mul(input,&c);
        else c++;
    }
    printf("Total muls: %d\n",total);
    free(input);
}