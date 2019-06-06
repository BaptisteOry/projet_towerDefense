#ifndef MAP_H__
#define MAP_H__

#include "../include/imageMap.h"
#include "../include/operations.h"
#include "../include/node.h"

typedef struct ItdEltsInfos {
	char keyword[10];
	int r, g, b;
	struct ItdEltsInfos* nextKeyword;
} ItdEltsInfos, *InfosList;


void itdCheck(char* itdFile, ImageMap* map, ItdEltsInfos** infos, Node** nodes, Link** links, int* nbOfNodes);

// Verifies if the .itd file corresponds to the map
void mapCheck(ImageMap* map, ItdEltsInfos* infos, Node* nodes);

void testNodeOnMap(int r, int g, int b, Node* node, ImageMap* map);

int testIfPath(int dataIndex, Node* nodes, ImageMap* map);

int testIfNode(int dataIndex, Node* nodes, ImageMap* map);

void testLectureItd(ItdEltsInfos* infos, Node* nodes, Link* links);

void printMapData(ImageMap* map);

ItdEltsInfos* allocInfo(char* keyword, int r, int g,int b);

void addInfo(ItdEltsInfos* info, InfosList* list);

Link* allocLink(int id1, int id2);

void addLink(Link* link, LinkList* list);

void createLinkedNodeList(Node* nodes, Link* links);

void testlinks(Node* nodes);



#endif