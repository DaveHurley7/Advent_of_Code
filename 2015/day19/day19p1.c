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

int find_combos(mlcl orig_mlcl, chem_swap* chems, int n_chswp){
    int cap = 50;
    int amt = 0;
    mlcl *mlcls = calloc(cap,sizeof(mlcl));
    int om_size = orig_mlcl.size;
    for(int c = 0; c < n_chswp; c++){
        for(int s = 0; s < om_size; s++){
            if(orig_mlcl.items[s] == chems[c].c_id){
                int mlcl_len = om_size + chems[c].rpl_len-1;
                mlcl *new_ml = calloc(1,sizeof(mlcl));
                *new_ml = (mlcl){ mlcl_len , mlcl_len , calloc(mlcl_len,sizeof(unsigned char)) };
                memcpy(new_ml->items,orig_mlcl.items,s);
                memcpy(new_ml->items+s,chems[c].replace,chems[c].rpl_len);
                int ofst = s + chems[c].rpl_len;
                memcpy(new_ml->items+ofst,orig_mlcl.items+s+1,orig_mlcl.size-s);
                if(mlcl_exists(mlcls,new_ml,amt)){
                    free(new_ml);
                }else{
                    mlcls = add_mlcl(mlcls,new_ml,&amt,&cap);
                }
            }
        }
    }
    free(mlcls);
    return amt;
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
    int combos = find_combos(init_mlcl,chem_swaps,n_chemswap);
    printf("Possible combinations found %d\n",combos);
    free(chem_swaps);
    free(init_mlcl.items);
    free(elems->elems);
    free(elems);
}