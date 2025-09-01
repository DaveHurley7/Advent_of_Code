#include <stdio.h>
#include <stdlib.h>

void inc_pwd(char *input, int len){
    int i = len-1;
    while(i >= 0){
        input[i]++;
        if(input[i] > 'z'){
            input[i--] = 'a';
        }else break;
    }
}

int verify_pwd(char *input, int len){
    for(int c = 0; c < len; c++){
        switch(input[len]){
            case 'i': case 'o': case 'l':
                return 0;
        }
    }

    int consec_letters = 0;
    for(int c = 0; c < len-2; c++){
        if(input[c+1] == input[c]+1 && input[c+2] == input[c]+2){
            consec_letters = 1;
            break;
        }
    }
    if(!consec_letters) return 0;

    int double_letters = 0;
    for(int c = 0; c < len-1; c++){
        if(input[c+1] == input[c]){
            c++;
            double_letters++;
        }
    }
    if(double_letters < 2) return 0;
    return 1;
}

#define INPUT_LEN 8

int main(){
    char* input = calloc(INPUT_LEN+1,sizeof(char));
    sprintf(input,"%s","<puzzle input>");
    int p1pass = 0;
    while(1){
        inc_pwd(input,INPUT_LEN);
        if(verify_pwd(input,INPUT_LEN)){
            if(p1pass == 0){
                p1pass = 1;
            }else break;
        }
    }
    printf("Next password: %s",input);
}