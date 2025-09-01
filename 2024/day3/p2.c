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

int scan_enable_cmd(char input[], int *c){
    (*c)++;
    int ch = *c;
    int ret;
    IS_CHAR(input,ch,'o')
    if(input[ch] == '('){
        ch++;
        IS_CHAR(input,ch,')')
        ret = 1;
    }else if(input[ch] == 'n'){
        IS_CHAR(input,ch,'\'')
        IS_CHAR(input,ch,'t')
        IS_CHAR(input,ch,'(')
        IS_CHAR(input,ch,')')
        ret = 0;
    }else return 2; //Arbitrary number to indicate no do() or don't()
    *c = ch;
    return ret;
}

int main(){
    char* input = read_input();
    int c = 0;
    int total = 0;
    int mul_enabled = 1;
    while(input[c]){
        if(mul_enabled && input[c] == 'm') total += scan_mul(input,&c);
        else if(input[c] == 'd'){
            int cmd = scan_enable_cmd(input,&c);
            switch(cmd){
                case 0: mul_enabled = 0; break;
                case 1: mul_enabled = 1; break;
                default: break;
            }
        }else c++;
    }
    printf("Total muls: %d\n",total);
    free(input);
}