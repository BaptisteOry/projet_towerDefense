#include <stdio.h>
#include <stdlib.h>

#include "../include/tower.h"

Tower* allocTower(towerType type, float x, float y){
    Tower* t = (Tower*) malloc(sizeof(Tower));
    if(t == NULL) {
		fprintf(stderr, "MEMORY ERROR\n");
		exit(1);
	}

	t->next = NULL;
    t->type = type; // Type
	t->x = x; // Position x
	t->y = y; // Position y
	
	switch(type){
		case TRED :
			tower->power = 100;
			tower->range= 300;
			tower->rate = 10; //every second
			tower->cost = 500;
			break;
		case TGREEN:
			tower->power = 50;
			tower->range= 200;
			tower->rate = 50; //every 200ms
			tower->cost = 300;
			break;
		case TYELLOW:
			tower->power = 30;
			tower->range= 100;
			tower->rate = 30; //every 600ms
			tower->cost = 250;
			break;
		case TBLUE:
			tower->power = 30;
			tower->range= 40;
			tower->rate = 30; //every 400ms
			tower->cost = 400;
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
	GLuint towerTexture;
	
    while(list != NULL){
    	switch(list->type){
			case TRED:
				towerTexture = loadTexture("../images/tower_red.png");
				break;
			case TGREEN:
				towerTexture = loadTexture("../images/tower_green.png");
				break;
			case TYELLOW:
				towerTexture = loadTexture("../images/tower_yellow.png");
				break;
			case TBLUE:
				towerTexture = loadTexture("../images/tower_blue.png");
				break;
			default:
				break;
		}
		glPushMatrix();
			glTranslatef(list->x, list->y);
			drawPicture(towerTexture, 60, 60); // Taille tour
		glPopMatrix();

        list = list->next;
    }
}

void drawPicture(GLuint texture, int xScale, int yScale){
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
        glBegin(GL_POLYGON); //GL_LINE_STRIP
            glTexCoord2f(0, 0);
            glVertex2f(-xScale, -yScale);
            glTexCoord2f(0, 1);
            glVertex2f(-xScale, yScale);
            glTexCoord2f(1, 1);
            glVertex2f(xScale yScale);
            glTexCoord2f(1, 0);
            glVertex2f(xScale, -yScale);
        glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

GLuint loadTexture (char* fileName){
	/* Chargement d'une image sur CPU */
	SDL_Surface* imgTexture;
    imgTexture = IMG_Load(fileName);
    if(imgTexture == NULL)
    {
        fprintf(
            stderr, 
            "L'image n'a pas été chargée. Fin du programme.\n");
        exit(EXIT_FAILURE);
    }

    /* Chargement de la texture sur GPU */
    GLuint texture;
    glGenTextures(1, &texture);

    /* Paramètres pour utiliser la texture */
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* Envoyer image CPU à texture GPU */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 imgTexture->w, imgTexture->h, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, imgTexture->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}