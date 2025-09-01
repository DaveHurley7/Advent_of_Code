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

int count_char(char *seq, char ch){
    int c = 0;
    int count = 0;
    while(seq[c]){
        if(seq[c] == ch) count++;
        c++;
    }
    return count;
}

typedef struct _rrstat{
    unsigned char speed;
    unsigned char flytime;
    unsigned char resttime;
} rdeer_travel;

#define NONDIGIT(ch) ch < '0' || ch > '9'

unsigned char parse_num(char *data, int *c){
    unsigned char val = 0;
    int i = *c;
    while(data[i] >= '0' && data[i] <= '9'){
        val = (val*10) + (data[i]-'0');
        i++;
    }
    *c = i;
    return val;
}

rdeer_travel *parse_info(char *data, int amt){
    int c = 0, r_id = 0;
    rdeer_travel *stats = calloc(amt,sizeof(rdeer_travel));
    while(data[c]){
        while(NONDIGIT(data[c])) c++;
        unsigned char speed = parse_num(data,&c);
        while(NONDIGIT(data[c])) c++;
        unsigned char flytime = parse_num(data,&c);
        while(NONDIGIT(data[c])) c++;
        unsigned char resttime = parse_num(data,&c);
        while(data[c] != '\000' && data[c] != '\n') c++;
        stats[r_id++] = (rdeer_travel){ speed , flytime , resttime };
    }
    return stats;
}

#define RACETIME 2503

int main(){
    content input = read_input();
    int n_rdeers = count_char(input.data,'\n');
    rdeer_travel *race_stats = parse_info(input.data,n_rdeers);
    int max_dist = 0;
    for(int r = 0; r < n_rdeers; r++){
        int full_rest_fly_time = race_stats[r].flytime + race_stats[r].resttime;
        int full_rest_fly_count = RACETIME/full_rest_fly_time;
        int rest_fly_rem = RACETIME%full_rest_fly_time;
        int rem_time;
        if(rest_fly_rem >= race_stats[r].flytime) rem_time = race_stats[r].flytime;
        else rem_time = rest_fly_rem;
        int full_rest_fly_dist = race_stats[r].speed * race_stats[r].flytime * full_rest_fly_count;
        int rem_dist = race_stats[r].speed * rem_time;
        int total_dist = full_rest_fly_dist + rem_dist;
        if(total_dist > max_dist) max_dist = total_dist;
    }
    printf("Farthest distance travelled %d\n",max_dist);
    free(race_stats);
    free(input.data);
}