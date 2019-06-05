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

Monster* allocMonster(monsterType type, float x, float y){
    Monster* m = (Monster*) malloc(sizeof(Monster));
    if(m == NULL) {
		printf("allocMonster : erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}

    m->type = type; // Type
	m->x = x; // Position x
	m->y = y; // Position y
	m->loot = 5; // Butin
	switch(type){
		case MSAD :
			m->healthPoints = 10;
			m->speed = 10;
			m->r = 255; m->g = 255; m->b = 255;
			m->sprite = loadTexture("images/monster_sad.png");
			break;
		case MANGRY:
			m->healthPoints = 15;
			m->speed = 20;
			m->r = 255; m->g = 255; m->b = 255;
			m->sprite = loadTexture("images/monster_angry.png");
			break;
		default:
			break;
	}
	m->sizeBarW = 14; m->sizeBarH = 2;
	m->healthPointsRatio = m->healthPoints;
	m->size = 20;
	m->shape = CIRCLE;
	m->next = NULL;

    return m;
}

void addMonster(Monster* m, MonsterList* list){
    if(*list == NULL){
        *list = m;
    }else{
        Monster *temp = *list;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = m;
    }
}

void freeMonster(Monster* m){
	if(m != NULL){
		if(m->sprite){
      		glDeleteTextures(1, &(m->sprite));
    	}
    	free(m);
    }
}

void freeMonsters(MonsterList* list){
	if(list != NULL){
		Monster *temp = *list;
    	Monster *next;
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

void deleteMonster(Monster* m, MonsterList* list){
	if(m != NULL && *list != NULL){
	 	MonsterList *modif = list;
	    MonsterList toDelete = *modif;
		while(toDelete != NULL && toDelete->x != m->x && toDelete->y != m->y){
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

void drawMonsters(MonsterList list){
    while(list != NULL){
    	glPushMatrix();
    		glTranslatef(list->x, list->y, 0);
			glPushMatrix();
				glScalef(list->size, list->size, 0);
				drawCircle(list->r, list->g, list->b, 255);
				drawPicture(list->sprite, 255, 255, 255, 255);
			glPopMatrix();
			// Bar points de santé
			glPushMatrix();
				glTranslatef(0, (list->size)+6, 0);
				glPushMatrix();
					glScalef(list->sizeBarW, list->sizeBarH, 0);
					drawSquare(0, 0, 0, 255);
				glPopMatrix();
				glPushMatrix();
					int healthPointsBar = ((list->sizeBarW)*(list->healthPoints))/(list->healthPointsRatio);
					glTranslatef((healthPointsBar-(list->sizeBarW)), 0, 0);
					glScalef(healthPointsBar, list->sizeBarH, 0);
					(healthPointsBar<(list->sizeBarW)/2) ? drawSquare(255, 0, 0, 255) : drawSquare(0, 255, 0, 255);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();

        list = list->next;
    }
}

void moveMonster(Monster* m, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) {
	int nodeId = testOnNode(m, nodes, WINDOW_WIDTH, WINDOW_HEIGHT);
	switch(m->direction) {
		case UP : goUp(m);
			break;
		case DOWN : goDown(m);
			break;
		case LEFT : goLeft(m);
			break;
		case RIGHT : goRight(m);
			break;
		case NONE : chooseDirection(m, nodes, nodeId);
	}
}

void goUp(Monster* m) {
	m->y += 1;
}

void goDown(Monster* m) {
	m->y += -1;
}

void goLeft(Monster* m) {
	m->x += -1;
}

void goRight(Monster* m) {
	m->x += 1;
}

direction chooseDirection(Monster* m, Node* nodes, int nodeId) {
	Node* temp = nodes;
	while(temp != NULL && temp->id != nodeId) {
		temp = temp->nextNode;
	}
	Node* links = temp->linkedNodes;
	int nbLinks = 0;
	while(links != NULL) {
		nbLinks++;
		links = links->nextNode;
	}
	links = temp->linkedNodes;
	for(int i=0; i<rand()%nbLinks; i++) {
		links = links->nextNode;
	}
	if(temp->x = links->x) {
		if(temp->y > links->y) {
			return UP;
		}
		else if(temp->y < links->y) {
			return DOWN;
		}
	}
	else if(temp->y = links->y) {
		if(temp->x > links->x) {
			return LEFT;
		}
		else if(temp->x < links->x) {
			return RIGHT;
		}
	}
	return NONE;
}

int testOnNode(Monster* m, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) {
	Node* temp = nodes;
	float x = 10*m->x/WINDOW_WIDTH-0.5;
	float y = 10*m->x/WINDOW_HEIGHT-0.5;
	while(temp != NULL) {
		if(x<temp->x+1 && x>temp->x-1 && y<temp->y+1 && y>temp->y-1) {
			m->direction = NONE;
			return temp->id;
		}
		temp = temp->nextNode;
	}
	return -1;
}