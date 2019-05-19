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

	float power; // Puissance
	float range; // Portée
	float rate; // Cadence
	int cost; // Coût

	GLuint sprite; // Sprite
	int r, g, b; // Couleur
	int size; // Taille

	struct Tower *next; // Tour suivante
} Tower, *TowerList;

Tower* allocTower(towerType type, float x, float y);
void addTower(Tower* t, TowerList* list);
void freeTower(Tower* t);
int deleteTower(Tower* t, TowerList* list);
void deleteTowers(TowerList* list);
void drawTowers(TowerList list);
void drawRangeTowers(TowerList list);
Tower* towerSelected(TowerList list, float x, float y);
Tower* towerIntersection(TowerList towers, float xNew, float yNew, int sizeNew, shape shapeNew);

#endif