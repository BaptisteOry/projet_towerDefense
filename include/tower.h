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
	int range; // Portée
	int rate; // Cadence
	int cost; // Coût

	struct Tower *next; // Tour suivante
} Tower, *TowerList;

#endif