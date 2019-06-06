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
#include "../include/building.h"
#include "../include/tower.h"
#include "../include/game.h"
#include "../include/node.h"

Game* allocGame(){
    Game* g = (Game*) malloc(sizeof(Game));
    if(g == NULL) {
        fprintf(stderr, "MEMORY ERROR\n");
        exit(1);
    }
    g->money = 500;

    g->nbWave = 0;
    g->lastWave = 2;
    g->nbMonstersPerWave = 10;
    g->timeWave = 10000; // 10s entre chaque vague
    g->timeAddWave = 500; // 500ms entre les apparitions de monstre

    g->status = 0;

    return g;
}

void freeGame(Game* g){
    if(g != NULL){
        free(g);
    }
}

void addWave(Game* g, MonsterList* list, int counter, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT){
    if(counter >= 0){
        if(counter%(g->timeWave) == 0 && (g->nbWave) < (g->lastWave)){
            (g->nbWave) += 1;
        }
        if((counter%(g->timeWave) < (g->nbMonstersPerWave)*(g->timeAddWave)) && ((counter%(g->timeWave))%(g->timeAddWave) == 0) && counter < (g->timeWave)*(g->lastWave)){
            printf("%d\n", counter);
            Monster* tempM;
            tempM = allocMonster(randomRange(0, MNUMBER-1), -245, -75);
            initializeMonsterPath(tempM, nodes);
            initializeMonsterPosition(tempM, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
            tempM->loot *= 1+(g->nbWave-1)*0.25;
            tempM->healthPoints *= 1+(g->nbWave-1)*0.25;
            tempM->healthPointsRatio = tempM->healthPoints;
            addMonster(tempM, list);
        }
    }
}

void killMonsters(MonsterList* listMonsters, TowerList* listTowers, int counter){
    if(*listMonsters != NULL && *listTowers != NULL){
        Monster *tempM = *listMonsters;
        Monster *toDelete;
        while(tempM != NULL){ // Test pour chaque monstre
            Tower *tempT = *listTowers;
            while(tempT != NULL){ // Test pour chaque tour
                if(isCircleIntersectsCircle(tempT->x, tempT->y, tempM->x, tempM->y, (tempT->range*tempT->rangeBonus), tempM->size)){ // Test portée de la tour
                    if(counter%(int)(tempT->rate)/(tempT->rateBonus) == 0){ // Test cadence de la tour
                        tempM->healthPoints -= (tempT->power*tempT->powerBonus)/20; // Test puissance de la tour
                        if((tempM->healthPoints) <= 0){
                            toDelete = tempM;
                        }
                    }
                }
                tempT = tempT->next;
            }
            tempM = tempM->next;
            // Kill monster
            if(toDelete != NULL){
                deleteMonster(toDelete, listMonsters);
                toDelete = NULL;
            }
        }
    }
}

void endGame(Game* g, MonsterList* list){
    if(g->nbWave == g->lastWave && *list == NULL){
        printf("%s\n", "This is the end !");
    }
}