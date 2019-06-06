#ifndef GAME_H__
#define GAME_H__

typedef struct Game{
	int money;

	int nbWave; // Nombre de vagues
	int lastWave; // Numéro de la dernière vague
	int nbMonstersPerWave; // Nombre de monstres par vagues
	int timeWave ; // Temps entre chaque vague
    int timeAddWave; // Temps d'apparition des monstres pour une vague

    int lose;
	int status; // 0->begin,1->play,2->pause,3->end
}Game;

Game* allocGame();
void freeGame(Game* g);
void addWave(Game* g, MonsterList* list, int counter, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);
void killMonsters(MonsterList* listMonsters, TowerList* listTowers, int counter);
void endGame(Game* g, MonsterList* list);

#endif