#ifndef MAPVERIF_H__
#define MAPVERIF_H__

typedef struct Node {
	int id;
	int x, y;
	int type;
	Node* nextNode;	
} Node;

typedef struct Link {
	int nodeId1, nodeId2;
	Link* nextLink;
} Link;

typedef struct Graph {
	Node* nodes;
	Link* links;
};

typedef struct ItdEltsInfos {
	char keyword[10];
	int r, g, b;
} ItdEltsInfos;

// Verifies if the .idt file is correct
void itdCheck(char* itdFile);

// Verifies if the .itd file corresponds to the map
void mapCheck(ImageMap map, char* itdFile);

#endif