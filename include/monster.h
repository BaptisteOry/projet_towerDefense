#ifndef MONSTER_H__
#define MONSTER_H__

/// \enumeration types de monstres
typedef enum{
	MSAD, MANGRY, MNUMBER
}monsterType;

/// \structure tour
typedef struct Monster {
	monsterType type; // Type
	float x, y; // Positions

	int loot;
	float lootMultiplier;
	int healthPoints;
	float healthPointsMultiplier;
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

#endif