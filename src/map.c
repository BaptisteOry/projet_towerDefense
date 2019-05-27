#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/map.h"
#include "../include/imageMap.h"
#include "../include/operations.h"

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

	char buf[100]; 

	// Open .itd file
	FILE *file = NULL;

	char fileNameItd[255]= "";
	strcat(strcpy(fileNameItd, itdFile), ".itd");

	file = fopen(itdFile, "r"); // read mode
 
	if(file == NULL) {
		printf("Error while opening .itd file.\n");
	  	EXIT_FAILURE;
	}

	/* FILE CHECK */

	// Itd code check
	fscanf(file, "%s %d", code, &version);
	printf("%s %d\n", code, version);
	if(strcmp(code, "@ITD") != 0) {
		printf("ERROR : Wrong .itd file format.\n");
		EXIT_FAILURE;
	}
	if(version != 1) {
		printf("Itd version not supported.\n");
		EXIT_FAILURE;
	}

	// Read comment
	fscanf(file, "%s", comments);
	printf("%s\n", comments);
	if(comments[0] != 35) {
		printf("Second line isn't comments.\n");
		EXIT_FAILURE;
	}

	// Read & store variables & values
	int count = 0;
	while(count<6) {
		fscanf(file, "%s", keyword);
		printf("%s ", keyword);
		if(strcmp(keyword, "carte")==0) {
			fscanf(file, "%s", ppmFileName);
			printf("%s\n", ppmFileName);
		}
		else if(strcmp(keyword, "chemin")==0 || strcmp(keyword, "noeud")==0 || strcmp(keyword, "construct")==0 || strcmp(keyword, "in")==0 || strcmp(keyword, "out")==0){
			fscanf(file, "%d %d %d", &valueR, &valueG, &valueB);
			*(infos->keyword)=*keyword;
			if(testPixel(valueR)==0) {
				infos->r=valueR;
			}
			if(testPixel(valueG)==0) {
				infos->g=valueG;
			}
			if(testPixel(valueB)==0) {
				infos->b=valueB;
			}
			else if (testPixel(valueR)!=1 || testPixel(valueG)!=1 || testPixel(valueB)!=1) {
				printf("ERROR : Wrong pixel value(s).\n");
				EXIT_FAILURE;
			}
			printf("%d %d %d\n", infos->r, infos->g, infos->b);
			infos->nextKeyword = malloc(sizeof(ItdEltsInfos));
			infos=infos->nextKeyword;
		}
		else {
			printf("ERROR : Keyword not valid.\n");
			EXIT_FAILURE;
		}
		count++;
	}

	// Read & store nodes description
	fscanf(file, "%d", &nodesNumber);
	printf("%d\n", nodesNumber);
	int nodesCount = 0;
	Node* nodes = graph->nodes;
	Link* links = graph->links;


	while (fgets(buf, sizeof(buf), file) != NULL && nodesCount < nodesNumber) {
	    buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
	    int nbElts = strlen(buf)/2 +1;

	    //printf("%s\n", buf); // buf[i] is a character, and counts spaces
	    sscanf(buf, "%d %d %d %d", &nodeId, &nodeType, &nodeX, &nodeY);
	    printf("%d %d %d %d ", nodeId, nodeType, nodeX, nodeY);
	    char* slicedBuf[strlen(buf)+1];
	    slice_str(buf, slicedBuf, 7, strlen(buf)-1);
	    int i=2;
	    //printf("%s\n", slicedBuf);
	    while(strlen(slicedBuf)>1) {
	    	sscanf(slicedBuf, "%d", &nodeLink);
	    	printf("%d ", nodeLink);
	    	slice_str(buf, slicedBuf, 7+i, strlen(buf)-1);
	    	i=i+2;
	    	links->nodeId1 = nodes->id;
			links->nodeId2 = nodeLink;
			links->nextLink = malloc(sizeof(Link));
			links = links->nextLink;
	    }
	    printf("\n");
	    nodes->nextNode = malloc(sizeof(Node));
	    nodes = nodes->nextNode;
	    nodesCount++;
	}
	printf("%d\n", nodesCount);

	if(nodesCount != nodesNumber) {
		printf("ERROR : Number of nodes doesn't match nodeNumber.\n");
		EXIT_FAILURE;
	}

	/* PPM MAP CHECK */
	//mapCheck(map, infos, graph);

 
 	// Close .itd file
	fclose(file);
	EXIT_SUCCESS;
}

void mapCheck(ImageMap* map, ItdEltsInfos* infos, Graph* graph) {
	ItdEltsInfos* tempInfo = infos;
	Node* tempNode = graph->nodes;

	// Check all nodes
	while(tempNode != NULL) {
		if(tempNode->type == 1) {
			while(strcmp(tempInfo->keyword, "in")!=0) {
				tempInfo=tempInfo->nextKeyword;
			}
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
			tempInfo = infos;
		}
		else if(tempNode->type == 2) {
			while(strcmp(tempInfo->keyword, "out")!=0) {
				tempInfo=tempInfo->nextKeyword;
			}
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
			tempInfo = infos;
		}
		else if(tempNode->type == 3 || tempNode->type == 4) {
			while(strcmp(tempInfo->keyword, "node")!=0) {
				tempInfo=tempInfo->nextKeyword;
			}
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
			tempInfo = infos;
		}
	}

	// Check other pixels
	while(strcmp(tempInfo->keyword, "construct")!=0) {
		tempInfo = tempInfo->nextKeyword;
	}
	tempInfo = infos;
	int r = tempInfo->r;
	int g = tempInfo->g;
	int b = tempInfo->b;

	while(strcmp(tempInfo->keyword, "chemin")!=0) {
		tempInfo=tempInfo->nextKeyword;
	}
	int pathR = tempInfo->r;
	int pathG = tempInfo->g;
	int pathB = tempInfo->b;

	for(int i = 0; i<(map->height)*(map->width); i=i+3) {
		int R = map->data[i];
		int G = map->data[i+1];
		int B = map->data[i+2];
		if(R != r || G != g || B != b) {
			if((R == pathR) && (G == pathG) && (B == pathB)) {
				testIfPath(i, graph, map);
			}
			else {
				//tester si noeud (= si le x et le y correspondent a un noeud dans la liste) sinon erreur
				printf("ERROR : Pixel color doesn't match anything.");
			}
		}
	}

}


void testNodeOnMap(int r, int g, int b, Node* node, ImageMap* map) {
	int x = node->x;
	int y = node->y;
	if((map->data)[y*map->width + x*3] != r || (map->data)[y*map->width + x*3 +1] != g || (map->data)[y*map->width + x*3 +2] != b) {
		printf("ERROR : %d node not in wright place.\n", node->id);
		EXIT_FAILURE;
	}

}

void testIfPath(int dataIndex, Graph* graph, ImageMap* map) {
	// Check if dataIndex is between two nodes
	int x = dataIndex%map->width;
	int y = dataIndex/map->width;
	Node* tempNode = graph->nodes;
	while(tempNode != NULL) {
		if(tempNode->x == x) {
			Node* linkedNode = tempNode->linkedNodes;
			while(linkedNode != NULL) {
				if(linkedNode->x == x) {
					if((tempNode->y < y && linkedNode->y > y) || (tempNode->y > y && linkedNode->y < y)) {
						printf("This pixel in a on a path.");
						EXIT_SUCCESS;
					}
				}
				linkedNode = linkedNode->nextNode;	
			}
		}
		if(tempNode->y == y) {
			Node* linkedNode = tempNode->linkedNodes;
			while(linkedNode != NULL) {
				if(linkedNode->y == y) {
					if((tempNode->x < x && linkedNode->x > x) || (tempNode->x > x && linkedNode->x < x)) {
						printf("This pixel in a on a path.\n");
						EXIT_SUCCESS;
					}
				}
				linkedNode = linkedNode->nextNode;	
			}
		}		
		tempNode = tempNode->nextNode;
	}
	printf("This pixel is not on a path but should be.\n");
	EXIT_FAILURE;
}

/*void createGraph(Graph* graph) {

}*/