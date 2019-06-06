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
#include "../include/construction.h"
#include "../include/node.h"

void giveBonusTowers(Building* b, TowerList* listTowers){
	Tower *tempT = *listTowers;
    while(tempT != NULL){
        if(isSquareIntersectsCircle(tempT->x, tempT->y, b->x, b->y, tempT->size, b->range)){
            switch(b->type){
                case RADAR :
                    tempT->rangeBonus += 0.25;
                    break;
                case FACTORY:
                    tempT->powerBonus += 0.25;
                    break;
                case STOCK:
                    tempT->rateBonus += 0.25;
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
                    t->rangeBonus += 0.25;
                    break;
                case FACTORY:
                    t->powerBonus += 0.25;
                    break;
                case STOCK:
                    t->rateBonus += 0.25;
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
                    tempT->rangeBonus -= 0.25;
                    break;
                case FACTORY:
                    tempT->powerBonus -= 0.25;
                    break;
                case STOCK:
                    tempT->rateBonus -= 0.25;
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
                    t->rangeBonus -= 0.25;
                    break;
                case FACTORY:
                    t->powerBonus -= 0.25;
                    break;
                case STOCK:
                    t->rateBonus -= 0.25;
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

void towerConstructible(Tower* towerToBuild, BuildingList buildings, TowerList towers, Node* nodes, Game* game, int x, int y, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT){
    if(!(constructionIntersection(buildings, towers, towerToBuild->x, towerToBuild->y, towerToBuild->size, towerToBuild->shape)) 
       && (towerToBuild->cost <= game->money)
       && !(doesCircleIntersectsPath(x, y, towerToBuild->size, nodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT))){
        towerToBuild->r = 255; towerToBuild->g = 255; towerToBuild->b = 255; towerToBuild->a = 255;
        towerToBuild->constructible = 1;
    }else{
        towerToBuild->r = 255; towerToBuild->g = 0; towerToBuild->b = 0; towerToBuild->a = 150;
        towerToBuild->constructible = 0;
    }
}

void buildingConstructible(Building* buildingToBuild, BuildingList buildings, TowerList towers, Node* nodes, Game* game, int x, int y, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT){
    if(!(constructionIntersection(buildings, towers, buildingToBuild->x, buildingToBuild->y, buildingToBuild->size, buildingToBuild->shape)) 
       && (buildingToBuild->cost <= game->money)
       && !(doesCircleIntersectsPath(x, y, buildingToBuild->size, nodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT))){
        buildingToBuild->r = 255; buildingToBuild->g = 255; buildingToBuild->b = 255; buildingToBuild->a = 255;
        buildingToBuild->constructible = 1;
    }else{
        buildingToBuild->r = 255; buildingToBuild->g = 0; buildingToBuild->b = 0; buildingToBuild->a = 150;
        buildingToBuild->constructible = 0;
    }
}
