#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/game.h"

Game* allocGame(){
    Game* g = (Game*) malloc(sizeof(Game));
    if(g == NULL) {
        fprintf(stderr, "MEMORY ERROR\n");
        exit(1);
    }
    g->money = 500;
    g->time = 0;

    return g;
}

void freeGame(Game* game){
    if(game != NULL){
        free(game);
    }
}