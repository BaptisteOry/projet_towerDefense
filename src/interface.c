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
#include "../include/tower.h"
#include "../include/game.h"
#include "../include/interface.h"

Interface* allocInterface(float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT){
    Interface* i = (Interface*) malloc(sizeof(Interface));
    if(i == NULL) {
        fprintf(stderr, "MEMORY ERROR\n");
        exit(1);
    }

    i->wLogo = 40; i->hLogo = 16;
    i->xLogo = -GL_VIEW_WIDTH+(i->wLogo)+5; i->yLogo = GL_VIEW_HEIGHT-(i->hLogo)-5;
    i->wLogo2 = 50; i->hLogo2 = 20;
    i->xLogo2 = GL_VIEW_WIDTH-(i->wLogo2)-5; i->yLogo2 = -GL_VIEW_HEIGHT+(i->hLogo2)+5;
    i->spriteLogo = loadTexture("images/title.png");

    i->xMenu = -175; i->yMenu = 20;
    i->wMenuB = 45; i->hMenuB = 10;
    i->xMenuB = 0; i->yMenuB = -20;

    i->wInfosGame = 58; i->hInfosGame = 16;
    i->xInfosGame = -GL_VIEW_WIDTH+(i->wInfosGame)+5; i->yInfosGame = GL_VIEW_HEIGHT-(i->hInfosGame)-40;

    i->wInfosConstructions = 50; i->hInfosConstructions = 35;
    i->xInfosConstructions = GL_VIEW_WIDTH-(i->wInfosConstructions)-5; i->yInfosConstructions = GL_VIEW_HEIGHT-(i->hInfosConstructions)-5;

    i->wQuitB = 22; i->hQuitB = 10;
    i->xQuitB = GL_VIEW_WIDTH-(i->wQuitB)-5; i->yQuitB = -GL_VIEW_HEIGHT+(i->hQuitB)+5;

    i->wFull = GL_VIEW_WIDTH; i->hFull = GL_VIEW_HEIGHT; 

    i->xText = 5; i->yText = -10; 
    i->font = GLUT_BITMAP_9_BY_15;

    i->r = 110; i->g = 162; i->b = 66; i->a = 235;
    i->r2 = 255; i->g2 = 247; i->b2 = 193; i->a2 = 255;

    strcpy(i->infosConstructions, "");

    return i;
}

void freeInterface(Interface* i){
    if(i != NULL){
        if(i->spriteLogo){
            glDeleteTextures(1, &(i->spriteLogo));
        }
        free(i);
    }
}

void drawBeginning(Interface* f){
    char bufferText[255] = "";
    glPushMatrix();
        glPushMatrix();
            glScalef(f->wFull, f->hFull, 0);
            drawSquare(f->r, f->g, f->b, 255);
        glPopMatrix();
        sprintf(bufferText, "Bienvenue sur Flower Tower !\nVous vous appretez a combattre les emotions negatives grace aux fleurs.");
        displayText(f->font, (unsigned char*)bufferText, f->xMenu, f->yMenu, 255, 255, 255);
    glPopMatrix();
    // Bouton jouer
    glPushMatrix();
        glTranslatef(f->xMenuB, f->yMenuB, 0);
        glPushMatrix();
            glScalef(f->wMenuB, f->hMenuB, 0);
            drawSquare(f->r2, f->g2, f->b2, 255);
        glPopMatrix();
        sprintf(bufferText, "Jouer");
        displayText(f->font, (unsigned char*)bufferText, -12, -2, f->r, f->g, f->b);
    glPopMatrix();
    // Logo
    glPushMatrix();
        glTranslatef(f->xLogo2, f->yLogo2, 0);
        glScalef(f->wLogo2, f->hLogo2, 0);
        drawPicture(f->spriteLogo, 255, 255, 255, 255);
    glPopMatrix();
}

void drawGameElements(Interface* f, Game* g){
    char bufferText[255] = "";
    // Logo
    glPushMatrix();
        glTranslatef(f->xLogo, f->yLogo, 0);
        glScalef(f->wLogo, f->hLogo, 0);
        drawPicture(f->spriteLogo, 255, 255, 255, 255);
    glPopMatrix();
    // Bouton quitter
    glPushMatrix();
        glTranslatef(f->xQuitB, f->yQuitB, 0);
        glPushMatrix();
            glScalef(f->wQuitB, f->hQuitB, 0);
            drawSquare(f->r, f->g, f->b, 255);
        glPopMatrix();
        sprintf(bufferText, "Quitter");
        displayText(f->font, (unsigned char*)bufferText, -15.5, -2, 255, 255, 255);
    glPopMatrix();
    // Infos jeu
    glPushMatrix();
        glTranslatef(f->xInfosGame, f->yInfosGame, 0);
        glPushMatrix();
            glScalef(f->wInfosGame, f->hInfosGame, 0);
            drawSquare(f->r, f->g, f->b, f->a);
        glPopMatrix();
        sprintf(bufferText, "Argent : %d\nVague : %d/%d\n>Touche \"H\" pour l'aide", g->money, g->nbWave, g->lastWave);
        displayText(f->font, (unsigned char*)bufferText, -(f->wInfosGame)+(f->xText), (f->hInfosGame)+(f->yText), 255, 255, 255);
    glPopMatrix();
}

void drawHelp(Interface* f){
    char bufferText[510] = "";
    glPushMatrix();
        glPushMatrix();
            glScalef(f->wFull, f->hFull, 0);
            drawSquare(f->r, f->g, f->b, f->a);
        glPopMatrix();
        sprintf(bufferText, "Aide Flower Tower :\n\nA / Z / E / R + clic gauche : construire une tour\n> A : tour rouge\n> Z : tour violette\n> E : tour jaune\n> R : tour bleue\n\nQ / S / D + clic gauche : construire un batiment\n> Q : radar\n> S : usine\n> D : stock\n\nClic gauche sur construction : consulter les informations\n\nClic droit sur construction : detruire une construction");
        displayText(f->font, (unsigned char*)bufferText, -(f->wFull)+(f->xText), (f->hFull)+(f->yText), 255, 255, 255);
    glPopMatrix();
    // Logo
    glPushMatrix();
        glTranslatef(f->xLogo2, f->yLogo2, 0);
        glScalef(f->wLogo2, f->hLogo2, 0);
        drawPicture(f->spriteLogo, 255, 255, 255, 255);
    glPopMatrix();
}

void drawEnd(Interface* f, Game* g){
    char bufferText[255] = "";
    glPushMatrix();
        glPushMatrix();
            glScalef(f->wFull, f->hFull, 0);
            drawSquare(f->r, f->g, f->b, 255);
        glPopMatrix();
        if(g->lose == 1){
            sprintf(bufferText, "Vous n'avez pas sauve le monde des emotions negatives.\nReessayez encore une fois !");
        } else {
            sprintf(bufferText, "Bravo ! Vous avez gagne !");
        }
        displayText(f->font, (unsigned char*)bufferText, f->xMenu, f->yMenu, 255, 255, 255);
    glPopMatrix();
    // Bouton jouer
    glPushMatrix();
        glTranslatef(f->xMenuB, f->yMenuB, 0);
        glPushMatrix();
            glScalef(f->wMenuB, f->hMenuB, 0);
            drawSquare(f->r2, f->g2, f->b2, 255);
        glPopMatrix();
        sprintf(bufferText, "Recommencer");
        displayText(f->font, (unsigned char*)bufferText, -24, -2, f->r, f->g, f->b);
    glPopMatrix();
    // Logo
    glPushMatrix();
        glTranslatef(f->xLogo2, f->yLogo2, 0);
        glScalef(f->wLogo2, f->hLogo2, 0);
        drawPicture(f->spriteLogo, 255, 255, 255, 255);
    glPopMatrix();
}

void drawInfosConstructions(Interface* f){
    if(strcmp(f->infosConstructions, "") != 0){
        glPushMatrix();
            glTranslatef(f->xInfosConstructions, f->yInfosConstructions, 0);
            glPushMatrix();
                glScalef(f->wInfosConstructions, f->hInfosConstructions, 0);
                drawSquare(f->r, f->g, f->b, f->a);
            glPopMatrix();
            displayText(f->font, (unsigned char*)(f->infosConstructions), -(f->wInfosConstructions)+(f->xText), (f->hInfosConstructions)+(f->yText), 255, 255, 255);
        glPopMatrix();
    }
}

int menuBSelected(Interface* f, float x, float y){
    if(isSquareIntersectsRectangle(x, y, f->xMenuB, f->yMenuB, 0, f->wMenuB, f->hMenuB)){
        return 1;
    }
    return 0;
}

int quitSelected(Interface* f, float x, float y){
    if(isSquareIntersectsRectangle(x, y, f->xQuitB, f->yQuitB, 0, f->wQuitB, f->hQuitB)){
        return 1;
    }
    return 0;
}