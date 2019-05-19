#ifndef BUILDING_H__
#define BUILDING_H__

/// \enumeration types de tours
typedef enum{
	RADAR, FACTORY, STOCK
}buildingType;

/// \structure bâtiment
typedef struct Building {
	buildingType type; // Type
	float x, y; // Positions

	int range; // Portée
	int cost; // Coût

	GLuint sprite; // Sprite
	int r, g, b; // Couleur
	int size; // Taille

	struct Building *next; // Bâtiment suivant
} Building, *BuildingList;

Building* allocBuilding(buildingType type, float x, float y, int size);
void addBuilding(Building* b, BuildingList* list);
int deleteBuilding(Building* b, BuildingList* list);
void deleteBuildings(BuildingList* list);
void drawBuildings(BuildingList list);
void drawRangeBuildings(BuildingList list);
Building* buildingSelected(BuildingList list, float x, float y);

#endif