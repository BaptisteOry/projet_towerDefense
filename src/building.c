#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
#include "../include/building.h"

Building* allocBuilding(buildingType type, float x, float y){
    Building* b = (Building*) malloc(sizeof(Building));
    if(b == NULL) {
		fprintf(stderr, "MEMORY ERROR\n");
		exit(1);
	}

    b->type = type; // Type
	b->x = x; // Position x
	b->y = y; // Position y
	b->range= 100;
	switch(type){
		case RADAR :
			b->cost = 100;
			b->r = 0; b->g = 0; b->b = 0;
			b->sprite = loadTexture("images/building_1.png");
			break;
		case FACTORY:
			b->cost = 50;
			b->r = 128; b->g = 128; b->b = 128;
			b->sprite = loadTexture("images/building_1.png");
			break;
		case STOCK:
			b->cost = 25;
			b->r = 255; b->g = 255; b->b = 255;
			b->sprite = loadTexture("images/building_1.png");
			break;
		default:
			break;
	}
	b->size = 25;
	b->next = NULL;

    return b;
}

void addBuilding(Building* b, BuildingList* list){
    if(*list == NULL){
        *list = b;
    }else{
        Building *temp = *list;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = b;
    }
}

void freeBuilding(Building* b){
	if(b != NULL){
        free(b);
    }
}

int deleteBuilding(Building* b, BuildingList* list){
	if(b == NULL || *list == NULL){
		return 0;
	}
 	BuildingList *modif = list;
    BuildingList toDelete = *modif;
	while(toDelete != NULL && toDelete->x != b->x && toDelete->y != b->y){
		modif = &toDelete->next;
		toDelete = *modif;
	}
	if(toDelete == NULL){
		return 0;
	}else{
		*modif = toDelete->next;
		free(toDelete);

		return 1;
	}
}

void deleteBuildings(BuildingList* list){
    Building *temp = *list;
    Building *next;
    while(temp != NULL){
        next = temp->next;
        free(temp);
        temp = next;
    }
    *list = NULL;
}

void drawBuildings(BuildingList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			drawSquare(list->r, list->g, list->b, 255, list->size);
			drawPicture(list->sprite, list->size, list->size); // Taille tour
		glPopMatrix();

        list = list->next;
    }
}

void drawRangeBuildings(BuildingList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			drawCircle(list->r, list->g, list->b, 40, list->range);
		glPopMatrix();

        list = list->next;
    }
}

Building* buildingSelected(BuildingList list, float x, float y){
	while(list != NULL){
		if(isSquareIntersectsSquare(list->x, list->y, x, y, list->size, 0)){
		    return list;
		}
		list = list->next;
	}
    return 0;
}

Building* buildingIntersection(BuildingList list, float xNew, float yNew, int sizeNew, shape shapeNew){
	if(shapeNew == CIRCLE){
		while(list != NULL){
			if(isCircleIntersectsCircle(list->x, list->y, xNew, yNew, list->size, sizeNew)){
			    return list;
			}
			list = list->next;
		}
	}else if(shapeNew == SQUARE){
		while(list != NULL){
			if(isSquareIntersectsCircle(list->x, list->y, xNew, yNew, list->size, sizeNew)){
			    return list;
			}
			list = list->next;
		}
	}
    return 0;
}