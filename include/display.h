#ifndef DISPLAY_H__
#define DISPLAY_H__

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

#endif