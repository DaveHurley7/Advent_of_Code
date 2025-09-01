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

int parse_num(char *data, int *num){
    int val = data[*num] - '0';
    if(data[(*num)-1] == '-') val = -val;
    (*num)++;
    return val;
}

typedef struct _iinfo{
    union{
        struct{
            char cap;
            char dur;
            char flvr;
            char txtr;
            char cal;
        };
        char attr[5];
    };
} ingrd_info;

int count_char(char *seq, char ch){
    int c = 0;
    int count = 0;
    while(seq[c]){
        if(seq[c] == ch) count++;
        c++;
    }
    return count;
}

ingrd_info *get_ingred_info(char *data, int *num){
    int c = 0;
    int num_ingrd = count_char(data,'\n');
    ingrd_info *ingreds = calloc(num_ingrd,sizeof(ingrd_info));
    int ig_i = 0;
    while(data[c]){
        int cap, dur, flvr, txtr, cal;
        if(data[c] && data[c] != '\n'){
            while(data[c] < '0' || data[c] > '9') c++;
            cap = parse_num(data,&c);
            while(data[c] < '0' || data[c] > '9') c++;
            dur = parse_num(data,&c);
            while(data[c] < '0' || data[c] > '9') c++;
            flvr = parse_num(data,&c);
            while(data[c] < '0' || data[c] > '9') c++;
            txtr = parse_num(data,&c);
            while(data[c] < '0' || data[c] > '9') c++;
            cal = parse_num(data,&c);
        }
        ingreds[ig_i++] = (ingrd_info){ cap , dur , flvr , txtr , cal };
        if(data[c] == '\n') c++;
    }
    *num = num_ingrd;
    return ingreds;
}

#define INGRED_ATTR_AMT 4

int **make_value_matrix(ingrd_info *ingreds, int num){
    int **vmtrx = calloc(INGRED_ATTR_AMT,sizeof(int*));
    for(int v = 0; v < INGRED_ATTR_AMT; v++){
        vmtrx[v] = calloc(num,sizeof(int));
        for(int i = 0; i < num; i++){
            vmtrx[v][i] = ingreds[i].attr[v];
        }
    }
    return vmtrx;
}

int calc_score(int **matrix, char *vec, int len){
    int score = 0;
    for(int a = 0; a < INGRED_ATTR_AMT; a++){
        int attr_score = 0;
        for(int i = 0; i < len; i++){
            attr_score += matrix[a][i] * vec[i];
        }
        if(attr_score < 0) attr_score = 0;
        if(!score) score = attr_score;
        else score *= attr_score;
    }
    return score;
}

int get_ingred_units(int **val_mtrx, int num, char *units, char accum_units, int igd_count, int best_score){
    if(igd_count == num-1){
        units[igd_count] = 100-accum_units;
        int score = calc_score(val_mtrx,units,num);
        units[igd_count] = 0;
        return score > best_score ? score : best_score;
    }
    for(int i = 0; i < 100 - accum_units; i++){
        units[igd_count] = i;
        best_score = get_ingred_units(val_mtrx,num,units,accum_units+i,igd_count+1,best_score);
        units[igd_count] = 0;;
    }
    return best_score;
}

int main(){
    content input = read_input();
    int n_ingrds = 0;
    ingrd_info *ingreds = get_ingred_info(input.data,&n_ingrds);
    free(input.data);

    int **val_mtrx = make_value_matrix(ingreds,n_ingrds);
    free(ingreds);

    char *ingred_units = calloc(n_ingrds,sizeof(char));
    int score = get_ingred_units(val_mtrx,n_ingrds,ingred_units,0,0,0);
    printf("Best score: %d\n",score);
    free(ingred_units);

    for(int i = 0; i < n_ingrds; i++){
        free(val_mtrx[i]);
    }
    free(val_mtrx);
}