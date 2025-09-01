#include <stdio.h>
#include <stdlib.h>

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

int count_chars(char data[], char c){
    int i = 0;
    int count = 0;
    while(data[i]){
        if(data[i] == c) count++;
        i++;
    }
    return count;
}

char parse_num(char data[], int *i){
    int idx = *i;
    char val = 0;
    while(data[idx] >= '0' && data[idx] <= '9')
        val = (val*10) + (data[idx++]-'0');
    *i = idx;
    return val;
}

void fill_array(char data[], char *num_arr, int size){
    int i = 0;
    int idx = size-1;
    while(data[i]){
        char num = parse_num(data,&i);
        num_arr[idx--] = num;
        i++;
    }
}

short calc_group_weight(char nums[], int amt){
    short total = 0;
    for(int n = 0; n < amt; n++) total += nums[n];
    return total/4;
}

typedef struct _stat{
    char weight_count;
    long long qe;
}cfg_stat;

cfg_stat find_best_qe(char nums[], int len, int i, short grp_wght, char n_weights, short subtotal, long long subqe, cfg_stat best_stat){
    if(n_weights > best_stat.weight_count) return best_stat;
    if(subtotal < grp_wght){
        for(int n = i; n < len; n++){
            char pick = nums[n];
            best_stat = find_best_qe(nums,len,n+1,grp_wght,n_weights+1,subtotal+nums[n],subqe*nums[n],best_stat);
        }
        return best_stat;
    }else if(subtotal > grp_wght){
        return best_stat;
    }else{
        if(n_weights < best_stat.weight_count ||
            n_weights == best_stat.weight_count && subqe < best_stat.qe){
            return (cfg_stat){ n_weights , subqe };
        }else{
            return best_stat;
        }
    }
}

#define MAX_CFG_STAT (cfg_stat){ 10 , 0x7fffffffffffffff }

int main(){
    content input = read_input();
    int n_nums = count_chars(input.data,'\n');
    char *nums = calloc(n_nums,sizeof(char));
    fill_array(input.data,nums,n_nums);
    short group_weight = calc_group_weight(nums,n_nums);
    cfg_stat best = find_best_qe(nums,n_nums,0,group_weight,0,0,1,MAX_CFG_STAT);
    printf("Ideal QE: %lld",best.qe);
    free(input.data);
}