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

int isIntersection(TowerList towers, BuildingList buildings, float xNew, float yNew, int sizeNew){
	while(towers != NULL){
		if(isShapeIntersectsShape(towers->x, towers->y, xNew, yNew, towers->size, sizeNew)){
		    return 1;
		}
		towers = towers->next;
	}
	while(buildings != NULL){
		if(isShapeIntersectsShape(buildings->x, buildings->y, xNew, yNew, buildings->size, sizeNew)){
		    return 1;
		}
		buildings = buildings->next;
	}
    return 0;
}