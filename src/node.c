#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/map.h"
#include "../include/imageMap.h"
#include "../include/node.h"

int doesCircleIntersectsPath(float x, float y, int size, Node* nodes, float GL_VIEW_WIDTH, float GL_VIEW_HEIGHT) {
    Node* temp = nodes;
    float X = 10*(x+GL_VIEW_WIDTH)/(2*GL_VIEW_WIDTH)-0.5;
	float Y = 6*(-y+GL_VIEW_HEIGHT)/(2*GL_VIEW_HEIGHT)-0.5;
    while(temp != NULL) {
        if(X>temp->x-1 && X<temp->x+1) {
            if(Y>temp->y-1 && Y<temp->y+1) {
                return 1;
            }
            Node* linked = temp->linkedNodes;
            while(linked != NULL) {
                if((Y>linked->y && Y<temp->y) || (Y>temp->y && Y<linked->y)) {
                    return 1;
                }
                linked = linked->nextNode;
            }
        }
        else if(Y>temp->y-1 && Y<temp->y+1) {
            if(X>temp->x-1 && X<temp->x+1) {
                return 1;
            }
            Node* linked = temp->linkedNodes;
            while(linked != NULL) {
                if((X>linked->x && X<temp->x) || (X>temp->x && X<linked->x)) {
                    return 1;
                }
                linked = linked->nextNode;
            }
        }
        temp = temp->nextNode;
    }
    return 0;
}


int findStart(Node* nodes) {
    Node* temp = nodes;
    while(temp != NULL && temp->type != 1) {
    	temp = temp->nextNode;
    }
    return temp->id;
}

int findEnd(Node* nodes) {
    Node* temp = nodes;
    while(temp != NULL && temp->type != 2) {
    	temp = temp->nextNode;
    }
    return temp->id;
}

Node* idToNode(int id, Node* nodes) {
	Node* temp = nodes;
	while(temp != NULL && temp->id != id) {
		temp = temp->nextNode;
	}
	return temp;
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

void createLinkedNodeList(Node* nodes, Link* links) {
    Node* tempNode = nodes;
    Link* tempLink = links;
    while(tempNode != NULL) {
        while(tempLink != NULL && (tempLink->nodeId1 == tempNode->id)) {
            Node* temp = nodes;
            while((temp != NULL) && (temp->id != tempLink->nodeId2)) {
                temp = temp->nextNode;
            }
            Node* node = allocNode(temp->id, temp->type, temp->x, temp->y);
            addNode(node, &(tempNode->linkedNodes));
            tempLink=tempLink->nextLink;
        }
        tempNode = tempNode->nextNode;
    }
}

void testlinks(Node* nodes) {
    // Fonction temporaire pour tester
    Node* temp = nodes;
    while(temp != NULL) {
        printf("node : %d\n", temp->id);
        Node* link = temp->linkedNodes;
        while(link != NULL) {
            printf("%d\n", link->id);
            link = link->nextNode;
        }
        temp = temp->nextNode;
    }
}