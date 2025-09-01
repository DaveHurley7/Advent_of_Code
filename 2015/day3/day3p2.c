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

typedef struct{
    short int x;
    short int y;
}housepos;

typedef struct{
    housepos *list;
    int capacity;
    int len;
}houselist;

int cmp_houses(housepos searchhouse, housepos midhouse){
    if(searchhouse.x == midhouse.x){
        if(searchhouse.y == midhouse.y){
            return 0;
        }else if(searchhouse.y < midhouse.y){
            return -1;
        }else return 1;
    }else if(searchhouse.x < midhouse.x){
        return -1;
    }else return 1;
}

int inlist(housepos hpos, houselist houses){
    int s = 0, e = houses.len-1;
    while(s <= e){
        int mid = (s+e)/2;
        int h_cmp = cmp_houses(hpos,houses.list[mid]);
        housepos hp = houses.list[mid];
        if(h_cmp == 0) return 1;
        else if(h_cmp < 0) e = mid-1;
        else s = mid+1;
    }
    return 0;
}

void resize(houselist *list){
    int newcap = list->capacity+500;
    housepos *newlist = calloc(newcap,sizeof(housepos));
    memcpy(newlist, list->list, list->len*sizeof(housepos));
    free(list->list);
    list->capacity = newcap;
    list->list = newlist;
}

void order(houselist *houses){
    int currh = houses->len-1;
    while(currh > 0 && (cmp_houses(houses->list[currh],houses->list[currh-1]) == -1)){
        housepos tmp = houses->list[currh];
        houses->list[currh] = houses->list[currh-1];
        houses->list[currh-1] = tmp;
        currh--;
    }
}

void appendlist(housepos hpos, houselist *houses){
    if(houses->len == houses->capacity) 
        resize(houses);
    houses->list[houses->len++] = hpos;
    order(houses);
}

#define ROBO_SANTA 1

int main(){
    content input = read_input();
    int sz = input.size;
    houselist houses = { calloc(500,sizeof(housepos)) , 500 , 0 };
    content path = { sz-1 , calloc(sz,1) };
    int ohx = 0, ohy = 0;
    int chx = 0, chy = 0;
    appendlist((housepos){ohx,ohy},&houses);
    for(int h = 0; h < sz; h++){
        int hy = h&ROBO_SANTA ? chy : ohy;
        int hx = h&ROBO_SANTA ? chx : ohx;
        switch(input.data[h]){
            case '^': hy++;
                break;
            case 'v': hy--;
                break;
            case '<': hx--;
                break;
            case '>': hx++;
                break;
            default: break;
        }
        housepos hpos = { hx , hy };
        if(!inlist(hpos,houses)){
            appendlist(hpos,&houses);
        }
        if(h&ROBO_SANTA) chx = hx, chy  = hy;
        else ohx = hx, ohy = hy;
    }
    printf("Total houses: %d\n",houses.len);
    free(input.data);
}