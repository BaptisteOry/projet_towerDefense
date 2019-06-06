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

const char* getBuildingTypeName(buildingType bt){
   switch (bt){
    	case RADAR: return "Combi radar";
      		break;
    	case FACTORY: return "Combi usine";
      		break;
    	case STOCK: return "Combi stock";
      		break;
      	default:
			break;
   }
   return 0;
}

Building* allocBuilding(buildingType type, float x, float y){
    Building* b = (Building*) malloc(sizeof(Building));
    if(b == NULL) {
    	printf("allocBuilding : erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}

    b->type = type;
	b->x = x;
	b->y = y;
	b->constructible = 0;
	b->range= 100;
	switch(type){
		case RADAR :
			b->cost = 50;
			b->rRange = 255; b->gRange = 236; b->bRange = 180;
			b->sprite = loadTexture("images/building_radar.png");
			break;
		case FACTORY:
			b->cost = 50;
			b->rRange = 135; b->gRange = 108; b->bRange = 80;
			b->sprite = loadTexture("images/building_factory.png");
			break;
		case STOCK:
			b->cost = 50;
			b->rRange = 229; b->gRange = 119; b->bRange = 1;
			b->sprite = loadTexture("images/building_stock.png");
			break;
		default:
			break;
	}
	b->aRange = 75;
	b->r = 255; b->g = 255; b->b = 255; b->a = 255;
	b->size = 25;
	b->shape = SQUARE;
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

Building* freeBuilding(Building* b){
	if(b != NULL){
		if(b->sprite){
      		glDeleteTextures(1, &(b->sprite));
    	}
        free(b);
    }
    return NULL;
}

void freeBuildings(BuildingList* list){
	if(*list != NULL){
	    Building *temp = *list;
	    Building *next;
	    while(temp != NULL){
	        next = temp->next;
	        if(temp->sprite){
	      		glDeleteTextures(1, &(temp->sprite));
	    	}
	        free(temp);
	        temp = next;
	    }
	    *list = NULL;
	}
}

void deleteBuilding(Building* b, BuildingList* list){
	if(b != NULL && *list != NULL){
	 	BuildingList *modif = list;
	    BuildingList toDelete = *modif;
		while(toDelete != NULL && toDelete->x != b->x && toDelete->y != b->y){
			modif = &toDelete->next;
			toDelete = *modif;
		}
		if(toDelete != NULL){
			*modif = toDelete->next;
			if(toDelete->sprite){
	      		glDeleteTextures(1, &(toDelete->sprite));
	    	}
			free(toDelete);
		}
	}
}

void drawBuildings(BuildingList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			glScalef(list->size, list->size, 0);
			drawPicture(list->sprite, list->r, list->g, list->b, list->a);
		glPopMatrix();

        list = list->next;
    }
}

void drawBuilding(Building* b){
	if(b != NULL){
	    glPushMatrix();
			glTranslatef(b->x, b->y, 0);
			glScalef(b->size, b->size, 0);
			drawPicture(b->sprite, b->r, b->g, b->b, b->a);
		glPopMatrix();
	}	
}

void drawRangeBuildings(BuildingList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			glScalef(list->range, list->range, 0);
			drawCircle(list->rRange, list->gRange, list->bRange, list->aRange);
		glPopMatrix();

        list = list->next;
    }
}

void drawInfosBuilding(Building* b, char* infosConstructions){
	if(b != NULL){
		sprintf(infosConstructions, "Type : %s\nCout : %d\nPortee : %d\n", getBuildingTypeName(b->type), b->cost, b->range);
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