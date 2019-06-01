#ifndef OPERATIONS_H__
#define OPERATIONS_H__

void slice_str(const char * str, char * buffer, int start, int end);
int isCircleIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsSquare(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsRectangle(float x1, float y1, float x2, float y2, int size1, int size2w, int size2h);
float convertWindowGLViewWidth(float x, unsigned int WINDOW_WIDTH, float GL_VIEW_WIDTH);
float convertWindowGLViewHeight(float y, unsigned int WINDOW_HEIGHT, float GL_VIEW_HEIGHT);
int randomRange(int min, int max);
int doesCircleIntersectsPath(float x, float y, int size, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

#endif