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

int main(){
    content input = read_input();
    comp_list *compounds;
    init_hash(&compounds,N_BUCKETS,&dummy_hash);
    load_kvpairs(compounds);

    int c = 0;
    int aunt_num = 0;
    int comps_to_match = 3;
    while(input.data[c]){
        int s = c;
        aunt_num++;
        int num_comps = 0;
        while(input.data[c] != '\n') c++;
        while(input.data[s] != ':') s++;
        while(s < c){
            char *key = scan_key(input.data,&s);
            char val = scan_val(input.data,&s);
            if(hash_getval(compounds,key) == val) num_comps++;
            if(input.data[s] == ',') s++;
        }
        if(num_comps == comps_to_match) break;
        c++;
    }
    printf("Aunt Sue num %d\n",aunt_num);
    free(input.data);
    hash_del(compounds);
}