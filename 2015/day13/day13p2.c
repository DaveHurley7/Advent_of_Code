#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    long size;
    char *data;
} content;

content read_input(){
    FILE *input;
    fopen_s(&input,"input.txt","r");
    if(!input){ printf("input.txt isn't in the same folder as this file"); exit(1);}
    fseek(input,0,SEEK_END);
    int end_pos = ftell(input);
    rewind(input);
    content ctnt = { end_pos, calloc(end_pos+1,1) };
    fread(ctnt.data,1,end_pos,input);
    fclose(input);
    return ctnt;
}

char array_getidx(char **arr, char *item, int size){
    for(int i = 0; i < size; i++){
        if(strcmp(item,arr[i]) == 0){
            return i;
        }
    }
    return -1;
}

char is_delimiter(char curr_char, char delimiters[]){
    int d = 0;
    while(delimiters[d]){
        if(delimiters[d++] == curr_char) return 1;
    }
    return 0;
}

char *extract_name(char *data, int *idx, char delimits[]){
    int c = *idx;
    while(data[c] < 'A' || data[c] > 'Z') c++;
    int name_start = c;
    while(!is_delimiter(data[c],delimits)) c++;
    int name_end = c;
    int str_len = name_end - name_start;
    char *name = calloc(str_len+1,sizeof(char));
    memcpy(name,data+name_start,str_len);
    *idx = c;
    return name;
}

char **get_names(char *data, int *_count){
    char **names = calloc(1,sizeof(char*));
    int count = 0;
    int c = 0;
    while(data[c]){
        char *name = extract_name(data,&c," .");
        if(array_getidx(names,name,count) < 0){
            if(count){
                names = realloc(names,sizeof(char*)*(count+1));
                names[count] = name;
            }else{
                names[0] = name;
            }
            count++;
        }
        if(data[c] == '.') c+=2;
    }
    *_count = count;
    return names;
}

char contains_less(char *data, int *c){
    int s = *c+1;
    while(data[s] < '0' || data[s] > '9'){
        if(data[s] == 'l' && data[s+1] == 'o' && data[s+2] == 's' && data[s+3] == 'e') return 1;
        else s++;
    }
    *c = s;
    return 0;
}

char get_level(char *data, int *c){
    int i = *c;
    int val = 0;
    while(data[i] >= '0' && data[i] <= '9'){
        val = (val*10) + (data[i]-'0');
        i++;
    }
    *c = i;
    return val;
}

char **make_happiness_matrix(char *data, char **names, int num){
    char **matrix = calloc(num,sizeof(char*));
    for(int n = 0; n < num; n++){
        matrix[n] = calloc(num,sizeof(char));
    }
    int c = 0;
    while(data[c]){
        char *p1name = extract_name(data,&c," ");
        int person1 = array_getidx(names,p1name,num);
        char is_neg = contains_less(data,&c);
        while(data[c] < '0' || data[c] > '9') c++;
        char happy_lvl = get_level(data,&c);
        if(is_neg) happy_lvl = -happy_lvl;
        char *p2name = extract_name(data,&c,".");
        int person2 = array_getidx(names,p2name,num);
        matrix[person1][person2] = happy_lvl;
        c+=2;
    }
    return matrix;
}

int in_path(char *path, int c_idx, char amt){
    for(int i = 0; i < amt; i++){
        if(path[i] == c_idx) return 1;
    }
    return 0;
}

int get_optimal_happiness(char **h_matrix, int n_names, char *path, char depth, int curr_happy, int opt_happy){
    if(depth == n_names){
        int last = path[depth-1];
        int final_happy = curr_happy + h_matrix[0][last] + h_matrix[last][0];
        return final_happy > opt_happy ? final_happy : opt_happy;
    }
    for(int c = 1; c < n_names; c++){
        if(in_path(path,c,depth)) continue;
        path[depth] = c;
        int prev_person = path[depth-1];
        int happy_mix = h_matrix[c][prev_person] + h_matrix[prev_person][c];
        opt_happy = get_optimal_happiness(h_matrix, n_names, path, depth+1, curr_happy + happy_mix,opt_happy); 
        path[depth] = 0x80;
    }
    return opt_happy;
}

int main(){
    content input = read_input();
    int n_names = 0;
    char **names = get_names(input.data,&n_names);
    
    n_names++;
    char *self = calloc(5,sizeof(char));
    strncpy(self,"self",4);
    names = realloc(names,sizeof(char*)*n_names);
    names[n_names-1] = self;

    char **happy_matrix = make_happiness_matrix(input.data,names,n_names);
    free(input.data);
    for(int i = 0; i < n_names; i++) free(names[i]);
    free(names);

    char *path = calloc(n_names,sizeof(char));
    path[0] = 0;
    for(int i = 1; i < n_names; i++) path[i] = 0x80;

    int opt_happiness = get_optimal_happiness(happy_matrix,n_names,path,1,0,0);
    printf("Optimal happiness: %d\n",opt_happiness);

    for(int i = 0; i < n_names; i++) free(happy_matrix[i]);
    free(happy_matrix);
}