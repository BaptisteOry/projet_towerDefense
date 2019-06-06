#ifndef NODE_H__
#define NODE_H__

#include "../include/imageMap.h"

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

int doesCircleIntersectsPath(float x, float y, int size, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT);
int findStart(Node* nodes);
int findEnd(Node* nodes);
Node* idToNode(int id, Node* nodes);

Node* allocNode(int id, int type, int x, int y);

void addNode(Node* node, NodeList* list);

#endif