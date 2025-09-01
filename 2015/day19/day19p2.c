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

#define ELEM_NAME_SIZE 3

typedef struct _einf{
    unsigned char id;
    char name[3];
} elem_info;

typedef struct _el_list{
    int size;
    int cap;
    elem_info *elems;
}elem_list;

void resize_list(elem_list *elems){
    elems->elems = realloc(elems->elems,sizeof(elem_info)*(elems->size+20));
    memset(elems->elems+elems->size,0,20);
}

int add_elem(elem_list *elems, char *elem){
    if(elems->size >= elems->cap){
        resize_list(elems);
    }
    elems->elems[elems->size] = (elem_info) { elems->size };
    memcpy(elems->elems[elems->size].name,elem,3);
    int i = elems->size;
    while(i > 0 && strcmp(elem,elems->elems[i-1].name) == -1){
        elem_info tmp = elems->elems[i];
        elems->elems[i] = elems->elems[i-1];
        elems->elems[i-1] = tmp;
        i--;
    }
    return elems->size++;    
}

int in_list(elem_info *list, char *elem, int s, int e){
    while(s <= e){
        int m = (s+e)/2;
        int cmp = strcmp(elem,list[m].name);
        if(cmp == 0) return list[m].id;
        else if(cmp == -1) e = m-1;
        else s = m+1;
    }
    return -1;
}

int scan_elem(char *data, int *idx, elem_list *elems){
    char elem[3] = {0};
    int c = *idx;
    elem[0] = data[c++];
    if(data[c] != 'e' && (data[c] >= 'a' && data[c] <= 'z')){
        elem[1] = data[c++];
    }
    *idx = c;
    int e_idx = in_list(elems->elems,elem,0,elems->size);
    if(e_idx == -1) e_idx = add_elem(elems,elem);
    return e_idx;
}

typedef struct chem_swp{
    unsigned char c_id;
    unsigned char rpl_len;
    unsigned char replace[10];
} chem_swap;

int count_chem_swaps(char *data){
    int c = 0;
    int count = 0;
    while(data[c]){
        if(data[c] == '\n'){
            if(data[c-1] == '\n') break;
            else count++;
        }
        c++;
    }
    return count;
}

typedef struct _mlcl{
    int size;
    int cap;
    unsigned char *items;
}mlcl;

void resize(mlcl *mlist){
    mlist->cap += 50;
    mlist->items = realloc(mlist->items,sizeof(mlcl)*(mlist->cap));
    memset(mlist->items+mlist->size,0,sizeof(mlcl)*50);
}

char cmp_mlcl(mlcl m1, mlcl m2){
    if(m1.size != m2.size){
        return (m1.size < m2.size) ? -1 : 1;
    }
    int sz = m1.size;
    for(int s = 0; s < sz; s++){
        if(m1.items[s] != m2.items[s]){
            return (m1.items[s] < m2.items[s]) ? -1 : 1;
        }
    }
    return 0;
}

char mlcl_exists(mlcl *mlcls, mlcl *check, int size){
    if(!size) return 0;
    int s = 0, e = size;
    while(s <= e){
        int mid = (s+e)/2;
        int cmp = cmp_mlcl(*check,mlcls[mid]);
        if(cmp==0) return 1;
        else if(cmp==1) s = mid+1;
        else e = mid-1;
    }
    return 0;
}

mlcl *add_mlcl(mlcl *mlcls, mlcl *new, int *size, int *cap){
    mlcl *list = mlcls;
    if(*size >= *cap){
        *cap += 50;
        mlcl *new_list = calloc(*cap,sizeof(mlcl));
        memcpy(new_list,mlcls,(*size)*sizeof(mlcl));
        list = new_list;
        free(mlcls);
    }
    int idx = (*size)++;
    list[idx] = *new;
    while(idx > 0 && cmp_mlcl(list[idx],list[idx-1]) == -1){
        mlcl tmp = list[idx];
        list[idx] = list[idx-1];
        list[idx-1] = tmp;
        idx--;
    }
    return list;
}

char cmp_chswap(chem_swap swp1, chem_swap swp2){
    if(swp1.rpl_len != swp2.rpl_len) 
        return swp1.rpl_len < swp2.rpl_len ? -1 : 1;
    int e = 0;
    while(swp1.replace[e] == swp2.replace[e]) e++;
    return swp1.replace[e] < swp2.replace[e] ? -1 : 1;
}

typedef unsigned char uchar;

char mlcl_rpl_cmp(mlcl mchk, chem_swap chswp){
    if(mchk.size != chswp.rpl_len ) 
        return mchk.size < chswp.rpl_len ? -1 : 1;
    int e = 0;
    while(mchk.items[e] && mchk.items[e] == chswp.replace[e]) e++;
    if(!mchk.items[e]) return 0;
    return mchk.items[e] < chswp.replace[e] ? -1 : 1;
}

uchar check_rules(chem_swap *swaps, mlcl check, int n_swps){
    int i = n_swps;
    while(i >= 0 && mlcl_rpl_cmp(check,swaps[i]) < 0) i--;
    return mlcl_rpl_cmp(check,swaps[i]) == 0;
}

typedef struct _rule_res{
    char count;
    chem_swap* rule;
}rule_res;

char rule_startswith(chem_swap rule, unsigned char seq[], int seq_sz){
    if(seq_sz > rule.rpl_len) return 0;
    for(int i = 0; i < seq_sz; i++){
        if(seq[i] != rule.replace[i]) return 0;
    }
    return 1;
}


rule_res couldDerive(unsigned char seq[], int seq_sz, chem_swap *swaps, int n_swaps, uchar memo[]){
    if(seq_sz == 1){
        int m = 0;
        while(memo[m]){
            if(seq[0] == memo[m]) return (rule_res){ 0 , 0 };
        } 
    }
    int count = 0;
    chem_swap *rule = 0;
    for(int s = 0; s < n_swaps; s++){
        if(rule_startswith(swaps[s],seq,seq_sz)){
            count++;
            rule = &swaps[s];
        }
    }
    return (rule_res){ count , rule };
}

uchar deriveSequence(mlcl mlcl_make, chem_swap *swaps, int n_swaps, int *mcl_c, int *steps, uchar prev, uchar elem_memo[]){
    if(!mlcl_make.items[*mcl_c]) 
        return 0;
    int seq_sz = 0;
    uchar seq[10] = {0};
    int chr_pos = *mcl_c;
    if(prev) seq[seq_sz++] = prev;
    else{
        seq[seq_sz++] = mlcl_make.items[*mcl_c];
        rule_res res_elem = couldDerive(seq,seq_sz,swaps,n_swaps,elem_memo);
        (*mcl_c)++;
        if(res_elem.count == 0) return seq[0];
    }
    seq_sz++;
    char use_last = 0;
    while(1){
        seq[seq_sz-1] = deriveSequence(mlcl_make,swaps,n_swaps,mcl_c,steps,use_last ? seq[seq_sz-1] : 0,elem_memo);
        rule_res res_seq = couldDerive(seq,seq_sz,swaps,n_swaps,elem_memo);
        if(res_seq.count){
            use_last = 0;
            chr_pos = *mcl_c;
            if(res_seq.count == 1 && seq_sz == res_seq.rule->rpl_len){
                (*steps)++;
                printf("Have mol, %d\n",chr_pos);
                return res_seq.rule->c_id;
            }   
            seq_sz++;
        }else use_last = 1;
    }
}

int find_steps(mlcl mlcl_make, chem_swap *swaps, int n_swaps, elem_list *elems){
    int steps = 0;
    int mcl_c = 0;
    uchar elem_memo[10] = {0};
    uchar seq[10] = {0};
    int seq_i = 1;
    char use_last = 0;
    //Get elem info
    char elem[3] = {'e',0};
    int elem_id = in_list(elems->elems,elem,0,elems->size);
    while(seq[0] != elem_id){
        seq[seq_i-1] = deriveSequence(mlcl_make,swaps,n_swaps,&mcl_c,&steps,use_last ? seq[seq_i-1] : 0,elem_memo);
        rule_res res = couldDerive(seq,seq_i,swaps,n_swaps,elem_memo);
        if(res.count){
            use_last = 0;
            if(res.count == 1 && seq_i == res.rule->rpl_len){ 
                memset(seq,0,10);
                seq[0] = res.rule->c_id;
                seq_i = 1;
                steps++;
            }
            seq_i++;
        }else{
            use_last = 1;
        }
        
    }
    //uchar res = deriveSequence(mlcl_make,swaps,n_swaps,&mcl_c,&steps,0,elem_memo);
    return steps;
}

int main(){
    content input = read_input();
    elem_list *elems = calloc(1,sizeof(elem_list));
    int n_chemswap = count_chem_swaps(input.data);
    *elems = (elem_list){ 0 , 20 , calloc(20,sizeof(elem_info)) };
    chem_swap *chem_swaps = calloc(n_chemswap,sizeof(chem_swap));
    add_elem(elems,"\0\0\0");
    int c = 0;
    int chem_idx = 0;
    while(input.data[c] != '\n'){
        if(input.data[c] < 'A' && input.data[c] > 'Z' && input.data[c] != 'e'){
            c++;
            continue;
        }
        chem_swaps[chem_idx] = (chem_swap){ 0 , 0 , {0}};
        chem_swaps[chem_idx].c_id = (unsigned char)scan_elem(input.data,&c,elems);
        if(input.data[c] == ' ') c += 4;
        unsigned char rpl_len = 0;
        while(input.data[c] != '\n'){
            unsigned char chem = scan_elem(input.data,&c,elems);
            chem_swaps[chem_idx].replace[rpl_len++] = chem;
        }
        c++;
        chem_swaps[chem_idx++].rpl_len = rpl_len;
    }
    c++;
    mlcl init_mlcl = { 0 , 50 , calloc(50,sizeof(char)) };
    while(input.data[c] != '\n'){
        unsigned char elem_id = scan_elem(input.data,&c,elems);
        if(init_mlcl.size >= init_mlcl.cap){
            resize(&init_mlcl);
        }
        init_mlcl.items[init_mlcl.size++] = elem_id;
    }
    free(input.data);

    for(int s = 1; s < n_chemswap; s++){
        int e = s;
        while(e > 0 && cmp_chswap(chem_swaps[e],chem_swaps[e-1]) == -1){
            chem_swap tmp = chem_swaps[e];
            chem_swaps[e] = chem_swaps[e-1];
            chem_swaps[e-1] = tmp;
            e--;
        }
    }
    int min_steps = find_steps(init_mlcl,chem_swaps,n_chemswap,elems);
    printf("Minimum steps: %d",min_steps);
    free(chem_swaps);
    free(init_mlcl.items);
    free(elems->elems);
    free(elems);
}