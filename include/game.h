#ifndef GAME_H__
#define GAME_H__

typedef struct game{
	int money;
	int time;
}Game;

Game* allocGame();
void freeGame(Game* game);

#endif