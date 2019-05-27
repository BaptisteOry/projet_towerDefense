#ifndef TOWER_H__
#define TOWER_H__

/// \enumeration types de tours
typedef enum{
	TRED, TGREEN, TYELLOW, TBLUE
}towerType;

/// \structure tour
typedef struct Tower {
	towerType type; // Type
	float x, y; // Positions

	int power; // Puissance
	float powerMultiplier;
	int range; // Portée
	float rangeMultiplier;
	int rate; // Cadence
	float rateMultiplier;
	int cost; // Coût

	GLuint sprite; // Sprite
	int r, g, b; // Couleur
	int size; // Taille
	shape shape; // Forme

	struct Tower *next; // Tour suivante
} Tower, *TowerList;

Tower* allocTower(towerType type, float x, float y);
void addTower(Tower* t, TowerList* list);
void freeTower(Tower* t);
void freeTowers(TowerList* list);
void deleteTower(Tower* t, TowerList* list);
void drawTowers(TowerList list);
void drawRangeTowers(TowerList list);
void drawInfosTower(Tower* t, char* infosConstructions);
Tower* towerSelected(TowerList list, float x, float y);
Tower* towerIntersection(TowerList towers, float xNew, float yNew, int sizeNew, shape shapeNew);

#endif