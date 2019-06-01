#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/operations.h"

void slice_str(const char * str, char * buffer, int start, int end) {
    int j = 0;
    for (int i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
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

int isSquareIntersectsRectangle(float x1, float y1, float x2, float y2, int size1, int size2w, int size2h){
    printf("%f %f %f %f %d %d %d\n", x1, y1, x2, y2, size1, size2w, size2h);
    if(    x1 + (size2w + size1) >= x2
        && x1 - (size2w + size1) <= x2
        && y1 + (size2h + size1) >= y2
        && y1 - (size2h + size1) <= y2){
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