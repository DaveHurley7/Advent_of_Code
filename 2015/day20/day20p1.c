#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _ilst{
    int size;
    int cap;
    int *list;
}int_list;

char is_prime(int n, int_list *primes){
    int size = primes->size;
    if(size == 0){
        primes->list[primes->size++] = 2;
        return 1;
    }
    if(!(n&1)) return 0;
    for(int p = 1; p < size; p++){
        if(n%primes->list[p] == 0) return 0;
    }
    if(size >= primes->cap){
        primes->cap += 50;
        primes->list = realloc(primes->list,primes->cap*sizeof(int));
        memset(primes->list+primes->size,0,50);
    }
    primes->list[primes->size++] = n;
    return 1;
}

int sum_of_powers(int prime, int count){
    if(count == 1) return prime+1;
    int res = 1;
    int val = prime;
    for(int i = 0; i < count; i++){
        res += val;
        if(i < count-1){
            val *= prime;
        }
    }
    return res;
}

int get_factor_sum(int n, int_list primes){
    int p = 0;
    int pr_factor = primes.list[p];
    int pr_count = 0;
    int total = 1;
    while(n > 1){
        if((pr_factor == 2 && !(n&1))){
            n >>= 1;
            pr_count++;
        }else if((n%pr_factor) == 0){
            n /= pr_factor;
            pr_count++;
        }else{
            total *= sum_of_powers(pr_factor,pr_count);
            pr_count = 0;
            pr_factor = primes.list[++p];
        }
    }
    total *= sum_of_powers(pr_factor,pr_count);
    return total;
}

int main(){
    int puzzle_input = 0; //Puzzle input
    int_list prime_list = { 0 , 50 , calloc(50,sizeof(int))};
    int house = 1;
    int total_presents = 0;
    while(total_presents < puzzle_input){
        house++;
        int presents = (is_prime(house,&prime_list) ? (house+1) : get_factor_sum(house,prime_list)) * 10;
        if(total_presents < presents) total_presents = presents;
    }
    printf("House no.: %d",house);
}