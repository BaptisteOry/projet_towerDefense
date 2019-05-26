#ifndef GAME_H__
#define GAME_H__

typedef struct game{
	int money;
	int nbWave;
	int nbMonstersWave;
	int end;
}Game;

Game* allocGame();
void freeGame(Game* game);
void addWave(Game* g, MonsterList* list);

#endif