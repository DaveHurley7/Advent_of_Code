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

int level_slope(int nums[], int length){
    int slope = 0;
    for(int i = 0; i < length-1; i++){
        if(nums[i+1] - nums[i] > 0) slope++;
        else slope--;
    }
    if(!slope){
        printf("We have 0");
    }
    //printf("%d\n",slope);
    return slope > 0;
}

int check_level_safety(char level[], int *idx){
    int lvl_end = *idx;
    int c = lvl_end;
    int n_ints = 0;
    while(level[lvl_end] != '\r'){
        lvl_end++;
        if (level[lvl_end] == ' ') n_ints++;
    }
    int nums[++n_ints];
    int n_idx = 0;
    while(c < lvl_end){
        int val = 0;
        while(c < lvl_end && level[c] != ' '){
            val = (val*10) + (level[c] - '0');
            c++;
        }
        nums[n_idx++] = val;
        c++;
    }
    *idx = lvl_end+2;
    if(n_ints < 3){
        return 1;
    }
    int bad_level = 0;
    int is_asc = level_slope(nums,n_ints);
    for(int i = 0; i < n_ints-1; i++){
        int diff = is_asc ? nums[i+1] - nums[i] : nums[i] - nums[i+1];
        if(diff < 1 || diff > 3){
            /*for(int n = 0; n < n_ints; n++) printf("%d ",nums[n]);
            printf("\n");*/
            bad_level++;
            if(bad_level > 1) return 0;
        }
    }
    return 1;
}


int main(){
    char* input = read_input();
    int c = 0;
    int safe_levels = 0;
    while(input[c]){
        if(check_level_safety(input,&c)) safe_levels++;
    }
    printf("Safe levels: %d",safe_levels);
    free(input);
}