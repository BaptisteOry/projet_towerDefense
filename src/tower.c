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
#include "../include/tower.h"

const char* getTowerTypeName(towerType tt){
   switch (tt){
    	case TRED: return "Fleur rouge";
      		break;
    	case TPURPLE: return "Fleur violette";
      		break;
    	case TYELLOW: return "Fleur jaune";
      		break;
      	case TBLUE: return "Fleur bleue";
      		break;
      	default:
			break;
   }
   return 0;
}

Tower* allocTower(towerType type, float x, float y){
    Tower* t = (Tower*) malloc(sizeof(Tower));
    if(t == NULL) {
		printf("allocTower : erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}

    t->type = type;
	t->x = x;
	t->y = y;
	t->constructible = 0;
	t->powerBonus = 1;
	t->rangeBonus = 1;
	t->rateBonus = 1;
	switch(type){
		case TRED :
			t->power = 50;
			t->range = 100;
			t->rate = 2000; // Toutes les 2 secondes
			t->cost = 50;
			t->rRange = 217; t->gRange = 55; t->bRange = 30;
			t->sprite = loadTexture("images/tower_red.png");
			break;
		case TPURPLE:
			t->power = 30;
			t->range = 90;
			t->rate = 600; // Toutes les 600ms
			t->cost = 65;
			t->rRange = 130; t->gRange = 78; t->bRange = 139;
			t->sprite = loadTexture("images/tower_purple.png");
			break;
		case TYELLOW:
			t->power = 20;
			t->range = 75;
			t->rate = 350; // Toutes les 350ms
			t->cost = 35;
			t->rRange = 249; t->gRange = 171; t->bRange = 60;
			t->sprite = loadTexture("images/tower_yellow.png");
			break;
		case TBLUE:
			t->power = 25;
			t->range = 125;
			t->rate = 500; // Toutes les 500ms
			t->cost = 75;
			t->rRange = 43; t->gRange = 117; t->bRange = 140;
			t->sprite = loadTexture("images/tower_blue.png");
			break;
		default:
			break;
	}
	t->aRange = 75;
	t->r = 255; t->g = 255; t->b = 255; t->a = 255;
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

Tower* freeTower(Tower* t){
	if(t != NULL){
		if(t->sprite){
      		glDeleteTextures(1, &(t->sprite));
    	}
    	free(t);
    }
    return NULL;
}

void freeTowers(TowerList* list){
	if(*list != NULL){
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
			drawPicture(list->sprite, list->r, list->g, list->b, list->a);
		glPopMatrix();

        list = list->next;
    }
}

void drawTower(Tower* t){
	if(t != NULL){
	    glPushMatrix();
			glTranslatef(t->x, t->y, 0);
			glScalef(t->size, t->size, 0);
			drawPicture(t->sprite, t->r, t->g, t->b, t->a);
		glPopMatrix();
	}
}

void drawRangeTowers(TowerList list){
    while(list != NULL){
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			glScalef((list->range)*(list->rangeBonus), (list->range)*(list->rangeBonus), 0);
			drawCircle(list->rRange, list->gRange, list->bRange, list->aRange);
		glPopMatrix();

        list = list->next;
    }
}

void drawInfosTower(Tower* t, char* infosConstructions){
	if(t != NULL){
		sprintf(infosConstructions, "Type : %s\nCout : %d\nPuissance : %d\nPortee : %d\nCadence : %d\n", getTowerTypeName(t->type), t->cost, (int)(t->power*t->powerBonus), (int)(t->range*t->rangeBonus), (int)(t->rate*t->rateBonus));
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