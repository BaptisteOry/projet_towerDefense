#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/display.h"
#include "../include/operations.h"
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
    g->nbMonstersPerWave = 10;
    g->timeWave = 10000; // 10s entre chaque vague
    g->timeAddWave = 500; // 500ms entre les apparitions de monstre

    g->end = 0;

    return g;
}

void freeGame(Game* g){
    if(g != NULL){
        free(g);
    }
}

void addWave(Game* g, MonsterList* list, int counter){
    if((counter%(g->timeWave) < (g->nbMonstersPerWave)*(g->timeAddWave)) && ((counter%(g->timeWave))%(g->timeAddWave) == 0) && (counter >= (g->timeWave))){
        if(counter%(g->timeWave) == 0){
            (g->nbWave) += 1;
        }
        Monster* tempM;
        tempM = allocMonster(randomRange(0, MNUMBER-1), -245, -75);
        tempM->lootMultiplier += (g->nbWave-1)*0.25;
        tempM->healthPointsMultiplier += (g->nbWave-1)*0.25;
        addMonster(tempM, list);
    }
}