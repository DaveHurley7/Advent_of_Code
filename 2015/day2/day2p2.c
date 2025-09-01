#include <stdio.h>
#include <stdlib.h>

typedef struct{
    long size;
    char *data;
} content;

content get_input(){
    FILE *f = fopen("input.txt","rb");
    if(!f){
        printf("Couldn't open input file\n");
        exit(1);
    }
    fseek(f,0L,SEEK_END);
    long int f_len = ftell(f);
    rewind(f);
    content input = { f_len , malloc((sizeof(char)*f_len) + 1) };
    fread(input.data,sizeof(char),f_len,f);
    input.data[f_len] = 0;
    fclose(f);
    return input;
}

int count_presents(content data){
    int num_objs = 0;
    for(int c = 0; c < data.size; c++)
        if(data.data[c] == '\n')
            num_objs++;
    printf("Number of presents %d\n",num_objs);
    return num_objs;
}

typedef struct{
    int l, w, h;
} obj_dims;

int parse_int(content data, char limit_char, int* char_pos){
    int c = *char_pos;
    int val = 0;
    while(data.data[c] != limit_char){
        char cchr = data.data[c];
        val = (val*10) + (data.data[c++]-'0');
    }
    *char_pos = c+1;
    return val;
}

void parse_obj_surfs(content data, obj_dims objsf[]){
    int c = 0;
    int obj_idx = 0;
    while(data.data[c]){
        int l = parse_int(data,'x',&c);
        int w = parse_int(data,'x',&c);
        int h = parse_int(data,'\r',&c);
        c++;
        objsf[obj_idx++] = (obj_dims){ l, w, h };
    }
}

void oneiter_bblsort(int dims[], int len){
    for(int i = 0; i < len-1; i++){
        if(dims[i] > dims[i+1]){
            int tmp = dims[i];
            dims[i] = dims[i+1];
            dims[i+1] = tmp;
        }
    }
}
                                     
int calculate_ribbon_length(obj_dims gift){
    int dims[] = {gift.l,gift.w,gift.h};
    oneiter_bblsort(dims,3);
    int perimeter = (dims[0] + dims[1]) * 2;
    int volume = dims[0] * dims[1] * dims[2];
    return perimeter + volume;
}                                

int main(){
    content input = get_input();
    int num_osfs = count_presents(input);
    obj_dims *gifts = (obj_dims*) calloc(num_osfs,sizeof(obj_dims));
    parse_obj_surfs(input,gifts);
    int total_size = 0;
    for(int gwp = 0; gwp < num_osfs; gwp++)
        total_size += calculate_ribbon_length(gifts[gwp]);
    printf("Total ribbon length %d\n",total_size);
    free(input.data);
}