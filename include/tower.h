#ifndef TOWER_H__
#define TOWER_H_
/// \enumeration types de tours
typedef enum{
	TRED, TPURPLE, TYELLOW, TBLUE
}towerType;

/// \structure tour
typedef struct Tower {
	towerType type; // Type
	float x, y; // Positions
	int constructible; // Construisible

	int power; // Puissance
	float powerBonus;
	int range; // Portée
	float rangeBonus;
	int rate; // Cadence
	float rateBonus;
	int cost; // Coût

	GLuint sprite; // Sprite
	int r, g, b, a; // Couleurs
	int rRange, gRange, bRange, aRange; // Couleurs portée
	int size; // Taille
	shape shape; // Forme

	struct Tower *next; // Tour suivante
} Tower, *TowerList;

const char* getTowerTypeName(towerType tt);
Tower* allocTower(towerType type, float x, float y);
void addTower(Tower* t, TowerList* list);
Tower* freeTower(Tower* t);
void freeTowers(TowerList* list);
void deleteTower(Tower* t, TowerList* list);
void drawTower(Tower* t);
void drawTowers(TowerList list);
void drawRangeTowers(TowerList list);
void drawInfosTower(Tower* t, char* infosConstructions);
Tower* towerSelected(TowerList list, float x, float y);
Tower* towerIntersection(TowerList towers, float xNew, float yNew, int sizeNew, shape shapeNew);

#endif