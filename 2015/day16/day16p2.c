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

#define N_BUCKETS 10

typedef struct _chr_hash{
    char *comps;
    int (*hash_func)(char*);
    int id;
}comp_list;

int dummy_hash(char* key){
    if(strcmp(key,"children") == 0){
        return 0;
    }else if(strcmp(key,"cats") == 0){
        return 1;
    }else if(strcmp(key,"samoyeds") == 0){
        return 2;
    }else if(strcmp(key,"pomeranians") == 0){
        return 3;
    }else if(strcmp(key,"akitas") == 0){
        return 4;
    }else if(strcmp(key,"vizslas") == 0){
        return 5;
    }else if(strcmp(key,"goldfish") == 0){
        return 6;
    }else if(strcmp(key,"trees") == 0){
        return 7;
    }else if(strcmp(key,"cars") == 0){
        return 8;
    }else if(strcmp(key,"perfumes") == 0){
        return 9;
    }
    return -1;
}

void init_hash(comp_list **compounds, int size, int (*hash_key)(char*)){
    *compounds = calloc(1,sizeof(comp_list));
    (*compounds)->comps = calloc(size,sizeof(char));
    (*compounds)->hash_func = hash_key;
}

void hash_add(comp_list *comps, char *key, char val){
    int idx = comps->hash_func(key);
    comps->comps[idx] = val;
}

char hash_getval(comp_list *comps, char *key){
    int idx = comps->hash_func(key);
    return idx < 0 ? 0 : comps->comps[idx];
}

void hash_del(comp_list *comps){
    free(comps->comps);
    comps->hash_func = 0;
    free(comps);
}

void load_kvpairs(comp_list* hashtable){
    char *keys[] = {"children","cats","samoyeds","pomeranians","akitas","vizslas","goldfish","trees","cars","perfumes"};
    char values[] = {3,7,2,3,0,0,5,3,2,1};
    for(int k = 0; k < N_BUCKETS; k++){
        hash_add(hashtable,keys[k],values[k]);
    }
}

void load_unknownpairs(comp_list* hashtable){
    char *keys[] = {"children","cats","samoyeds","pomeranians","akitas","vizslas","goldfish","trees","cars","perfumes"};
    char values[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    for(int k = 0; k < N_BUCKETS; k++){
        hash_add(hashtable,keys[k],values[k]);
    }
}

char *scan_key(char *data, int *i){
    int c = *i;
    while(data[c] < 'a' || data[c] > 'z') c++;
    int start = c;
    while(data[c] >= 'a' && data[c] <= 'z') c++;
    int len = c-start;
    char *key = calloc(len+1,sizeof(char));
    memcpy(key,data+start,len);
    *i = c;
    return key;
}

char scan_val(char *data, int *i){
    int c = *i;
    while(data[c] < '0' || data[c] > '9') c++;
    char val = data[c++] - '0';
    *i = c;
    return val;
}

void hash_filter(comp_list*** arr, int *size, char *key, comp_list* comp, char (*cmp)(char v1, char v2)){
    int items_left = 0;
    for(int a = 0; a < *size; a++){
        if(
            hash_getval((*arr)[a],key) == -1 ||
            cmp(hash_getval(comp,key),hash_getval((*arr)[a],key))
        ) items_left++;
        else{ 
            free((*arr)[a]);
            (*arr)[a] = 0;
        }
    }
    comp_list** new_arr = calloc(items_left,sizeof(comp_list*));
    int new_idx = 0;
    for(int a = 0; a < *size; a++){
        if((*arr)[a]) new_arr[new_idx++] = (*arr)[a]; 
    }
    *arr = new_arr;
    *size = new_idx;
}

char cmp_lt(char v1, char v2){
    return v1 < v2;
}

char cmp_gt(char v1, char v2){
    return v1 > v2;
}

char cmp_eq(char v1, char v2){
    return v1 == v2;
}

int main(){
    content input = read_input();
    comp_list *components;
    init_hash(&components,N_BUCKETS,&dummy_hash);
    load_kvpairs(components);

    int c = 0;
    int aunt_num = 0;
    comp_list **aunts = calloc(500,sizeof(comp_list*));
    for(int a = 0; a < 500; a++){
        comp_list *aunt = calloc(1,sizeof(comp_list));
        init_hash(&aunt,N_BUCKETS,&dummy_hash);
        load_unknownpairs(aunt);
        aunt->id = a+1;
        aunts[a] = aunt;
    }
    while(input.data[c]){
        int s = c;
        while(input.data[c] != '\n') c++;
        while(input.data[s] != ':') s++;
        while(s < c){
            char *key = scan_key(input.data,&s);
            char val = scan_val(input.data,&s);
            hash_add(aunts[aunt_num],key,val);
            char kval = hash_getval(components,key);
            if(input.data[s] == ',') s++;
        }
        aunt_num++;
        s = ++c;
    }
    free(input.data);
    char *keys[] = {"children","cats","samoyeds","pomeranians","akitas","vizslas","goldfish","trees","cars","perfumes"};
    int size = 500;
    for(int c = 0; c < N_BUCKETS; c++){
        if(c == 1 || c == 7) hash_filter(&aunts,&size,keys[c],components,&cmp_lt);
        else if(c == 3 || c == 6) hash_filter(&aunts,&size,keys[c],components,&cmp_gt);
        else hash_filter(&aunts,&size,keys[c],components,&cmp_eq);
    }
    printf("Aunt Sue %d\n",aunts[0]->id);
    for(int i = 0; i < size; i++){
        free(aunts[i]);
    }
    free(aunts);
    hash_del(components);
}