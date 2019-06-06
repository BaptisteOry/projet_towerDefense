#ifndef MONSTER_H__
#define MONSTER_H__

#include "../include/node.h"

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
	Node* path; // Chemin

	int loot;
	int healthPoints;
	int healthPointsRatio;
	int sizeBarW, sizeBarH;
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
direction directionAB(Node* A, Node* B);
Monster* moveMonsterAB(Monster* m, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);
void initializeMonsterPath(Monster* m, Node* nodes);
void initializeMonsterPosition(Monster* m, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);
void moveMonsters(MonsterList* monsters, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);
int passNodeCenter(Monster* m, Node* n, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);

#endif