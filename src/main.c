#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "../include/tower.h"
#include "../include/building.h"
#include "../include/object.h"
#include "../include/controller.h"
#include "../include/imageMap.h"
#include "../include/map.h"

// Dimensions initiales et titre de la fenetre
static const unsigned int WINDOW_WIDTH = 1000;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "Tower defense";
// Espace fenetre virtuelle
static const float GL_VIEW_WIDTH = 250.;
static const float GL_VIEW_HEIGHT = 150.;
// Nombre de bits par pixel de la fenetre
static const unsigned int BIT_PER_PIXEL = 32;
// Nombre minimal de millisecondes separant le rendu de deux images
static const Uint32 FRAMERATE_MILLISECONDS = 100;

// Initialisation des valeurs principales
static ImageMap imageMap;
static char imageMap01[255]="images/map01";

static TowerList towers;
static BuildingList buildings;

static towerType towerToBuild = -1;
static buildingType buildingToBuild = -1;

// Creation d'un contexte OpenGL et redimensionnement
void reshape(SDL_Surface** surface, unsigned int width, unsigned int height){
    SDL_Surface* surface_temp = SDL_SetVideoMode(
        width, height, BIT_PER_PIXEL,
        SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
    if(NULL == surface_temp){
        fprintf(
            stderr, 
            "Erreur lors du redimensionnement de la fenetre.\n");
        exit(EXIT_FAILURE);
    }
    *surface = surface_temp;

    glViewport(0, 0, (*surface)->w, (*surface)->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-GL_VIEW_WIDTH, GL_VIEW_WIDTH, -GL_VIEW_HEIGHT, GL_VIEW_HEIGHT);
}

int main(int argc, char** argv){
    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)){
        fprintf(
            stderr, 
            "Impossible d'initialiser la SDL. Fin du programme.\n");
        exit(EXIT_FAILURE);
    }
    // Ouverture d'une fenetre et creation d'un contexte OpenGL
    SDL_Surface* surface;
    reshape(&surface, WINDOW_WIDTH, WINDOW_HEIGHT);
    // Initialisation du titre de la fenetre
    SDL_WM_SetCaption(WINDOW_TITLE, NULL);
    // Initialisation paramètres textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // Initialisation matrice de projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Charger image ppm (placer dans l'image carte)
    if(loadImageMapPPM(&imageMap, imageMap01) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }
    /* Pour le temps 
    time_t rawtime;
    struct tm* timeinfo;*/

    /* Boucle principale */
    int loop = 1;
    while(loop){
        // Recuperation du temps au debut de la boucle
        Uint32 startTime = SDL_GetTicks();
        // Nettoyage de la fenetre
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1);
        /* Desactivage du double buffering */
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
        
        /* Code de dessin */
        drawImageMap(&imageMap, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
        drawTowers(towers);
        drawBuildings(buildings);
        drawRangeTowers(towers);
        drawRangeBuildings(buildings);

        /* Récupération de l'heure
        time(&rawtime);
        timeinfo = localtime(&rawtime);*/

        // Echange du front et du back buffer : mise a jour de la fenetre
        SDL_GL_SwapBuffers();
        
        /* Boucle traitant les evenements */
        float x,y;
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            // L'utilisateur ferme la fenetre
            if(e.type == SDL_QUIT){
                loop = 0;
                break;
            }
            if(e.type == SDL_KEYDOWN 
                && (e.key.keysym.sym == SDLK_ESCAPE)){
                loop = 0; 
                break;
            }
            
            switch(e.type){
                // Redimensionnement fenetre
                case SDL_VIDEORESIZE:
                    reshape(&surface, e.resize.w, e.resize.h);
                    break;

                // Clic souris
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    x = -GL_VIEW_WIDTH + GL_VIEW_WIDTH*2 * (e.button.x) / WINDOW_WIDTH;
                    y = -(-GL_VIEW_HEIGHT + GL_VIEW_HEIGHT*2 * (e.button.y) / WINDOW_HEIGHT);
                    switch(e.button.button){
                        case SDL_BUTTON_LEFT:
                            if(towerToBuild != -1){
                                int sizeNew = (GL_VIEW_WIDTH*100)/WINDOW_WIDTH;
                                if(!(isIntersection(towers, buildings, x, y, sizeNew))){
                                    addTower(allocTower(towerToBuild, x, y, sizeNew), &towers);
                                }
                            }else if(buildingToBuild != -1){
                                int sizeNew = (GL_VIEW_WIDTH*100)/WINDOW_WIDTH;
                                if(!(isIntersection(towers, buildings, x, y, sizeNew))){
                                    addBuilding(allocBuilding(buildingToBuild, x, y, sizeNew), &buildings);
                                }
                            }
                            break;

                        case SDL_BUTTON_RIGHT:
                            deleteTower(towerSelected(towers, x, y), &towers);
                            deleteBuilding(buildingSelected(buildings, x, y), &buildings);
                            break;

                        default:
                            break;
                    }
                    break;
                
                // Touche clavier
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    // Sélection des tours
                    if(e.key.keysym.sym == SDLK_a){
                        towerToBuild = TRED;
                    }
                    if(e.key.keysym.sym == SDLK_z){
                        towerToBuild = TGREEN;
                    }
                    if(e.key.keysym.sym == SDLK_e){
                        towerToBuild = TYELLOW;
                    }
                    if(e.key.keysym.sym == SDLK_r){
                        towerToBuild = TBLUE;
                    }
                    // Sélection des bâtiments
                    if(e.key.keysym.sym == SDLK_q){
                        buildingToBuild = RADAR;
                    }
                    if(e.key.keysym.sym == SDLK_s){
                        buildingToBuild = FACTORY;
                    }
                    if(e.key.keysym.sym == SDLK_d){
                        buildingToBuild = STOCK;
                    }
                    break;

                case SDL_KEYUP:
                    // Désélection des tours
                    if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_r) {
                        towerToBuild = -1;
                    }
                    // Désélection des bâtiments
                    if(e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_d) {
                        buildingToBuild = -1;
                    }
                    break;
                    
                default:
                    break;
            }
        }

        // Calcul du temps ecoule
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        // Si trop peu de temps s'est ecoule, on met en pause le programme
        if(elapsedTime < FRAMERATE_MILLISECONDS){
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    // Liberation des ressources associees a la SDL
    SDL_Quit();
    
    return EXIT_SUCCESS;
}