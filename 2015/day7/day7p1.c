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
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1);}
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    content ctnt = { end_pos, calloc(end_pos+1,1) };
    fread(ctnt.data,1,end_pos,input);
    fclose(input);
    return ctnt;
}

int count_nodes(content in){
    int c = 0;
    int num = 0;
    while(in.data[c]){
        if(in.data[c] == '\n') num++;
        c++;
    }
    return num;
}

typedef enum _e{
    VALUE,
    NOT,
    LSHIFT,
    RSHIFT,
    AND,
    OR
}exptype;

typedef struct _n{
    char wire[2];
    char expr;
    union _attr1{
        char wire[2];
        unsigned short signal;
    }op1;
    union _attr2{
        char wire[2];
        unsigned short signal;
        short shift_amt;
    }op2;
}node;

int count_items(char data[], int i, char sep, char end){
    int items = 0;
    while(data[i] != end){
        if(data[i] == sep) items++;
        i++;
    }
    return items+1;
}

typedef struct _list{
    char len;
    char **tokens;
}tokenlist;

tokenlist split(char data[], int *start, char sep, char end){
    int ofst = *start;
    int n_items = count_items(data,*start,sep,end);
    char **tokens = calloc(n_items,sizeof(char*));
    int delimit = *start;
    int t_idx = 0;
    while(data[delimit] != end){
        while(data[delimit] != end && data[delimit] != sep) delimit++;
        int itemlen = delimit - ofst;
        char* token = calloc(1,itemlen+1);
        memcpy_s(token,itemlen,data+ofst,itemlen);
        tokens[t_idx++] = token;
        if(data[delimit] == sep)
            ofst = ++delimit;
    }
    *start = delimit;
    return (tokenlist){ n_items , tokens };
}

char get_valtype(char valtoken[]){
    if(valtoken[0] >= '0' && valtoken[0] <= '9') return 0;
    if(valtoken[0] >= 'a' && valtoken[0] <= 'z') return 1;
    puts("Error: Cannot determine if value is signal or wire");
    exit(1);
}

int get_nodeidx(char wire[]){
    if(!wire[1])
        return wire[0] - 'a';
    return ((wire[0] - '`') * 26) + (wire[1] - 'a');
}

void wirenamef(char wire[], char wirename[]){
    if(!wirename[1]) memcpy_s(wire,1,wirename,1);
    else memcpy_s(wire,2,wirename,2);
}

char verify_cmd(char token[], char cmd[]){
    int i = 0;
    while(cmd[i] && token[i]){
        if(cmd[i] != token[i]) return 0;
        i++;
    }
    return !(cmd[i] && token[i]);
}

void parse_nodes(content in, node *nodes){
    int c = 0;
    while(in.data[c]){
        tokenlist tokenstream = split(in.data,&c,' ','\n');
        node n = {0,0,0,0};
        switch(tokenstream.len){
            case 3:{
                wirenamef(n.wire,tokenstream.tokens[2]);
                n.expr = get_valtype(tokenstream.tokens[0]);
                if(n.expr) wirenamef(n.op1.wire,tokenstream.tokens[0]);
                else n.op1.signal = atoi(tokenstream.tokens[0]);
                break;
            }
            case 4:{
                if(!verify_cmd(tokenstream.tokens[0],"NOT")){
                    puts("4 word instruction, expected first as NOT");
                    exit(1);
                }
                wirenamef(n.wire,tokenstream.tokens[3]);
                n.expr = get_valtype(tokenstream.tokens[1]);
                if(n.expr) wirenamef(n.op1.wire,tokenstream.tokens[1]);
                else n.op1.signal = atoi(tokenstream.tokens[1]);
                n.expr += NOT<<4;
                break;
            }
            case 5:{
                wirenamef(n.wire,tokenstream.tokens[4]);
                char op = 0;
                char and = 0, or = 0, lshift = 0, rshift = 0;
                if((and = verify_cmd(tokenstream.tokens[1],"AND"))){}
                else if((or = verify_cmd(tokenstream.tokens[1],"OR"))){}
                else if((lshift = verify_cmd(tokenstream.tokens[1],"LSHIFT"))){}
                else if((rshift = verify_cmd(tokenstream.tokens[1],"RSHIFT"))){}
                else {puts("5 word instruction, expected operator AND, OR, LSHIFT or RSHIFT"); exit(1); }
                if(and || or){
                    op = and ? AND : OR;
                    n.expr = get_valtype(tokenstream.tokens[0]);
                    if(n.expr) wirenamef(n.op1.wire,tokenstream.tokens[0]);
                    else n.op1.signal = atoi(tokenstream.tokens[0]);
                    char op2type = get_valtype(tokenstream.tokens[2]);
                    if(op2type) wirenamef(n.op2.wire,tokenstream.tokens[2]);
                    else n.op2.signal = atoi(tokenstream.tokens[2]);
                    n.expr += (op2type<<2) + (op<<4);
                }else if(lshift || rshift){
                    op = lshift ? LSHIFT : RSHIFT;
                    n.expr = get_valtype(tokenstream.tokens[0]);
                    if(n.expr) wirenamef(n.op1.wire,tokenstream.tokens[0]);
                    else n.op1.signal = atoi(tokenstream.tokens[0]);
                    n.op2.shift_amt = atoi(tokenstream.tokens[2]);
                    n.expr += op<<4;
                }
                break;
            }
        }
        for(int ti = 0; ti < tokenstream.len; ti++){
            free(tokenstream.tokens[ti]);
        }
        free(tokenstream.tokens);
        int n_idx = get_nodeidx(n.wire);
        nodes[n_idx] = n;
        c++;
    }
}

int main(){
    content input = read_input();
    int node_count = count_nodes(input);
    node *nodes = calloc(node_count,sizeof(node));
    parse_nodes(input,nodes);
    free(input.data);
    int n = 0;
    while(nodes[0].expr){
        switch(nodes[n].expr>>4){
            case VALUE:{
                    if(nodes[n].expr){
                        int node_idx = get_nodeidx(nodes[n].op1.wire);
                        if(!nodes[node_idx].expr){
                            nodes[n].op1.signal = nodes[node_idx].op1.signal;
                            nodes[n].expr = 0;
                        }
                    }
                }
                break;
            case NOT:{
                    if(nodes[n].expr&3){
                        int node_idx = get_nodeidx(nodes[n].op1.wire);
                        if(!(nodes[node_idx].expr&3)){
                            nodes[n].op1.signal = ~(nodes[node_idx].op1.signal);
                            nodes[n].expr = 0;
                        }
                    }else{
                        nodes[n].op1.signal = ~nodes[n].op1.signal;
                        nodes[n].expr = 0;
                    }
                }
                break;
            case AND: case OR:{
                    if(nodes[n].expr&3){
                        int node_idx = get_nodeidx(nodes[n].op1.wire);
                        if(!nodes[node_idx].expr){
                            nodes[n].op1.signal = nodes[node_idx].op1.signal;
                            nodes[n].expr = nodes[n].expr & 0xfc;  //Only clear last 2 bits
                        };
                    }
                    if((nodes[n].expr>>2)&3){
                        int node_idx = get_nodeidx(nodes[n].op2.wire);
                        if(!nodes[node_idx].expr){
                            nodes[n].op2.signal = nodes[node_idx].op1.signal;
                            nodes[n].expr = nodes[n].expr & 0xf3;  //Only clear last 3rd and 4th bits
                        };
                    }
                    if(!(nodes[n].expr&0xf)){  //Check if lower nibble is all 0s
                        if(nodes[n].expr>>4 == AND) nodes[n].op1.signal &= nodes[n].op2.signal;
                        else nodes[n].op1.signal |= nodes[n].op2.signal;
                        nodes[n].expr = 0;
                        nodes[n].op2.signal = 0;
                    }
                }
                break;
            case LSHIFT: case RSHIFT:{
                    if(nodes[n].expr&3){
                        int node_idx = get_nodeidx(nodes[n].op1.wire);
                        if(!nodes[node_idx].expr){
                            nodes[n].op1.signal = nodes[node_idx].op1.signal;
                            nodes[n].expr = nodes[n].expr & 0xfc;  //Only clear last 2 bits
                        }else break;
                    }
                    if(nodes[n].expr>>4 == LSHIFT) nodes[n].op1.signal <<= nodes[n].op2.shift_amt;
                    else nodes[n].op1.signal >>= nodes[n].op2.shift_amt;
                    nodes[n].expr = 0;
                    nodes[n].op2.signal = 0;
                }
                break;
        }
        n++;
        n %= node_count;
    }
    printf("Wire 'a' signal: %d",nodes[0].op1.signal);
    free(nodes);
}