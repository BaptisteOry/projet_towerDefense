#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <math.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/operations.h"
#include "../include/node.h"

#define MAX_NODES 10


void slice_str(const char * str, char * buffer, int start, int end) {
    int j = 0;
    for (int i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

int isCircleIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2){
    if( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1) < (size1 + size2)*(size1 + size2)){
        return 1;
    }else{
        return 0;
    }
}

int isSquareIntersectsCircle(float x1, float y1, float x2, float y2, int size1, int size2){
    if(    x1 + (size1 + size2) >= x2
        && x1 - (size1 + size2) <= x2
        && y1 + (size1 + size2) >= y2
        && y1 - (size1 + size2) <= y2){
        return 1;
    }else{
        return 0;
    }
}

int isSquareIntersectsSquare(float x1, float y1, float x2, float y2, int size1, int size2){
    if(    x1 + (size1 + size2) >= x2
        && x1 - (size1 + size2) <= x2
        && y1 + (size1 + size2) >= y2
        && y1 - (size1 + size2) <= y2){
        return 1;
    }else{
        return 0;
    }
}

int isSquareIntersectsRectangle(float x1, float y1, float x2, float y2, int size1, int size2w, int size2h){
    if(    x1 + (size2w + size1) >= x2
        && x1 - (size2w + size1) <= x2
        && y1 + (size2h + size1) >= y2
        && y1 - (size2h + size1) <= y2){
        return 1;
    }else{
        return 0;
    }
}

float convertWindowGLViewWidth(float x, unsigned int WINDOW_WIDTH, float GL_VIEW_WIDTH){
    return (-GL_VIEW_WIDTH + GL_VIEW_WIDTH*2 * x / WINDOW_WIDTH);
}

float convertWindowGLViewHeight(float y, unsigned int WINDOW_HEIGHT, float GL_VIEW_HEIGHT){
    return -(-GL_VIEW_HEIGHT + GL_VIEW_HEIGHT*2 * y / WINDOW_HEIGHT);
}

int randomRange(int min, int max){
    return rand()%(max-min+1) + min;
}

Node* dijkstra(Node* nodes) {
    Node* temp = nodes;
    int start = findStart(nodes);
    int end = findEnd(nodes);

    int distances[MAX_NODES][2]; // Pour chaque noeud donne la distance + 1 s'il est marqué, 0 sinon
    int previous[MAX_NODES];

    // Initialisation distances & previous
    int nbOfNodes = 0;
    while(temp != NULL) {
        distances[temp->id][0] = -1;
        distances[temp->id][1] = 0;
        previous[temp->id] = -1;
        temp = temp->nextNode;
        nbOfNodes++;
    }
    //printf("Tu peux le faire <3\n");
    distances[start][0] = 0;

    temp = nodes;
    int s = start;
    while(s != end) {
        distances[s][1] = 1;
        while(temp->id != s) {
            temp = temp->nextNode;
        }
        Node * link = temp->linkedNodes;
        while(link != NULL) {
            if(distances[link->id][1] == 0) {
                int dist = distance(temp, link);
                if((distances[s][0] + dist < distances[link->id][0]) || (distances[link->id][0] == -1)) {
                    distances[link->id][0] = distances[s][0] + dist;
                    previous[link->id] = s;
                }
            }
            link = link->nextNode;
        }
        int minDist = INT_MAX;
        int minDistIndex = s;
        for(int i=0; i<nbOfNodes; i++) {
            if((minDist >= distances[i][0]) && (distances[i][0] > distances[s][0])) {
                minDist = distances[i][0];
                minDistIndex = i;
            }
        }
        s = minDistIndex;
        temp = nodes;
    }

    // Recréer le chemin
    Node* path = idToNode(end, nodes);
    int index = end;

    while(index != start) {
        index = previous[index];
        Node* node = idToNode(index, nodes);
        Node* toAdd = allocNode(node->id, node->type, node->x, node->y);
        addNode(toAdd, &path);
    }
    reverse(&path);
    return path;
}

int distance(Node* node1, Node* node2) {
    int dist = (node1->x-node2->x)*(node1->x-node2->x) + (node1->y-node2->y)*(node1->y-node2->y);
    return sqrt(abs(dist));
}

void reverse (NodeList* n)
{
   Node* inv = *n;
   Node* tete;
   if (inv && inv->nextNode)       // Sinon il n'y a rien à faire
   {
      tete = inv->nextNode;        // On mémorise la tete de sous liste
      reverse (&(inv->nextNode));  // On inverse la sous-liste
      tete->nextNode = inv;        // La tête actuelle de la liste passe en queue
      *n = inv->nextNode;          // La tête de la sous liste inversée devient la nouvelle tête de liste
      tete->nextNode->nextNode = NULL; // Important, sinon la LSC devient circulaire
   }
}

Node* randomPath(Node* nodes) {
    Node* temp = nodes;
    Node* path = NULL;
    while(temp != NULL && temp->type != 1) {
        temp = temp->nextNode;
    }
    Node* previous = temp;
    Node* toAdd = allocNode(temp->id, temp->type, temp->x, temp->y);
    addNode(toAdd, &path);
    while(temp->type != 2) {
        if(temp->type != 1) {
            Node* list = nodes;
            while(temp->id != list->id) {
                list = list->nextNode;
            }
            temp = list;
        }
        Node* links = temp->linkedNodes;
        int nbLinks = 0;
        int end = -1;
        while(links != NULL) {
            nbLinks++;
            if(links->type == 2) {
                end = links->id;
            }
            links = links->nextNode;
        }
        links = temp->linkedNodes;
        if(end == -1) {
            for (int i = 0; i < rand()%nbLinks; ++i) {
                links = links->nextNode;
            }
        }
        else {
            while(links->id != end) {
                links = links->nextNode;
            }
        }
        if(links->type != 1 && links->id != previous->id) {
            toAdd = allocNode(links->id, links->type, links->x, links->y);
            addNode(toAdd, &path);
            previous = temp;
            temp = toAdd;
        }
    }
    return path;
}