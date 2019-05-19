#ifndef CONTROLLER_H__
#define CONTROLLER_H__

/* Subdivisions cercle */
static const int CIRCLE_SUBDIVS = 2<<6;

GLuint loadTexture (char* fileName);
void drawPicture(GLuint texture, int xScale, int yScale);
void drawCircle(int red, int green, int blue, int alpha, int size);
int isShapeIntersectsShape(float x1, float y1, float x2, float y2, int size1, int size2);


#endif