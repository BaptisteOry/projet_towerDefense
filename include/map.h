#ifndef MAP_H__
#define MAP_H__

#include "../include/imageMap.h"
#include "../include/operations.h"

typedef struct Node {
	int id;
	int x, y;
	int type;
	struct Node* nextNode;
	struct Node* linkedNodes;
} Node, *NodeList;

typedef struct Link {
	int nodeId1, nodeId2;
	struct Link* nextLink;
} Link, *LinkList;

typedef struct Graph {
	Node* nodes;
	Link* links;
} Graph;

typedef struct ItdEltsInfos {
	char keyword[10];
	int r, g, b;
	struct ItdEltsInfos* nextKeyword;
} ItdEltsInfos, *InfosList;


void itdCheck(char* itdFile, ImageMap* map, ItdEltsInfos** infos, Node** nodes, Link** links);

// Verifies if the .itd file corresponds to the map
void mapCheck(ImageMap* map, ItdEltsInfos* infos, Node* nodes);

void testNodeOnMap(int r, int g, int b, Node* node, ImageMap* map);

void testIfPath(int dataIndex, Node* nodes, ImageMap* map);

void testLectureItd(ItdEltsInfos* infos, Node* nodes, Link* links);

void printMapData(ImageMap* map);

ItdEltsInfos* allocInfo(char* keyword, int r, int g,int b);

void addInfo(ItdEltsInfos* info, InfosList* list);

Node* allocNode(int id, int type, int x, int y);

void addNode(Node* node, NodeList* list);

Link* allocLink(int id1, int id2);

void addLink(Link* link, LinkList* list);

#endif