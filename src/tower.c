#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
#include "../include/tower.h"

Tower* allocTower(towerType type, float x, float y, int size){
    Tower* t = (Tower*) malloc(sizeof(Tower));
    if(t == NULL) {
		fprintf(stderr, "MEMORY ERROR\n");
		exit(1);
	}

    t->type = type; // Type
	t->x = x; // Position x
	t->y = y; // Position y
	switch(type){
		case TRED :
			t->power = 100;
			t->range= 135;
			t->rate = 10; //every second
			t->cost = 500;
			t->r = 255; t->g = 0; t->b = 0;
			t->sprite = loadTexture("images/tower_1.png");
			break;
		case TGREEN:
			t->power = 50;
			t->range= 110;
			t->rate = 85; //every 200ms
			t->cost = 300;
			t->r = 0; t->g = 255; t->b = 0;
			t->sprite = loadTexture("images/tower_2.png");
			break;
		case TYELLOW:
			t->power = 30;
			t->range= 95;
			t->rate = 30; //every 600ms
			t->cost = 250;
			t->r = 255; t->g = 255; t->b = 0;
			t->sprite = loadTexture("images/tower_3.png");
			break;
		case TBLUE:
			t->power = 30;
			t->range= 80;
			t->rate = 30; //every 400ms
			t->cost = 400;
			t->r = 0; t->g = 0; t->b = 255;
			t->sprite = loadTexture("images/tower_4.png");
			break;
		default:
			break;
	}
	t->size = size;
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

int deleteTower(Tower* t, TowerList* list){
	if(t == NULL || *list == NULL){
		return 0;
	}
 	TowerList *modif = list;
    TowerList toDelete = *modif;
	while(toDelete != NULL && toDelete->x != t->x && toDelete->y != t->y){
		modif = &toDelete->next;
		toDelete = *modif;
	}
	if(toDelete == NULL){
		printf("%s\n", "lol");
		return 0;
	}else{
		*modif = toDelete->next;
		free(toDelete);

		return 1;
	}
}

void deleteTowers(TowerList* list){
    Tower *temp = *list;
    Tower *next;
    while(temp != NULL){
        next = temp->next;
        free(temp);
        temp = next;
    }
    *list = NULL;
}

void drawTowers(TowerList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			drawCircle(list->r, list->g, list->b, 255, list->size);
			drawPicture(list->sprite, list->size, list->size); // Taille tour
		glPopMatrix();

        list = list->next;
    }
}

void drawRangeTowers(TowerList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			drawCircle(list->r, list->g, list->b, 40, list->range);
		glPopMatrix();

        list = list->next;
    }
}

Tower* towerSelected(TowerList list, float x, float y){
	while(list != NULL){
		if(isShapeIntersectsShape(list->x, list->y, x, y, list->size, 0)){
		    return list;
		}
		list = list->next;
	}
    return 0;
}