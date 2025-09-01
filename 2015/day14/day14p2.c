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
    unsigned char bursttime;
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
        stats[r_id++] = (rdeer_travel){ speed , flytime , resttime , flytime + resttime};
    }
    return stats;
}

#define RACETIME 2503

void update_dists(int *dist, rdeer_travel rdeer, int elapsed_time){
    int fly_rest = elapsed_time % rdeer.bursttime;
    if(fly_rest < rdeer.flytime){
        *dist += rdeer.speed;
    }
}

int get_farthest(int *dists, int num){
    int max = dists[0];
    for(int i = 1; i < num; i++){
        if(max < dists[i]) max = dists[i];
    }
    return max;
}

int main(){
    content input = read_input();
    int n_rdeers = count_char(input.data,'\n');
    rdeer_travel *race_stats = parse_info(input.data,n_rdeers);

    int *dists = calloc(n_rdeers,sizeof(int));
    int *scores = calloc(n_rdeers,sizeof(int));
    for(int s = 0; s < RACETIME; s++){
        for(int r = 0; r < n_rdeers; r++){
            update_dists(&dists[r],race_stats[r],s);
        }
        int farthest = get_farthest(dists,n_rdeers);
        for(int r = 0; r < n_rdeers; r++){
            if(dists[r] == farthest) scores[r]++;
        }
    }
    int highest_score = get_farthest(scores,n_rdeers);
    printf("Highest score: %d\n",highest_score);
    free(race_stats);
    free(input.data);
}