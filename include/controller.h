#ifndef CONTROLLER_H__
#define CONTROLLER_H__

#include "map.h"

/* Subdivisions cercle */
static const int CIRCLE_SUBDIVS = 2<<6;

typedef enum{
	CIRCLE, SQUARE
}shape;

GLuint loadTexture (char* fileName);
void drawPicture(GLuint texture);
void drawCircle(int red, int green, int blue, int alpha);
void drawSquare(int red, int green, int blue, int alpha);
void displayText(void *font, unsigned char* text, float x, float y, int red, int green, int blue);
int isCircleIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsSquare(float x1, float y1, float x2, float y2, int size1, int size2);
float convertWindowGLViewWidth(float x, unsigned int WINDOW_WIDTH, float GL_VIEW_WIDTH);
float convertWindowGLViewHeight(float y, unsigned int WINDOW_HEIGHT, float GL_VIEW_HEIGHT);
int randomRange(int min, int max);

int doesCircleIntersectsPath(float x, float y, int size, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

#endif