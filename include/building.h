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
	int constructible; // Construisible

	int range; // Portée
	int cost; // Coût

	GLuint sprite; // Sprite
	int r, g, b, a; // Couleurs
	int rRange, gRange, bRange, aRange; // Couleurs portée
	int size; // Taille
	shape shape; // Forme

	struct Building *next; // Bâtiment suivant
} Building, *BuildingList;

const char* getBuildingTypeName(buildingType bt);
Building* allocBuilding(buildingType type, float x, float y);
void addBuilding(Building* b, BuildingList* list);
void freeBuilding(Building* b);
void freeBuildings(BuildingList* list);
void deleteBuilding(Building* b, BuildingList* list);
void drawBuildings(BuildingList list);
void drawBuilding(Building* b);
void drawRangeBuildings(BuildingList list);
void drawInfosBuilding(Building* b, char* infosConstructions);
Building* buildingSelected(BuildingList list, float x, float y);
Building* buildingIntersection(BuildingList buildings, float xNew, float yNew, int sizeNew, shape shapeNew);

#endif