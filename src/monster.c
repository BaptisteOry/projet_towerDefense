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
#include "../include/node.h"

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
	m->direction = NONE; // Direction
	m->path = NULL; // Chemin à parcourrir
	switch(type){
		case MSAD :
			m->healthPoints = 10;
			m->speed = 3;
			m->r = 255; m->g = 255; m->b = 255;
			m->sprite = loadTexture("images/monster_sad.png");
			break;
		case MANGRY:
			m->healthPoints = 15;
			m->speed = 5;
			m->r = 255; m->g = 255; m->b = 255;
			m->sprite = loadTexture("images/monster_angry.png");
			break;
		default:
			break;
	}
	m->healthPointsRatio = m->healthPoints;
	m->sizeBarW = 14; m->sizeBarH = 2;
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

void goUp(Monster* m) {
	m->y += m->speed;
}

void goDown(Monster* m) {
	m->y += -m->speed;
}

void goLeft(Monster* m) {
	m->x += -m->speed;
}

void goRight(Monster* m) {
	m->x += m->speed;
}

direction directionAB(Node* A, Node* B) {
	if(A->x == B->x) {
		if(A->y > B->y) {
			return UP;
		}
		else if(A->y < B->y) {
			return DOWN;
		}
	}
	else if(A->y == B->y) {
		if(A->x > B->x) {
			return LEFT;
		}
		else if(A->x < B->x) {
			return RIGHT;
		}
	}
	return NONE;
}

Node* testOnNodeAB(Monster* m, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT) {
	Node* temp = nodes;
	while(temp != NULL) {
		float x = (2*GL_VIEW_WIDTH)*(temp->x+0.5)/10 - GL_VIEW_WIDTH;
		float y = GL_VIEW_HEIGHT - (2*GL_VIEW_HEIGHT)*(temp->y+0.5)/6;
		if(m->x<=x+(GL_VIEW_WIDTH/10) && m->x>=x-(GL_VIEW_WIDTH/10) && m->y<=y+(GL_VIEW_HEIGHT/6) && m->y>=y-(GL_VIEW_HEIGHT/6) /*x<temp->x+1 && x>temp->x-1 && y<temp->y+1 && y>temp->y-1*/) {
			if(m->path != NULL) {
				if(passNodeCenter(m, temp, GL_VIEW_WIDTH, GL_VIEW_HEIGHT) == 0) {
					m->direction = directionAB(temp, m->path->nextNode);
				}
				if(temp->id != m->path->id) {
					m->path = m->path->nextNode;
				}
			}
			return temp;
		}
		temp = temp->nextNode;
	}
	return NULL;
}

Monster* moveMonsterAB(Monster* m, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT) {
	if(m->path->nextNode == NULL && passNodeCenter(m, m->path, GL_VIEW_WIDTH, GL_VIEW_HEIGHT) == 0) {
		return m;
	}
	Node* A = testOnNodeAB(m, nodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
	//float x = (2*GL_VIEW_WIDTH)*(m->path->x)/10 - GL_VIEW_WIDTH + (GL_VIEW_WIDTH/10);
	//float y = GL_VIEW_HEIGHT - (2*GL_VIEW_HEIGHT)*(m->path->y)/6 - (GL_VIEW_HEIGHT/6);

	switch(m->direction) {
		case UP : goUp(m);
			/*if(m->x != x) {
				m->x = x;
			}*/
			break;
		case DOWN : goDown(m);
			/*if(m->x != x) {
				m->x = x;
			}*/
			break;
		case LEFT : goLeft(m);
			/*if(m->y != y) {
				m->y = y;
			}*/
			break;
		case RIGHT : goRight(m);
			/*if(m->y != y) {
				m->y = y;
			}*/
			break;
		case NONE : m->direction = directionAB(A, m->path);
			break;
	}
	return NULL;
}

void initializeMonsterPath(Monster* m, Node* nodes) {
	int r = rand()%2;
	if(r == 1) {
		m->path = dijkstra(nodes);
	}
	else {
		m->path = randomPath(nodes);
	}
}

void initializeMonsterPosition(Monster* m, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT) {
	float x = (2*GL_VIEW_WIDTH)*(m->path->x)/10 - GL_VIEW_WIDTH;
	float y = GL_VIEW_HEIGHT - (2*GL_VIEW_HEIGHT)*(m->path->y)/6 - (GL_VIEW_HEIGHT/6);
	m->x = x;
	m->y = y;
	m->direction = directionAB(m->path, m->path->nextNode);
}

int moveMonsters(MonsterList* monsters, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT) {
	Monster* temp = *monsters;
	int youDied = 0; // Pour gérer la fin de partie
	while(temp != NULL) {
		Monster* toDelete = NULL;
		toDelete = moveMonsterAB(temp, nodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
		if(toDelete != NULL) {
			youDied = 1;
		}
		deleteMonster(toDelete, monsters);
		temp = temp->next;
	}
	if(youDied == 1) {
		return 1;
	}
	return 0;
}

int passNodeCenter(Monster* m, Node* n, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT) {
	float x = (2*GL_VIEW_WIDTH)*(n->x+0.5)/10 - GL_VIEW_WIDTH;
	float y = GL_VIEW_HEIGHT - (2*GL_VIEW_HEIGHT)*(n->y+0.5)/6;
	switch(m->direction) {
		case UP : 
			if(m->y > y) {
				return 0;
			}
			break;
		case DOWN : 
			if(m->y < y) {
				return 0;
			}
			break;
		case LEFT : 
			if(m->x < x) {
				return 0;
			}
			break;
		case RIGHT : 
			if(m->x > x) {
				return 0;
			}
			break;
		case NONE :
			m->direction = directionAB(m->path, m->path->nextNode);
			break;
	}
	return 1;
}