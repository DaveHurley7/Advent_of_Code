#include <stdio.h>
#include <stdlib.h>

#define LINES 1000

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

void read_line(char* input, int *idx, int list1[], int list2[], int listidx){
    int c = *idx;
    int op1 = 0;
    while(input[c] != ' '){
        op1 = (op1*10) + (input[c] - '0');
        c++;
    }
    while(input[c] == ' ') c++;
    int op2 = 0;
    while(input[c] != '\r'){
        op2 = (op2*10) + (input[c] - '0');
        c++;
    }
    c+=2;
    *idx = c;
    list1[listidx] = op1;
    list2[listidx] = op2;
}

void sort(int list[]){
    for(int i = 1; i < LINES; i++){
        int j = i;
        while(j > 0 && list[j] < list[j-1]){
            int tmp = list[j];
            list[j] = list[j-1];
            list[j-1] = tmp;
            j--;
        }
    }
}

int main(){
    char* input = read_input();
    int idx = 0;
    int numlist1[LINES], numlist2[LINES];
    int numidx = 0;
    while(input[idx]){
        read_line(input,&idx,numlist1,numlist2,numidx);
        numidx++;
    }
    sort(numlist1);
    sort(numlist2);
    int total_diff = 0;
    for(int i = 0; i < LINES; i++){
        int val1 = numlist1[i];
        int val2 = numlist2[i]
        total_diff += (val1 > val2) ? val1 - val2 : val2 - val1;
    }
    printf("Total diff: %d\n",total_diff);
    free(input);
}