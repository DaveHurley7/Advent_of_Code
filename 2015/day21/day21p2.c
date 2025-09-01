#include <stdio.h>

typedef struct _itst{
    short cost;
    char damage;
    char armor;
}item_stat;

typedef struct _ftr{
    char hit_points;
    char damage;
    char armor;
}fighter;

typedef struct sdb{
    char damage;
    char armor;
    short cost;
}prev_stats;

char begin_fight(fighter player, fighter boss){
    char player_attack = player.damage > boss.armor ? player.damage - boss.armor : 1;
    char boss_attack = boss.damage > player.armor ? boss.damage - player.armor : 1;
    while(1){
        boss.hit_points -= player_attack;
        if(boss.hit_points <= 0) 
            return 1;
        player.hit_points -= boss_attack;
        if(player.hit_points <= 0) 
            return 0;
    }
}

int main(){
    fighter player = { 100 , 0 , 0 };
    fighter boss = { 0 };  //Puzzle input
    item_stat weapons[] = {
        { 8 , 4 , 0 },
        { 10 , 5 , 0 },
        { 25 , 6 , 0 },
        { 40 , 7 , 0 },
        { 74 , 8 , 0 },
    };
    item_stat armortypes[] = {
        { 13 , 0 , 1 },
        { 31 , 0 , 2 },
        { 53 , 0 , 3 },
        { 75 , 0 , 4 },
        { 102 , 0 , 5 },
    };
    item_stat rings[] = {
        { 25 , 1 , 0 },
        { 50 , 2 , 0 },
        { 100 , 3 , 0 },
        { 20 , 0 , 1 },
        { 40 , 0 , 2 },
        { 80 , 0 , 3 },
    };
    short best_cost = 0;
    for(char w = 0; w < 5; w++){
        for(char a = -1; a < 5; a++){
            for(char r1 = -1; r1 < 6; r1++){
                for(char r2 = r1; r2 < 6; r2++){
                    if(r1 == -1) r2 = 7;  
                    short current_cost = weapons[w].cost;
                    player.damage = weapons[w].damage;
                    player.armor = weapons[w].armor;
                    if(a >= 0){
                        current_cost += armortypes[a].cost;
                        player.damage += armortypes[a].damage;
                        player.armor += armortypes[a].armor;
                    }
                    if(r1 >= 0){
                        current_cost += rings[r1].cost;
                        player.damage += rings[r1].damage;
                        player.armor += rings[r1].armor;
                        if(r2 < 7 && r2 != r1){
                            current_cost += rings[r2].cost;
                            player.damage += rings[r2].damage;
                            player.armor += rings[r2].armor;
                        }
                    }
                    if(!begin_fight(player,boss) && best_cost < current_cost){ 
                        best_cost = current_cost;
                    }
                }
            }
        }        
    }
    printf("Expensive loss: %d",best_cost);
}