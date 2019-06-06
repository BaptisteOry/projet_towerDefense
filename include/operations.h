#ifndef OPERATIONS_H__
#define OPERATIONS_H__

#include "../include/node.h"


void slice_str(const char * str, char * buffer, int start, int end);
int isCircleIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsSquare(float x1, float y1, float x2, float y2, int size1, int size2);
int isSquareIntersectsRectangle(float x1, float y1, float x2, float y2, int size1, int size2w, int size2h);
float convertWindowGLViewWidth(float x, unsigned int WINDOW_WIDTH, float GL_VIEW_WIDTH);
float convertWindowGLViewHeight(float y, unsigned int WINDOW_HEIGHT, float GL_VIEW_HEIGHT);
int randomRange(int min, int max);
Node* dijkstra(Node* nodes, int nbOfNodes);
int distance(Node* node1, Node* node2);
void reverse (NodeList* n);
Node* randomPath(Node* nodes);
#endif