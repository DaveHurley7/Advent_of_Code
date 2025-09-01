#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _ilst{
    int size;
    int cap;
    int *list;
}int_list;

int get_factor_partial_sum(int n, int visit_limit){
    int total = 0;
    for(int f = 1; f <= visit_limit; f++){
        if(n%f==0){
            total += n/f;
        }
    }
    return total;
}

#define PART1_HOUSE 0
//^This can be usedul if done part 1

int main(){
    int puzzle_input = 0; //Puzzle input
    int house = PART1_HOUSE;
    int total_presents = 0;
    int house_visit_limit = 50;
    while(total_presents < puzzle_input){
        house++;
        int presents = get_factor_partial_sum(house,house_visit_limit) * 11;
        if(total_presents < presents){
            total_presents = presents;
        }
    }
    printf("House no.: %d",house);
}