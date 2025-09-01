#include <stdio.h>
#include <stdlib.h>

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

typedef struct{
    int before;
    int after;
} pair;


int have_pairs = 0;

int strtoint(char *text, int *idx){
    int val = 0;
    while(text[*idx] >= '0' && text[*idx] <= '9'){
        val = (val*10) + (text[*idx] - '0');
        (*idx)++;
    }
    return val;
}

void get_pair(char input[], int *chp, pair *pairs, int *prp){
    int before = strtoint(input,chp);
    if(input[*chp] != '|'){
        printf("Expected |"); exit(1);
    }else (*chp)++;
    int after = strtoint(input,chp);
    pairs[*prp] = (pair){ before, after };
    (*prp)++;
    *chp += 2;
}

pair* get_pairs(char input[], int *pair_count, int *cursor){
    int count = 0;
    int i = 0;
    while(1){
        if(input[i] == '\r'){
            count++;
            if(input[i+2] == '\r') break;
        }
        i++;
    }
    pair *pairs = (pair*) calloc(count,sizeof(pair));
    i = 0;
    int pidx = 0;
    while(pidx < count)
        get_pair(input,&i,pairs,&pidx);
    *cursor = i+2;
    *pair_count = count;
    return pairs;
}

int updates_len(char input[], int *chp, char endchar){
    int i = *chp;
    int count = 0;
    while(input[i] != endchar){
        if(input[i] == ',') count++;
        i++;
    }
    return count+1;
}

int array_index(int nlist[], int len, int item){
    for(int i = 0; i < len; i++){
        if(nlist[i] == item) return i;
    }
    return -1;
}

typedef struct{
    int val;
    int idx;
}prev_bef_num;

int check_if_valid_update(char input[], int *chp, pair *pairs, int pr_amt){
    int len = updates_len(input,chp,'\r');
    int updates[len] = {};
    int uidx = 0;
    while(1){
        int num = strtoint(input,chp);
        updates[uidx++] = num;
        if(input[*chp] == '\r') break;
        if(input[*chp] != ','){
            printf("Expected a ,");
            exit(1);
        }
        (*chp)++;
    }
    prev_bef_num pbn = {0, -1};
    for(int p = 0; p < pr_amt; p++){
        int bef_idx;
        if(pairs[p].before != pbn.val){
            bef_idx = array_index(updates,len,pairs[p].before);
            if(bef_idx == -1) continue;
            pbn.val = pairs[p].before;
            pbn.idx = bef_idx;
        }else{
            bef_idx = pbn.idx;
        }
        int afr_idx = array_index(updates,len,pairs[p].after);
        if(afr_idx == -1) continue;
        if(bef_idx > afr_idx) return 0;
    }
    if(len&1 != 1){
        printf("Expected a list with odd number of items"); exit(1); 
    }
    return updates[len/2];
}

int main(){
    char* input = read_input();
    int pr_amt = 0;
    int cursor = 0;
    pair *pairs = get_pairs(input,&pr_amt,&cursor);
    int total = 0;
    have_pairs = 1;
    while(input[cursor]){
        while(input[cursor] != '\r')
            total += check_if_valid_update(input,&cursor,pairs,pr_amt);
        cursor+=2;
    }
    printf("\nAccumulation of medians %d\n",total);
    free(pairs);
    free(input);
}