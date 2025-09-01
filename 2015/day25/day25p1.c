#include <stdio.h>

int new_code(int old){
    return (old*252533LL)%33554393LL;
}

int main(){
    short latest_row = 1;
    int code = 20151125;
    short row = 0;
    short col = 1;
    short given_row = 0; //Row from puzzle input
    short given_col = 0; //Column from puzzle input
    int exp_code = 0;
    char code_found = 0;
    while(1){
        row = ++latest_row;
        col = 1;
        while(row){
            code = new_code(code);
            if(row == given_row && col == given_col){
                exp_code = code;
                code_found = 1;
                break;
            }
            row--;
            col++;
        }
        if(code_found) break;
    }
    printf("Code %d",exp_code);
}