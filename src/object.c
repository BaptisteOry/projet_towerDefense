#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
#include "../include/building.h"
#include "../include/tower.h"
#include "../include/object.h"

void giveBonusTowers(Building* b, TowerList* listTowers){
	Tower *tempT = *listTowers;
    while(tempT != NULL){
        if(isCircleIntersectsCircle(tempT->x, tempT->y, b->x, b->y, tempT->size, b->range)){
            switch(b->type){
                case RADAR :
                    tempT->range = ((tempT->range)*125)/100;
                    break;
                case FACTORY:
                    tempT->power = ((tempT->power)*125)/100;
                    break;
                case STOCK:
                    tempT->rate = ((tempT->rate)*125)/100;
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
        if(isCircleIntersectsCircle(tempT->x, tempT->y, b->x, b->y, tempT->size, b->range)){
        	printf("%s\n", "lol");
            switch(b->type){
                case RADAR :
                    tempT->range = ((tempT->range)*100)/125;
                    break;
                case FACTORY:
                    tempT->power = ((tempT->power)*100)/125;
                    break;
                case STOCK:
                    tempT->rate = ((tempT->rate)*100)/125;
                    break;
                default:
                    break;
            }
        }
        printf("%f\n",tempT->range );
    	tempT = tempT->next;
    }
}

