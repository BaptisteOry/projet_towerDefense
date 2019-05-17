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
		fprintf(stderr, "MEMORY ERROR\n");
		exit(1);
	}

    t->type = type; // Type
	t->x = x; // Position x
	t->y = y; // Position y

    t->next = NULL;
	
	switch(type){
		case TRED :
			t->power = 100;
			t->range= 300;
			t->rate = 10; //every second
			t->cost = 500;
			break;
		case TGREEN:
			t->power = 50;
			t->range= 200;
			t->rate = 50; //every 200ms
			t->cost = 300;
			break;
		case TYELLOW:
			t->power = 30;
			t->range= 100;
			t->rate = 30; //every 600ms
			t->cost = 250;
			break;
		case TBLUE:
			t->power = 30;
			t->range= 40;
			t->rate = 30; //every 400ms
			t->cost = 400;
			break;
		default:
			break;
	}

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

void deleteTowers(TowerList* list){
    Tower *temp = *list;
    Tower *next;
    while (NULL != temp)
    {
        next = temp->next;
        free(temp);
        temp = next;
    }
    *list = NULL;
}

void drawTowers(TowerList list){
	GLuint towerTexture = 0;
	
    while(list != NULL){
    	switch(list->type){
			case TRED:
				towerTexture = loadTexture("images/tow1.png");
				break;
			case TGREEN:
				towerTexture = loadTexture("images/tower_green.png");
				break;
			case TYELLOW:
				towerTexture = loadTexture("images/tower_yellow.png");
				break;
			case TBLUE:
				towerTexture = loadTexture("images/tower_blue.png");
				break;
			default:
				break;
		}

		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			drawPicture(towerTexture, 10, 10); // Taille tour
		glPopMatrix();

        list = list->next;
    }
}

void drawTower(Tower* t){
    GLuint towerTexture = 0;
    
    switch(t->type){
        case TRED:
            towerTexture = loadTexture("images/tower_red.png");
            break;
        case TGREEN:
            towerTexture = loadTexture("images/tower_green.png");
            break;
        case TYELLOW:
            towerTexture = loadTexture("images/tower_yellow.png");
            break;
        case TBLUE:
            towerTexture = loadTexture("images/tower_blue.png");
            break;
        default:
            break;
    }

    glPushMatrix();
        printf("%f\n", t->x);
        printf("%f\n", t->y);
        glTranslatef(t->x, t->y, 0);
        drawPicture(towerTexture, 10, 10); // Taille tour
    glPopMatrix();
}