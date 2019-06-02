#ifndef MONSTER_H__
#define MONSTER_H__

/// \enumeration types de monstres
typedef enum{
	MSAD, MANGRY, MNUMBER
}monsterType;

typedef enum {
	UP, DOWN, LEFT, RIGHT, NONE
}direction;

/// \structure tour
typedef struct Monster {
	monsterType type; // Type
	float x, y; // Positions
	direction direction;

	int loot;
	int healthPoints;
	int speed;

	GLuint sprite; // Sprite
	int r, g, b; // Couleur
	int size; // Taille
	shape shape; // Forme

	struct Monster *next; // Monstre suivant
} Monster, *MonsterList;

Monster* allocMonster(monsterType type, float x, float y);
void addMonster(Monster* m, MonsterList* list);
void freeMonster(Monster* m);
void freeMonsters(MonsterList* list);
void deleteMonster(Monster* m, MonsterList* list);
void drawMonsters(MonsterList list);
void goUp(Monster* m);
void goDown(Monster* m);
void goLeft(Monster* m);
void goRight(Monster* m);
void moveMonster(Monster* m, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);
direction chooseDirection(Monster* m, Node* nodes, int nodeId);
int testOnNode(Monster* m, Node* nodes, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

#endif