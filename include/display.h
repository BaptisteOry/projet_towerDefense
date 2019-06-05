#ifndef DISPLAY_H__
#define DISPLAY_H__

#include "map.h"

/* Subdivisions cercle */
static const int CIRCLE_SUBDIVS = 2<<6;

typedef enum{
	CIRCLE, SQUARE
}shape;

GLuint loadTexture (char* fileName);
void drawPicture(GLuint texture, int r, int g, int b, int a);
void drawCircle(int red, int green, int blue, int alpha);
void drawSquare(int red, int green, int blue, int alpha);
void displayText(void *font, unsigned char* text, float x, float y, int red, int green, int blue);

int doesCircleIntersectsPath(float x, float y, int size, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

#endif