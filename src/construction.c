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
#include "../include/building.h"
#include "../include/tower.h"
#include "../include/construction.h"

void giveBonusTowers(Building* b, TowerList* listTowers){
	Tower *tempT = *listTowers;
    while(tempT != NULL){
        if(isSquareIntersectsCircle(tempT->x, tempT->y, b->x, b->y, tempT->size, b->range)){
            switch(b->type){
                case RADAR :
                    tempT->rangeMultiplier += 0.25;
                    break;
                case FACTORY:
                    tempT->powerMultiplier += 0.25;
                    break;
                case STOCK:
                    tempT->rateMultiplier += 0.25;
                    break;
                default:
                    break;
            }
        }
    	tempT = tempT->next;
    }
}

void giveBonusTower(Tower* t, BuildingList* listBuildings){
    Building *tempB = *listBuildings;
    while(tempB != NULL){
        if(isSquareIntersectsCircle(t->x, t->y, tempB->x, tempB->y, t->size, tempB->range)){
            switch(tempB->type){
                case RADAR :
                    t->rangeMultiplier += 0.25;
                    break;
                case FACTORY:
                    t->powerMultiplier += 0.25;
                    break;
                case STOCK:
                    t->rateMultiplier += 0.25;
                    break;
                default:
                    break;
            }
        }
        tempB = tempB->next;
    }
}

void removeBonusTowers(Building* b, TowerList* listTowers){
	Tower *tempT = *listTowers;
    while(tempT != NULL){
        if(isSquareIntersectsCircle(tempT->x, tempT->y, b->x, b->y, tempT->size, b->range)){
            switch(b->type){
                case RADAR :
                    tempT->rangeMultiplier -= 0.25;
                    break;
                case FACTORY:
                    tempT->powerMultiplier -= 0.25;
                    break;
                case STOCK:
                    tempT->rateMultiplier -= 0.25;
                    break;
                default:
                    break;
            }
        }
    	tempT = tempT->next;
    }
}

void removeBonusTower(Tower* t, BuildingList* listBuildings){
    Building *tempB = *listBuildings;
    while(tempB != NULL){
        if(isSquareIntersectsCircle(t->x, t->y, tempB->x, tempB->y, t->size, tempB->range)){
            switch(tempB->type){
                case RADAR :
                    t->rangeMultiplier -= 0.25;
                    break;
                case FACTORY:
                    t->powerMultiplier -= 0.25;
                    break;
                case STOCK:
                    t->rateMultiplier -= 0.25;
                    break;
                default:
                    break;
            }
        }
        tempB = tempB->next;
    }
}

int constructionIntersection(BuildingList listBuildings, TowerList listTowers, float xNew, float yNew, int sizeNew, shape shapeNew){
    if(towerIntersection(listTowers, xNew, yNew, sizeNew, shapeNew)){
        return 1;
    }else if(buildingIntersection(listBuildings, xNew, yNew, sizeNew, shapeNew)){
        return 1;
    }else{
        return 0;
    }
}

