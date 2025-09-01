#include <stdio.h>
#include <stdlib.h>

char *look_and_say(char *seq, int *buf_sz){
    char count = 0;
    int c = 0, ofst = 0;
    char *new_seq = calloc(*buf_sz*2,sizeof(char));
    char curr_digit = seq[0];
    char prev_digit = curr_digit;
    while((curr_digit = seq[c])){
        if(curr_digit == prev_digit){
            count++;
        }else{
            new_seq[ofst++] = count+'0';
            new_seq[ofst++] = prev_digit;
            count = 1;
        }
        prev_digit = curr_digit;
        c++;
    }
    new_seq[ofst++] = count+'0';
    new_seq[ofst++] = prev_digit;
    *buf_sz = ofst;
    return new_seq;
}

int getlen(char *str){
    int len = 0;
    while(str[len]) len++;
    return len;
}

int main(){
    char input[] = "<puzzle input>";
    int seq_sz = sizeof(input)/sizeof(char);
    char *next_seq = look_and_say(input,&seq_sz);
    for(char i = 1; i < 40; i++){
        next_seq = look_and_say(next_seq,&seq_sz);
    }
    printf("Length: %d",getlen(next_seq));
    free(next_seq);
}