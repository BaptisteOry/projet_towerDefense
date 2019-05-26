#ifndef CONSTRUCTION_H__
#define CONSTRUCTION_H__

void giveBonusTowers(Building* b, TowerList* listTowers);
void removeBonusTowers(Building* b, TowerList* listTowers);
int constructionIntersection(BuildingList listBuildings, TowerList listTowers, float xNew, float yNew, int sizeNew, shape shapeNew);
void drawInfosConstructions(char* infosConstructions);

#endif