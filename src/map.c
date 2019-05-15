#include <stdio.h>
#include <stdlib.h>

#include "../include/map.h"
#include "../include/imageMap.h"

void itdCheck(char* itdFile, ImageMap* map, ItdEltsInfos* infos, Graph* graph) {

	// ITD file elements
	char code[5];
	int version;

	char comments[50];

	char keyword[10];
	int valueR, valueG, valueB;

	char ppmFileName[10];

	int nodesNumber;
	int nodeId;
	int nodeType;
	int nodeX;
	int nodeY;
	int nodeLink;

	// Open .itd file
	FILE *file;

	file = fopen(itdFile, "r"); // read mode
 
	if (file == NULL) {
		printf("Error while opening .itd file.\n");
	  	return EXIT_FAILURE;
	}

	/* FILE CHECK */

	// Itd code check
	fscanf(file, "%s %d", code, &version);
	printf("%s\n", code);
	if(code != "@ITD") {
		printf("ERROR : Wrong .itd file format.\n");
		EXIT_FAILURE;
	}
	if(version != 1) {
		printf("Itd version not supported.\n");
		EXIT_FAILURE;
	}

	// Read comment
	fgets(comments, sizeof comments, file);
	if(comments[0] != "#") {
		printf("Second line isn't comments.");
		EXIT_FAILURE
	}
	fputs(comments, stdout);

	// Read & store variables & values
	while(fscanf(file, "%s", keyword) != NULL) {
		if(keyword=="carte") {
			fscanf(file, "%s", ppmFileName);
		}
		else if(keyword=="chemin" || keyword=="noeud" || keyword=="construct" || keyword=="in" || keyword=="out"){
			fscanf(file, "%d %d %d", &valueR, &valueG, &valueB);
			infos->keyword=keyword;
			if(testPixel(valueR)==0) {
				infos->r=valueR;
			}
			if(testPixel(valueG)==0) {
				infos->g=valueG;
			}
			if(testPixel(valueB)==0) {
				infos->b=valueB;
			}
			else if (!testPixel(valueR)==1 || !testPixel(valueG)==1 || !testPixel(valueB)==1) {
				printf("ERROR : Wrong pixel value(s).\n");
				EXIT_FAILURE;
			}
			infos=infos->nextKeyword;
		}
		else {
			printf("=ERROR : Keyword not valid.\n");
			EXIT_FAILURE;
		}
	}

	// Read & store nodes description
	fscanf(file, "%d", nodesNumber);
	int nodesCount = 0;
	Node* nodes = graph->nodes;
	Link* links = graph->links;

	char EOL ='\n';
	char buffer[20];

	while(!foef(file)) {
		// Create nodes
		nodesCount++;
		fscanf(file, "%d %d %d %d", &nodeId, &nodeType, &nodeX, &nodeY);
		nodes->id = nodeId;
		nodes->nodeType = nodeType;
		nodes->x = nodeX;
		nodes->y = nodeY;
		while(buffer[i] != EOL) {
			// Create links
			fscanf(file, "%d", nodeLink);
			links->id1 = nodes->id;
			links->id2 = nodeLink;
			links = links->nextLink;
			i++;
		}
		nodes=nodes->nextNode;

	}

	if(nodesCount != nodesNumber) {
		printf("ERROR : Number of nodes doesn't match nodeNumber.\n");
		EXIT_FAILURE;
	}


	/* PPM MAP CHECK */
	mapCheck(map, infos, graph);

 
 	// Close .itd file
	fclose(file);
}

void mapCheck(ImageMap* map, ItdEltsInfos* infos, Graph* graph) {
	ItdEltsInfos* tempInfo = infos;
	Node* tempNode = graph->nodes;
	Link* tempLink = graph->links;
	int x = 0;
	int y = 0;

	// Check all nodes
	while(tempNode != NULL) {
		if(tempNode->type = 1) {
			while(tempInfo->keyword != "in") {
				tempInfo=tempInfo->nextInfo;
			}
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
			tempInfo = infos;
		}
		else if(tempNode->type = 2) {
			while(tempInfo->keyword != "out") {
				tempInfo=tempInfo->nextInfo;
			}
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
			tempInfo = infos;
		}
		else if(tempNode->type = 3 || tempNode->type = 4) {
			while(tempInfo->keyword != "node") {
				tempInfo=tempInfo->nextInfo;
			}
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
			tempInfo = infos;
		}
	}

	// Check other pixels
	while(tempInfo->keyword != "construct") {
		tempInfo = tempInfo->nextInfo;
	}
	int r = tempInfo->r;
	int g = tempInfo->g;
	int b = tempInfo->b;

	for(int i = 0; i<(map->height)*(map->width); i=i+3) {
		int R = map->data[i];
		int G = map->data[i+1];
		int B = map->data[i+2];
		if(R != r || G != g || B != b) {
			printf("ERROR : ");
		}
	}

}


void testNodeOnMap(int r, int g, int b, Node* node, ImageMap* map) {
	int x = node->x;
	int y = node->y;
	if((map->data)[y*map->width + x*3] != r, (map->data)[y*map->width + x*3 +1] != g, (map->data)[y*map->width + x*3 +2] != b) {
		printf("ERROR : %d node not in wright place.\n", node->id);
		EXIT_FAILURE;
	}

}

/*void createGraph(Graph* graph) {

}*/