#ifndef MAP_H__
#define MAP_H__

#include "../include/imageMap.h"

typedef struct Node {
	int id;
	int x, y;
	int type;
	struct Node* nextNode;	
} Node;

typedef struct Link {
	int nodeId1, nodeId2;
	struct Link* nextLink;
} Link;

typedef struct Graph {
	Node* nodes;
	struct Link* links;
} Graph;

typedef struct ItdEltsInfos {
	char keyword[10];
	int r, g, b;
	struct ItdEltsInfos* nextKeyword;
} ItdEltsInfos;


void itdCheck(char* itdFile, ImageMap* map, ItdEltsInfos* infos, Graph* graph);

// Verifies if the .itd file corresponds to the map
void mapCheck(ImageMap* map, ItdEltsInfos* infos, Graph* graph);

void testNodeOnMap(int r, int g, int b, Node* node, ImageMap* map);

#endif