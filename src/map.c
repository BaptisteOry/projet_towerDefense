#include <stdio.h>
#include <stdlib.h>

#include "../include/map.h"
#include "../include/ImageMap.h"

void itdCheck(char* itdFile, ImageMap* map, ItsEltsInfos* infos, Graph* graph) {

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
	if(comments[0] != #) {
		printf("Second line isn't comments.")
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
		nodeCount++;
		fscanf(file, "%d %d %d %d", &nodeId, &nodeType, &nodeX, &nodeY);
		nodes->id = nodeId;
		nodes->nodeType = nodeType;
		nodes->x = nodeX;
		nodes->y = nodeY;
		while(buffer[i] != EOL) {
			// Create links
			fscanf(file, "%d", nodeLink);
			links->id1 = nodes->id;
			links->id2 = nodeLink
			links = links->nextLink;
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

void mapCheck(ImageMap* map, ItsEltsInfos* infos, Graph* graph) {
	ItsEltsInfos* tempInfo = infos;
	Node* tempNode = graph->nodes;
	Link* tempLink = graph->links;
	int x = 0;
	int y = 0;
	while(y < map->height && x < map->width) {
		while(x< map->width) {

		}
	}
}


void testConstruct(int r, int g, int b, int x, int y, Graph* graph) {

}

void testPath(int r, int g, int b, int x, int y, Graph* graph) {

}

void testNode(int r, int g, int b, int x, int y, Graph* graph) {

}

void testIn(int r, int g, int b, int x, int y, Graph* graph) {

}

void testOut(int r, int g, int b, int x, int y, Graph* graph) {

}

/*void createGraph(Graph* graph) {

}*/