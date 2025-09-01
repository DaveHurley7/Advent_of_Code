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

typedef enum{
    JSON_OBJ,
    JSON_ARR,
    JSON_STR,
    JSON_INT
}json_type;

typedef struct{
    json_type type;
    int val;
}json_entity;

json_entity parse_num(char *input, int *i){
    int c = *i;
    int is_neg = input[c-1] == '-' ? 1 : 0;
    int val = 0;
    while(input[c] >= '0' && input[c] <= '9'){
        val = val*10 + (input[c++]-'0');
    }
    if(is_neg) val = -val;
    *i = c;
    printf("Num: %d\n",val);
    return (json_entity){ JSON_INT , val };
}

json_entity parse_str_for_red(char *input, int *i){
    int c = *i;
    int s = c;
    while(input[c] && input[c] != '"'){
        c++;
    }
    *i = ++c;
    return (json_entity){ JSON_STR , strncmp(input+s,"red",3) == 0 };
}

int json_parser(char *data, int *idx);
json_entity json_parser_generic(char *data, int *idx);
json_entity json_parse_array(char *data, int *idx);
json_entity json_parse_object(char *data, int *idx);

json_entity json_parse_array(char *data, int *idx){
    int c = *idx;
    int total = 0;
    while(data[c] != ']'){
        if(data[c] != ','){
            *idx = c;
            json_entity res = json_parser_generic(data,idx);
            c = *idx;
            if(res.type != JSON_STR) total += res.val;
        }else c++;
    }
    *idx = ++c;
    return (json_entity){ JSON_ARR , total };
}

json_entity json_parse_object(char *data, int *idx){
    int c = *idx;
    int total = 0;
    int ignore = 0;
    while(data[c] != '}'){
        if(data[c] == '"') c++;
        *idx = c;
        parse_str_for_red(data,idx);
        c = *idx;
        if(data[c] == ':') c++;
        if(data[c] != ','){
            *idx = c;
            json_entity res = json_parser_generic(data,idx);
            c = *idx;
            if(res.type == JSON_STR && res.val) ignore = 1;
            else total += res.val;
        }
    }
    *idx = ++c;
    return (json_entity){ JSON_OBJ , ignore ? 0 : total };
}

int json_parser(char *data, int *idx){
    return json_parser_generic(data,idx).val;
}

json_entity json_parser_generic(char *data, int *idx){
    int c = *idx;
    json_entity res = {0};
    if(data[c] == '['){
        *idx = ++c;
        res = json_parse_array(data,idx);
    }else if(data[c] == '{'){
        *idx = ++c;
        res = json_parse_object(data, idx);
    }else if(data[c] == '"'){
        *idx = ++c;
        res = parse_str_for_red(data,idx);
    }else if((data[c] >= '0' && data[c] <= '9') || data[c] == '-'){
        *idx = data[c] == '-' ? c+1 : c;
        res = parse_num(data,idx);
    }
    c = *idx;
    return res;
}

int main(){
    content input = read_input();
    int i = 0;
    int total = json_parser(input.data,&i);
    printf("Sum of numbers: %d\n",total);
    free(input.data);
}