#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
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

int constructionIntersection(BuildingList listBuildings, TowerList listTowers, float xNew, float yNew, int sizeNew, shape shapeNew){
    if(towerIntersection(listTowers, xNew, yNew, sizeNew, shapeNew)){
        return 1;
    }else if(buildingIntersection(listBuildings, xNew, yNew, sizeNew, shapeNew)){
        return 1;
    }else{
        return 0;
    }
}

void drawInfosConstructions(char* infosConstructions){
    if(strcmp(infosConstructions, "") != 0){
        glPushMatrix();
            glTranslatef(250 - 55, 150 - 55, 0);
            glPushMatrix();
                glScalef(50, 50, 0);
                drawSquare(0, 0, 0, 205);
            glPopMatrix();
            displayText(GLUT_BITMAP_HELVETICA_18, (unsigned char*)infosConstructions, -48, 40, 255, 255, 255);
        glPopMatrix();
    }
}

