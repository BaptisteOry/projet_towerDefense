#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <GL/freeglut.h> 
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/display.h"

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

void drawPicture(GLuint texture, int r, int g, int b, int a){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
        glColor4ub(r, g, b, a);

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