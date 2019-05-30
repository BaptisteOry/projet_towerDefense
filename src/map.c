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

void itdCheck(char* itdFile, ImageMap* map, ItdEltsInfos** infos, Node** nodes, Link** links) {

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
	//ItdEltsInfos* infosTemp = infos;
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
			//strcpy(infosTemp->keyword, keyword);
			/*if(testPixel(valueR)==0) {
				infosTemp->r=valueR;
			}
			if(testPixel(valueG)==0) {
				infosTemp->g=valueG;
			}
			if(testPixel(valueB)==0) {
				infosTemp->b=valueB;
			}*/
			if (testPixel(valueR)!=0 || testPixel(valueG)!=0 || testPixel(valueB)!=0) {
				printf("ERROR : Wrong pixel value(s).\n");
				EXIT_FAILURE;
			}
			ItdEltsInfos* infoTemp = allocInfo(keyword, valueR, valueG, valueB);
			printf("%d %d %d\n", infoTemp->r, infoTemp->g, infoTemp->b);
			addInfo(infoTemp, infos);
			/*infosTemp->nextKeyword = malloc(sizeof(infosTemp));
			infosTemp=infosTemp->nextKeyword;*/
		}
		else {
			printf("ERROR : Keyword not valid.\n");
			EXIT_FAILURE;
		}
		count++;
	}
	//infosTemp = NULL;

	// Read & store nodes description
	fscanf(file, "%d", &nodesNumber);
	printf("%d\n", nodesNumber);
	int nodesCount = 0;
	//graph->nodes = malloc(sizeof(Node));
	//if(graph->nodes == NULL) {
	//	printf("ERROR: bad allocation nodes in graph.\n");
	//	EXIT_FAILURE;
	//}
	//graph->links = malloc(sizeof(Link));
	//if(graph->links == NULL) {
	//	printf("ERROR : bad allocation links in graph.\n");
	//}
	//Node* nodes = graph->nodes;
	//Link* links = graph->links;

	fgets(buf, sizeof(buf), file);
	while(fgets(buf, sizeof(buf), file) != NULL) {
	    buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores

	    //printf("%s\n", buf); // buf[i] is a character, and counts spaces
	    sscanf(buf, "%d %d %d %d", &nodeId, &nodeType, &nodeX, &nodeY);
	    printf("%d %d %d %d ", nodeId, nodeType, nodeX, nodeY);
	    //sscanf(buf, "%d %d %d %d", &nodes->id, &nodes->type, &nodes->x, &nodes->y);
	    //printf("%d %d %d %d ", nodes->id, nodes->type, nodes->x, nodes->y);
	    char slicedBuf[strlen(buf)+1];
	    slice_str(buf, slicedBuf, 7, strlen(buf)-1);
	    int i=2;
	    //printf("%s\n", slicedBuf);
	    while(strlen(slicedBuf)>1) {
	    	sscanf(slicedBuf, "%d", &nodeLink);
	    	printf("%d ", nodeLink);
	    	slice_str(buf, slicedBuf, 7+i, strlen(buf)-1);
	    	i=i+2;
	    	Link* linkTemp = allocLink(nodeId, nodeLink);
	    	addLink(linkTemp, links);
	    	//links->nodeId1 = nodes->id;
			//links->nodeId2 = nodeLink;
			//links->nextLink = malloc(sizeof(Link));
			//links = links->nextLink;
	    }
	    printf("\n");
	    //nodes->id = nodeId;
	    //nodes->type = nodeType;
	    //nodes->x = nodeX;
	    //nodes->y = nodeY;
	    Node* nodeTemp = allocNode(nodeId, nodeType, nodeX, nodeY);
	    addNode(nodeTemp, nodes);
	    //nodes->nextNode = malloc(sizeof(Node));
	    //nodes = nodes->nextNode;
	    nodesCount++;
	}
	//links = NULL;
	//nodes = NULL;
	printf("%d\n", nodesCount);

	if(nodesCount != nodesNumber) {
		printf("ERROR : Number of nodes doesn't match nodeNumber.\n");
		EXIT_FAILURE;
	}

	/* PPM MAP CHECK */
	//mapCheck(map, infos, nodes);
 
 	// Close .itd file
	fclose(file);
}

void mapCheck(ImageMap* map, ItdEltsInfos* infos, Node* nodes) {
	Node* tempNode = nodes;

	// Check all nodes
	while(tempNode != NULL) {
		ItdEltsInfos* tempInfo = infos;
		if(tempNode->type == 1) {
			while(strcmp(tempInfo->keyword, "in")!=0) {
				tempInfo=tempInfo->nextKeyword;
			}
			printf("%s %d %d %d\n", tempInfo->keyword, tempInfo->r, tempInfo->g, tempInfo->b);
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
		}
		else if(tempNode->type == 2) {
			while(strcmp(tempInfo->keyword, "out")!=0) {
				tempInfo=tempInfo->nextKeyword;
			}
			printf("%s %d %d %d\n", tempInfo->keyword, tempInfo->r, tempInfo->g, tempInfo->b);
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
		}
		else if(tempNode->type == 3 || tempNode->type == 4) {
			while(strcmp(tempInfo->keyword, "noeud")!=0) {
				tempInfo=tempInfo->nextKeyword;
			}
			printf("%s %d %d %d\n", tempInfo->keyword, tempInfo->r, tempInfo->g, tempInfo->b);
			testNodeOnMap(tempInfo->r, tempInfo->g, tempInfo->b, tempNode, map);
		}
		else {
			printf("Error in node, impossible to test color.\n");
			EXIT_FAILURE;
		}
		tempNode=tempNode->nextNode;
	}

	// Check other pixels
	ItdEltsInfos* tempInfo = infos;
	while(strcmp(tempInfo->keyword, "construct")!=0) {
		tempInfo = tempInfo->nextKeyword;
	}
	int r = tempInfo->r;
	int g = tempInfo->g;
	int b = tempInfo->b;
	printf("test construct %d %d %d\n", r, g, b);
	tempInfo = infos;

	while(strcmp(tempInfo->keyword, "chemin")!=0) {
		tempInfo=tempInfo->nextKeyword;
	}
	int pathR = tempInfo->r;
	int pathG = tempInfo->g;
	int pathB = tempInfo->b;
	printf("test chemmin %d %d %d\n", pathR, pathG, pathB);

	for(int i = 0; i<(map->height)*(map->width); i=i+3) {
		int R = map->data[i];
		int G = map->data[i+1];
		int B = map->data[i+2];
		if(R != r || G != g || B != b) {
			if((R == pathR) && (G == pathG) && (B == pathB)) {
				testIfPath(i, nodes, map);
			}
			else {
				//tester si noeud (= si le x et le y correspondent a un noeud dans la liste) sinon erreur
				printf("ERROR : Pixel color doesn't match anything.\n");
				printf("%d : %d, %d : %d, %d : %d\n", i, R, i+1, G, i+2, B);
			}
		}
		else {
			printf("This pixel is on a construction zone, where it should be <3.\n");
		}
	}

}


void testNodeOnMap(int r, int g, int b, Node* node, ImageMap* map) {
	int x = node->x;
	int y = node->y;
	//printf("%d, %d\n", x, y);
	//printf("%d\n", y*map->width*3 + x*3);
	printf("%d %d %d\n", (map->data)[y*map->width*3 + x*3], (map->data)[y*map->width*3 + x*3+1], (map->data)[y*map->width*3 + x*3+2]);
	if((map->data)[y*map->width*3 + x*3] != r || (map->data)[y*map->width*3 + x*3 +1] != g || (map->data)[y*map->width*3 + x*3 +2] != b) {
		printf("ERROR : %d node not in right place.\n", node->id);
		EXIT_FAILURE;
	}
	printf("Pixel in right place.\n");
}

void testIfPath(int dataIndex, Node* nodes, ImageMap* map) {
	// Check if dataIndex is between two nodes
	int x = dataIndex%map->width;
	int y = dataIndex/map->width;
	Node* tempNode = nodes;
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

void testLectureItd(ItdEltsInfos* infos, Node* nodes, Link* links) {
	ItdEltsInfos* infosTemp = infos;
	Node* nodesTemp = nodes;
	Link* linksTemp = links;
	while(infosTemp != NULL) {
		printf("%s %d %d %d\n", infosTemp->keyword, infosTemp->r, infosTemp->g, infosTemp->b);
		infosTemp = infosTemp->nextKeyword;
	}
	while(nodesTemp != NULL) {
		printf("%d %d %d %d\n", nodesTemp->id, nodesTemp->type, nodesTemp->x, nodesTemp->y);
		nodesTemp = nodesTemp->nextNode;
	}
	while(linksTemp !=NULL) {
		printf("%d %d\n", linksTemp->nodeId1, linksTemp->nodeId2);
		linksTemp = linksTemp->nextLink;
	}
}

void printMapData(ImageMap* map) {
	printf("Map data:\n");
	for(int i=0; i<map->width*map->height*3; i++) {
		printf("%d : %d\n", i, map->data[i]);
	}
	printf("\n");
}

ItdEltsInfos* allocInfo(char* keyword, int r, int g,int b) {
	ItdEltsInfos* info = (ItdEltsInfos*) malloc(sizeof(ItdEltsInfos));
	if(info == NULL) {
		printf("allocInfos : erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	strcpy(info->keyword, keyword);
	info->r = r;
	info->g = g;
	info->b = b;
	info->nextKeyword = NULL;
	return info;
}

void addInfo(ItdEltsInfos* info, InfosList* list){
    if(*list == NULL){
        *list = info;
    }else{
        ItdEltsInfos *temp = *list;
        while(temp->nextKeyword != NULL){
            temp = temp->nextKeyword;
        }
        temp->nextKeyword = info;
    }
}

Node* allocNode(int id, int type, int x, int y) {
	Node* node = (Node*) malloc(sizeof(Node));
	if(node == NULL) {
		printf("allocNode : erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	node->id = id;
	node->type = type;
	node->x = x;
	node->y = y;
	node->nextNode = NULL;
	node->linkedNodes = NULL;
	return node;
}

void addNode(Node* node, NodeList* list){
    if(*list == NULL){
        *list = node;
    }else{
        Node *temp = *list;
        while(temp->nextNode != NULL){
            temp = temp->nextNode;
        }
        temp->nextNode = node;
    }
}

Link* allocLink(int id1, int id2) {
	Link* link = (Link*) malloc(sizeof(Link));
	if(link == NULL) {
		printf("allocLink : erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	link->nodeId1 = id1;
	link->nodeId2 = id2;
	link->nextLink = NULL;
	return link;
}

void addLink(Link* link, LinkList* list){
    if(*list == NULL){
        *list = link;
    }else{
        Link *temp = *list;
        while(temp->nextLink != NULL){
            temp = temp->nextLink;
        }
        temp->nextLink = link;
    }
}