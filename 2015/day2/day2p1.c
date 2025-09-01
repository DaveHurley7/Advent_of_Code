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
    int s1, s2, s3;
} obj_surf;

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

void parse_obj_surfs(content data, obj_surf objsf[]){
    int c = 0;
    int obj_idx = 0;
    while(data.data[c]){
        int l = parse_int(data,'x',&c);
        int w = parse_int(data,'x',&c);
        int h = parse_int(data,'\r',&c);
        c++;
        printf("L: %d, W: %d, H: %d\n",l,w,h);
        objsf[obj_idx++] = (obj_surf){ l*w , w*h, l*h };
    }
}
                                     
int calculate_wrapping_paper_size(obj_surf gift){
    int total_surface_area = (2*gift.s1) + (2*gift.s2) + (2*gift.s3);
    int smallest_surface = gift.s1;
    if (gift.s2 < smallest_surface) smallest_surface = gift.s2;
    if (gift.s3 < smallest_surface) smallest_surface = gift.s3;
    return total_surface_area + smallest_surface;
}                                

int main(){
    content input = get_input();
    int num_osfs = count_presents(input);
    obj_surf *gifts = (obj_surf*) calloc(num_osfs,sizeof(obj_surf));
    parse_obj_surfs(input,gifts);
    int total_size = 0;
    for(int gwp = 0; gwp < num_osfs; gwp++)
        total_size += calculate_wrapping_paper_size(gifts[gwp]);
    printf("Total wrapping paper %d\n",total_size);
    free(input.data);
}