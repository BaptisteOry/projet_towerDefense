#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
#include "../include/monster.h"
#include "../include/game.h"

Game* allocGame(){
    Game* g = (Game*) malloc(sizeof(Game));
    if(g == NULL) {
        fprintf(stderr, "MEMORY ERROR\n");
        exit(1);
    }
    g->money = 500;
    g->nbWave = 0;
    g->nbMonstersWave = 10;
    g->end = 0;

    return g;
}

void freeGame(Game* g){
    if(g != NULL){
        free(g);
    }
}

void addWave(Game* g, MonsterList* list){
    Monster* tempM;
    for (int i = 0; i < g->nbMonstersWave; i++){
        tempM = allocMonster(randomRange(0, MNUMBER-1), -245, -75);
        tempM->lootMultiplier += g->nbWave*0.25;
        tempM->healthPointsMultiplier += g->nbWave*0.25;
        addMonster(tempM, list);
    }
    (g->nbWave) += 1;
}