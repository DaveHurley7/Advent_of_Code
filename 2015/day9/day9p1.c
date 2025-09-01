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
    char **list;
}charlist;

charlist split(char data[], int *start, char sep, char end){
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
    return (charlist){ n_items , tokens };
}

char* scan_city(char str[], int *idx){
    int e = *idx;
    int s = e;
    while(str[e] != ' ') e++;
    char* city = calloc(1,sizeof(char)*(e-s+1));
    memcpy(city,str+s,e-s);
    *idx = e;
    return city;
}

typedef struct _cll{
    char *city;
    struct _cll* next;
}city;

char inlist(city list, city *item){
    if(!list.city) return -1;
    city currcity = list;
    int i = 0;
    if(!currcity.next){
        if(!strcmp(currcity.city,item->city)) return 0;
    }
    while(currcity.next){
        if(!strcmp(currcity.city,item->city)) return i;
        i++;
        currcity = *currcity.next;
    }
    return -1;
}

char add(city *list, city *item){
    if(!list->city){
        list->city = item->city;
        return 0;
    }
    int c_idx;
    if((c_idx = inlist(*list,item)) < 0){
        city *currcity = list;
        while(currcity->next) currcity = currcity->next;
        currcity->next = item;
    }
    return c_idx;
}

int getlen(city llist){
    if(!llist.city) return 0;
    if(!llist.next) return 1;
    city currcity = llist;
    int len = 1;
    while(currcity.next){
        len++;
        currcity = *currcity.next;
    }
    return len;
}

void llisttoarr(city llist, char*** arr){
    int listlen = getlen(llist);
    *arr = (char**) calloc(listlen,sizeof(char**));
    city currcity = llist;
    int i = 0;
    for(; i < listlen-1; i++){
        (*arr)[i] = currcity.city;
        currcity = *currcity.next;
    }
    (*arr)[i] = currcity.city;
}

void clear_llist(city list){
    if(list.next){
        clear_llist(*list.next);
        free(list.next);
    }
}

#define LAZY_CHECK

void extract_cities(content ctnt, char*** locs, int *nlocs){
    int c = 0;
    int n_city = 0;
    city citylist = {0};
    while(ctnt.data[c]){
        if(ctnt.data[c] >= 65 && ctnt.data[c] <= 90){
            int city_sidx = c;
            char* cityname = scan_city(ctnt.data,&c);
            city *item = calloc(1,c-city_sidx+1);
            *item = (city){ cityname , 0 };
            int poss_idx;
            if((poss_idx = add(&citylist,item)) >= 0) 
                n_city++;

            #ifdef LAZY_CHECK
            if(poss_idx > 0) break;
            #endif
        }
        c++;
    }
    llisttoarr(citylist,locs);
    clear_llist(citylist);
    *nlocs = n_city;
}

char get_item_idx(char **city_names, char *cityname, int n_cities){
    for(int c = 0; c < n_cities; c++){
        if(strcmp(city_names[c],cityname) == 0) return c;
    }
    return -1;
}

char scan_dist(char* data, int *idx){
    int c = *idx;
    char val = 0;
    while(data[c] >= 0x30 && data[c] <= 0x39){
        val = (val*10) + (data[c++]-'0');
    }
    *idx = c;
    return val;
}

unsigned char** get_city_dists(content input, char **city_names, int n_cities){
    unsigned char **adj_mtrx = calloc(n_cities,sizeof(char*));
    for(int c = 0; c < n_cities; c++){
        adj_mtrx[c] = calloc(n_cities,sizeof(char));
    }
    int c = 0;
    char city1 = -1, city2 = -1;
    unsigned char dist = 0;
    while(input.data[c]){
        while(input.data[c] < 65 || input.data[c] > 90) c++;
        char* cityname1 = scan_city(input.data,&c);
        city1 = get_item_idx(city_names,cityname1,n_cities);

        while(input.data[c] < 65 || input.data[c] > 90) c++;
        char* cityname2 = scan_city(input.data,&c);
        city2 = get_item_idx(city_names,cityname2,n_cities);

        while(input.data[c] < 0x30 || input.data[c] > 0x39) c++;
        dist = scan_dist(input.data,&c);
        c++;
        if(city1 >= 0 && city2 >= 0 && dist > 0){
            adj_mtrx[city1][city2] = dist;
            adj_mtrx[city2][city1] = dist;
        }
        free(cityname1);
        free(cityname2);
    }
    return adj_mtrx;
}

int in_path(char *path, int c_idx, char amt){
    for(int i = 0; i < amt; i++){
        if(path[i] == c_idx) return 1;
    }
    return 0;
}

unsigned int find_shortest(unsigned char** adj_mtrx, int n_cities, char *path, char depth, unsigned int curr_dist, unsigned int min_dist){
    if(depth == n_cities) 
        return curr_dist < min_dist ? curr_dist : min_dist;
    for(int c = 0; c < n_cities; c++){
        if(in_path(path,c,depth)) continue;
        path[depth] = c;
        if(depth){
            int prev_city = path[depth-1];
            min_dist = find_shortest(adj_mtrx, n_cities, path, depth+1, curr_dist + adj_mtrx[prev_city][c],min_dist); 
        }else{
            min_dist = find_shortest(adj_mtrx, n_cities, path, depth+1, 0, min_dist);
        }
        path[depth] = 0x80;
    }
    return min_dist;
}

int main(){
    content input = read_input();
    char **cities = 0;
    int n_cities = 0;
    extract_cities(input,&cities,&n_cities);
    unsigned char **adj_mtrx = get_city_dists(input,cities,n_cities);
    char *path = calloc(1,sizeof(char)); 
    for(int i = 0; i < n_cities; i++){
        path[i] = 0x80;
    }
    unsigned int shortest_path = find_shortest(adj_mtrx,n_cities,path,0,0,-1);
    printf("Shortest path: %d\n",shortest_path);
    for(int row = 0; row < n_cities; row++){
        free(adj_mtrx[row]);
    }
    free(adj_mtrx);
    free(input.data);
}