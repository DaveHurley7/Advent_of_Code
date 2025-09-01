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

int count_nums(char *data){
    int n = 0, c = 0;
    while(data[c]){
        if(data[c] == '\n') n++;
        c++;
    }
    return n;
}

short *get_litres(char *data, int n){
    short *litres = calloc(n,sizeof(short));
    int c = 0;
    int ltr_idx = 0;
    while(data[c]){
        short val = 0;
        while(data[c] >= '0' && data[c] <= '9'){
            val = val*10 + (data[c]-'0');
            c++;
        }
        litres[ltr_idx++] = val;
        if(data[c] == '\n') c++;
    }
    return litres;
}

void sort(short list[], int len){
    for(int i = 1; i < len; i++){
        while(i > 0 && list[i] < list[i-1]){
            short tmp = list[i];
            list[i] = list[i-1];
            list[i-1] = tmp;
            i--;
        }
    }
}

int find_sum_combos(short *arr, int n, int idx, int total){
    if(idx == n) return total == 150;
    if(total == 150) return 1;
    short num = arr[idx];
    int combos = 0;
    if(total <= 150) combos += find_sum_combos(arr,n,idx+1,total+num);
    combos += find_sum_combos(arr,n,idx+1,total);
    return combos;
}

int main(){
    content input = read_input();
    int n_nums = count_nums(input.data);
    short *litres = get_litres(input.data,n_nums);
    free(input.data);
    sort(litres,n_nums);
    int combos = find_sum_combos(litres,n_nums,0,0);
    printf("Number of combinations %d\n",combos);
    free(litres);
}