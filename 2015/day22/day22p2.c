#include <stdio.h>
#include <string.h>

typedef struct _player{
    char hit_points;
    char armor;
    short mana;
}player;

typedef struct _boss{
    char hit_points;
    char damage;
}boss;

#define SHIELD 0
#define POISON 1
#define RECHARGE 2

typedef struct _gs{
    player player;
    boss boss;
    short turn;
    short mana_spent;
    char effects[3];
}game_state;

typedef struct _sq{
    short maxsize;
    game_state *states;
    short start;
    short end;
}state_queue;

void add_state(state_queue *queue, game_state game){
    queue->start = (queue->start+1)%queue->maxsize;
    queue->states[queue->start] = game;
}

char queue_empty(state_queue queue){
    return queue.start == queue.end;
}

game_state get_next_state(state_queue *queue){
    queue->end = (queue->end+1)%queue->maxsize;
    return queue->states[queue->end];
}

#define MAGIC_MISSILE_COST 53
#define DRAIN_COST 73
#define SHIELD_COST 113
#define POISON_COST 173
#define RECHARGE_COST 229

#define BOSS_TURN 1

short fight(game_state game, short least_mana){
    game_state current;
    memcpy(&current,&game,sizeof(game_state));
    if((current.turn&BOSS_TURN) == 0) 
        current.player.hit_points--;
    if(current.player.hit_points < 1) return least_mana;
    if(current.mana_spent > least_mana) return least_mana;
    if(current.effects[SHIELD]){
        current.player.armor = 7;
        current.effects[SHIELD]--;
    }else current.player.armor = 0;
    if(current.effects[POISON]){
        current.boss.hit_points -= 3;
        current.effects[POISON]--;
    }
    if(current.effects[RECHARGE]){
        current.player.mana += 101;
        current.effects[RECHARGE]--;
    }
    if(current.boss.hit_points < 1){
        return least_mana > current.mana_spent ? current.mana_spent : least_mana;
    }
    if(current.turn&BOSS_TURN){
        char boss_attack = (current.boss.damage < current.player.armor) ?  1 : current.boss.damage - current.player.armor;
        current.player.hit_points -= boss_attack;
        if(current.player.hit_points < 1) return least_mana;
        current.turn++;
        return fight(current,least_mana);
    }else{
        if(current.player.mana >= MAGIC_MISSILE_COST){
            game_state mm;
            memcpy(&mm,&current,sizeof(game_state));
            mm.turn++;
            mm.player.mana -= MAGIC_MISSILE_COST;
            mm.mana_spent += MAGIC_MISSILE_COST;
            mm.boss.hit_points -= 4;
            least_mana = fight(mm,least_mana);
        }
        if(current.player.mana >= DRAIN_COST){
            game_state dn;
            memcpy(&dn,&current,sizeof(game_state));
            dn.turn++;
            dn.player.mana -= DRAIN_COST;
            dn.mana_spent += DRAIN_COST;
            dn.player.hit_points += 2;
            dn.boss.hit_points -= 2;
            least_mana = fight(dn,least_mana);
        }
        if(current.player.mana >= SHIELD_COST && current.effects[SHIELD] == 0){
            game_state sh;
            memcpy(&sh,&current,sizeof(game_state));
            sh.turn++;
            sh.player.mana -= SHIELD_COST;
            sh.mana_spent += SHIELD_COST;
            sh.effects[SHIELD] = 6;
            least_mana = fight(sh,least_mana);
        }
        if(current.player.mana >= POISON_COST && current.effects[POISON] == 0){
            game_state pn;
            memcpy(&pn,&current,sizeof(game_state));
            pn.turn++;
            pn.player.mana -= POISON_COST;
            pn.mana_spent += POISON_COST;
            pn.effects[POISON] = 6;
            least_mana = fight(pn,least_mana);
        }
        if(current.player.mana >= RECHARGE_COST && current.effects[RECHARGE] == 0){
            game_state rc;
            memcpy(&rc,&current,sizeof(game_state));
            rc.turn++;
            rc.player.mana -= RECHARGE_COST;
            rc.mana_spent += RECHARGE_COST;
            rc.effects[RECHARGE] = 5;
            least_mana = fight(rc,least_mana);
        }
    }
    return least_mana;
}

int main(){
    game_state init = {
        /*Player stats*/ { 50 , 0 , 500 } , 
        /*Boss stats*/ { 0 } , //Puzzle input
        0 , 0 , {0,0,0} 
    };
    short least_mana = 0x7fff;
    least_mana = fight(init,least_mana);
    
    printf("Lowest mana spent: %d\n",least_mana);
}