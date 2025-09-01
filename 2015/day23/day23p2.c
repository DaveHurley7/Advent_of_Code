#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    long size;
    char *data;
} content;

content read_input(){
    FILE *input;
    fopen_s(&input,"input.txt","rb");
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1); }
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    content ctnt = { end_pos, calloc(end_pos+1,1) };
    fread(ctnt.data,1,end_pos,input);
    fclose(input);
    return ctnt;
}

int count_char(char data[], char c){
    int count = 0;
    int i = 0;
    while(data[i]){
        if(data[i] == c) count++;
        i++;
    }
    return count;
}

#define JMP 0
#define INC 1
#define HLF 2
#define TPL 3
#define JIE 4
#define JIO 5

#define REG_A 0
#define REG_B 1

typedef struct _ins{
    char opcode;
    char operand;
}instr;

char get_opcode(char data[], int idx){
    if(strncmp(data+idx,"jmp",3) == 0){
        return JMP;
    }else if(strncmp(data+idx,"inc",3) == 0){
        return INC;
    }else if(strncmp(data+idx,"hlf",3) == 0){
        return HLF;
    }else if(strncmp(data+idx,"tpl",3) == 0){
        return TPL;
    }else if(strncmp(data+idx,"jie",3) == 0){
        return JIE;
    }else if(strncmp(data+idx,"jio",3) == 0){
        return JIO;
    }
    puts("Invalid instruction");
    exit(1);
}

char get_jump_val(char data[], int *i){
    char val = 0;
    int idx = *i;
    char is_neg = data[idx++] == '-' ? 1 : 0;
    while(data[idx] >= '0' && data[idx] <= '9'){
        val = (val * 10) + data[idx++] - '0';
    }
    if(is_neg) val = -val;
    *i = idx;
    return val;
}

instr* parse_instructions(char data[], int num){
    instr *ins = calloc(num,sizeof(instr));
    int i = 0;
    int instr_idx = 0;
    while(data[i]){
        char opcode = get_opcode(data,i);
        i += 4;
        char operand = 0;
        char reg = 0;
        if(opcode != JMP){
            reg = data[i++] == 'a' ? REG_A : REG_B;
            if(opcode != JIO && opcode != JIE){
                if(data[i] != '\n'){ puts("Data manipulation instruction expected new line"); exit(2); }
            }else{
                if(data[i] != ','){ puts("Conditional jump expected two operands"); exit(3); }
                i+=2;
                operand = get_jump_val(data,&i);
            }
        }else{
            operand = get_jump_val(data,&i);
        }
        i++;
        ins[instr_idx++] = (instr){ opcode | (reg << 4) , operand};
    }
    return ins;
}

int main(){
    content input = read_input();
    int n_instr = count_char(input.data,'\n');
    instr *instuctions = parse_instructions(input.data,n_instr);
    free(input.data);
    int regs[2] = {1,0};
    int i = 0;
    while(i < n_instr){
        char opcode = instuctions[i].opcode&7;
        switch(opcode){
            case JMP:
                i += instuctions[i].operand - 1;
                break;
            case INC:
                regs[instuctions[i].opcode>>4]++;
                break;
            case HLF:
                regs[instuctions[i].opcode>>4]/=2;
                break;
            case TPL:
                regs[instuctions[i].opcode>>4]*=3;
                break;
            case JIE:
                if((regs[instuctions[i].opcode>>4]&1) == 0){
                    i += instuctions[i].operand - 1;
                }
                break;
            case JIO:
                if(regs[instuctions[i].opcode>>4] == 1){
                    i += instuctions[i].operand - 1;
                }
                break;
        }
        i++;
    }
    free(instuctions);
    printf("Value in B: %d",regs[REG_B]);
}