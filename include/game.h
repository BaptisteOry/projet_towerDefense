#ifndef GAME_H__
#define GAME_H__

typedef struct Game{
	int money;

	int nbWave; // Nombre de vagues
	int nbMonstersPerWave; // Nombre de monstres par vagues
	int timeWave ; // Temps entre chaque vague
    int timeAddWave; // Temps d'apparition des monstres pour une vague

	int status; // 0->begin,1->play,2->pause,3->end
}Game;

Game* allocGame();
void freeGame(Game* g);
void addWave(Game* g, MonsterList* list, int counter);
void killMonsters(MonsterList* listMonsters, TowerList* listTowers, int counter);

#endif