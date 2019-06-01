#ifndef CONSTRUCTION_H__
#define CONSTRUCTION_H__

void giveBonusTowers(Building* b, TowerList* listTowers);
void giveBonusTower(Tower* t, BuildingList* listBuildings);
void removeBonusTowers(Building* b, TowerList* listTowers);
void removeBonusTower(Tower* t, BuildingList* listBuildings);
int constructionIntersection(BuildingList listBuildings, TowerList listTowers, float xNew, float yNew, int sizeNew, shape shapeNew);

#endif