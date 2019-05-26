#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
#include "../include/tower.h"

Tower* allocTower(towerType type, float x, float y){
    Tower* t = (Tower*) malloc(sizeof(Tower));
    if(t == NULL) {
		printf("allocTower : erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}

    t->type = type; // Type
	t->x = x; // Position x
	t->y = y; // Position y
	t->powerMultiplier = 1;
	t->rangeMultiplier = 1;
	t->rateMultiplier = 1;
	switch(type){
		case TRED :
			t->power = 100;
			t->range= 135;
			t->rate = 10; //every second
			t->cost = 50;
			t->r = 255; t->g = 0; t->b = 0;
			t->sprite = loadTexture("images/tower_1.png");
			break;
		case TGREEN:
			t->power = 50;
			t->range= 110;
			t->rate = 85; //every 200ms
			t->cost = 40;
			t->r = 0; t->g = 255; t->b = 0;
			t->sprite = loadTexture("images/tower_2.png");
			break;
		case TYELLOW:
			t->power = 30;
			t->range= 95;
			t->rate = 30; //every 600ms
			t->cost = 30;
			t->r = 255; t->g = 255; t->b = 0;
			t->sprite = loadTexture("images/tower_3.png");
			break;
		case TBLUE:
			t->power = 30;
			t->range= 80;
			t->rate = 30; //every 400ms
			t->cost = 20;
			t->r = 0; t->g = 0; t->b = 255;
			t->sprite = loadTexture("images/tower_4.png");
			break;
		default:
			break;
	}
	t->size = 25;
	t->shape = CIRCLE;
	t->next = NULL;

    return t;
}

void addTower(Tower* t, TowerList* list){
    if(*list == NULL){
        *list = t;
    }else{
        Tower *temp = *list;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = t;
    }
}

void freeTower(Tower* t){
	if(t != NULL){
		if(t->sprite){
      		glDeleteTextures(1, &(t->sprite));
    	}
    	free(t);
    }
}

void freeTowers(TowerList* list){
	if(list != NULL){
		Tower *temp = *list;
    	Tower *next;
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

void deleteTower(Tower* t, TowerList* list){
	if(t != NULL && *list != NULL){
	 	TowerList *modif = list;
	    TowerList toDelete = *modif;
		while(toDelete != NULL && toDelete->x != t->x && toDelete->y != t->y){
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

void drawTowers(TowerList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			glScalef(list->size, list->size, 0);
			drawCircle(list->r, list->g, list->b, 255);
			drawPicture(list->sprite);
		glPopMatrix();

        list = list->next;
    }
}

void drawRangeTowers(TowerList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			glScalef((list->range)*(list->rangeMultiplier), (list->range)*(list->rangeMultiplier), 0);
			drawCircle(list->r, list->g, list->b, 40);
		glPopMatrix();

        list = list->next;
    }
}

void drawInfosTower(Tower* t, char* infosConstructions){
	if(t != NULL){
		sprintf(infosConstructions, "Cout : %d\nPuissance : %d\nPortee : %d\nCadence : %d\n", t->cost, (int)(t->power*t->powerMultiplier), (int)(t->range*t->rangeMultiplier), (int)(t->rate*t->rateMultiplier));
	}
}

Tower* towerSelected(TowerList list, float x, float y){
	while(list != NULL){
		if(isSquareIntersectsCircle(list->x, list->y, x, y, list->size, 0)){
		    return list;
		}
		list = list->next;
	}
    return 0;
}

Tower* towerIntersection(TowerList list, float xNew, float yNew, int sizeNew, shape shapeNew){
	if(shapeNew == CIRCLE){
		while(list != NULL){
			if(isCircleIntersectsCircle(list->x, list->y, xNew, yNew, list->size, sizeNew)){
			    return list;
			}
			list = list->next;
		}
	}else if(shapeNew == SQUARE){
		while(list != NULL){
			if(isSquareIntersectsSquare(list->x, list->y, xNew, yNew, list->size, sizeNew)){
			    return list;
			}
			list = list->next;
		}
	}
    return 0;
}