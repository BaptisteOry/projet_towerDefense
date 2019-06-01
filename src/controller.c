#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"

GLuint loadTexture (char* fileName){
	/* Chargement d'une image sur CPU */
    SDL_Surface* imgTexture = IMG_Load(fileName);
    if(imgTexture == NULL) {
        fprintf(stderr, "Echec du chargement de l'image %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    /* Chargement de la texture sur GPU */
    GLuint texture;
    glGenTextures(1, &texture);

    /* Paramètres pour utiliser la texture */
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLenum format;
    switch(imgTexture->format->BytesPerPixel) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            fprintf(stderr, "Format des pixels de l'image %s non supporte.\n", fileName);
            return EXIT_FAILURE;
    }

    /* Envoyer image CPU à texture GPU */
    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 imgTexture->w, imgTexture->h, 0,
                 format, GL_UNSIGNED_BYTE, imgTexture->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Liberation mémoire image */
    SDL_FreeSurface(imgTexture);

    return texture;
}

void drawPicture(GLuint texture){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
        glColor3ub(255, 255, 255);

        glTexCoord2f(0, 1);
        glVertex2f(-1, -1);
    
        glTexCoord2f(1, 1);
        glVertex2f(1, -1);
    
        glTexCoord2f(1, 0);
        glVertex2f(1, 1);
    
        glTexCoord2f(0, 0);
        glVertex2f(-1, 1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void drawCircle(int red, int green, int blue, int alpha){
    int i = 0;

    glBegin(GL_TRIANGLE_FAN);
        glColor4ub(red, green, blue, alpha);
        glVertex2f(0.0, 0.0);

        for(i = 0; i < CIRCLE_SUBDIVS; i++){
            glVertex2f( (cos(i * (2 * M_PI / (float) CIRCLE_SUBDIVS)))*1, 
                        (sin(i * (2 * M_PI / (float) CIRCLE_SUBDIVS)))*1);
        }
        glVertex2f( (cos(i * (2 * M_PI / (float) CIRCLE_SUBDIVS))*1), 
                    (sin(i * (2 * M_PI / (float) CIRCLE_SUBDIVS)))*1);
    glEnd();
}

void drawSquare(int red, int green, int blue, int alpha){
    glBegin(GL_TRIANGLE_FAN);
    glColor4ub(red, green, blue, alpha);
        glVertex2f(0.0, 0.0);
        
        glVertex2f(1 , -1);
        glVertex2f(1 , 1);
        glVertex2f(-1 , 1);
        glVertex2f(-1 , -1);
        glVertex2f(1 , -1);
    glEnd();
}

void displayText(void *font, unsigned char* text, float x, float y, int red, int green, int blue){
  const unsigned char* textToDisplay = text;
  glPushMatrix();
    glColor3ub(red, green, blue); 
    glRasterPos2f(x, y);
    glutBitmapString(font, textToDisplay);
  glPopMatrix();
}

int isCircleIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2){
    if( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1) < (size1 + size2)*(size1 + size2)){
        return 1;
    }else{
        return 0;
    }
}

int isSquareIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2){
    if(    x1 + (size1 + size2) >= x2
        && x1 - (size1 + size2) <= x2
        && y1 + (size1 + size2) >= y2
        && y1 - (size1 + size2) <= y2){
        return 1;
    }else{
        return 0;
    }
}

int isSquareIntersectsSquare(float x1, float y1, float x2, float y2, int size1, int size2){
    if(    x1 + (size1 + size2) >= x2
        && x1 - (size1 + size2) <= x2
        && y1 + (size1 + size2) >= y2
        && y1 - (size1 + size2) <= y2){
        return 1;
    }else{
        return 0;
    }
}

float convertWindowGLViewWidth(float x, unsigned int WINDOW_WIDTH, float GL_VIEW_WIDTH){
    return (-GL_VIEW_WIDTH + GL_VIEW_WIDTH*2 * x / WINDOW_WIDTH);
}

float convertWindowGLViewHeight(float y, unsigned int WINDOW_HEIGHT, float GL_VIEW_HEIGHT){
    return -(-GL_VIEW_HEIGHT + GL_VIEW_HEIGHT*2 * y / WINDOW_HEIGHT);
}

int randomRange(int min, int max){
    return rand()%(max-min+1) + min;
}

int doesCircleIntersectsPath(float x, float y, int size, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT) {
    printf("oui\n");
    Node* temp = nodes;
    float X = 10*x/WINDOW_WIDTH-0.5;
    float Y = 6*y/WINDOW_HEIGHT-0.5;
    while(temp != NULL) {
        if(X>temp->x-1 && X<temp->x+1) {
            printf("ouii\n");
            if(Y>temp->y-1 && Y<temp->y+1) {
                printf("ouiii\n");
                return 1;
            }
            Node* linked = temp->linkedNodes;
            while(linked != NULL) {
                printf("ouiiii\n");
                if((Y>linked->y && Y<temp->y) || (Y>temp->y && Y<linked->y)) {
                    return 1;
                }
                linked = linked->nextNode;
            }
        }
        else if(Y>temp->y-1 && Y<temp->y+1) {
            printf("noon\n");
            if(X>temp->x-1 && X<temp->x+1) {
                return 1;
            }
            Node* linked = temp->linkedNodes;
            while(linked != NULL) {
                if((X>linked->x && X<temp->x) || (X>temp->x && X<linked->x)) {
                    return 1;
                }
                linked = linked->nextNode;
            }
        }
        temp = temp->nextNode;
    }
    return 0;
}