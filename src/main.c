#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h> 
#include <math.h>
#include <time.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/display.h"
#include "../include/operations.h"
#include "../include/monster.h"
#include "../include/building.h"
#include "../include/tower.h"
#include "../include/construction.h"
#include "../include/game.h"
#include "../include/interface.h"
#include "../include/imageMap.h"
#include "../include/map.h"

// Dimensions initiales et titre de la fenetre
static unsigned int WINDOW_WIDTH = 1000;
static unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "Flower Tower";
// Espace fenetre virtuelle
static const float GL_VIEW_WIDTH = 250.;
static const float GL_VIEW_HEIGHT = 150.;
// Nombre de bits par pixel de la fenetre
static const unsigned int BIT_PER_PIXEL = 32;
// Nombre minimal de millisecondes separant le rendu de deux images
static const Uint32 FRAMERATE_MILLISECONDS = 100;

// Creation d'un contexte OpenGL et redimensionnement
void reshape(SDL_Surface** surface, unsigned int width, unsigned int height){
    SDL_Surface* surface_temp = SDL_SetVideoMode(
        width, height, BIT_PER_PIXEL,
        SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
    if(NULL == surface_temp){
        printf("Erreur lors du redimensionnement de la fenetre.\n");
        exit(EXIT_FAILURE);
    }
    *surface = surface_temp;

    WINDOW_WIDTH = (*surface)->w;
    WINDOW_HEIGHT = (*surface)->h;
    glViewport(0, 0, (*surface)->w, (*surface)->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-GL_VIEW_WIDTH, GL_VIEW_WIDTH, -GL_VIEW_HEIGHT, GL_VIEW_HEIGHT);
}

int main(int argc, char** argv){
    /* Initialisations */
    // Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        printf("Impossible d'initialiser la SDL. Fin du programme.\n");
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
    //Initialisation Glut
    glutInit(&argc, argv);
    // Initialisation des valeurs principales
    static ImageMap imageMap;
    static char imageMap01[255]="images/map01";
    static Game* game;
    static Interface* interface;
    static MonsterList monsters;
    static TowerList towers;
    static BuildingList buildings;
    towerType towerToBuild = -1;
    buildingType buildingToBuild = -1;
    int help = -1;
    int counter = 0;
    srand(time(NULL));

    // Charger image ppm (placer dans l'image carte)
    loadImageMapPPM(&imageMap, imageMap01);
    // Vérification carte et itd
    char* itdFile = "data/map01.itd";
    ItdEltsInfos* infos = NULL;
    Node* nodes = NULL;
    Link* links = NULL;
    int nbOfNodes = 0;
    //printMapData(&imageMap);
    itdCheck(itdFile, &imageMap, &infos, &nodes, &links, &nbOfNodes);

    
    createLinkedNodeList(nodes, links);

    printf("test liens\n");
    testlinks(nodes);

    // Vérification du ppm
    mapCheck(&imageMap, infos, nodes);

    // Affiche les informations enregistrées (à titre de test)
    printf("Test test\n");
    //testLectureItd(infos, nodes, links);

    // Créer un nouveau jeu
    // Créer un nouveau jeu et l'interface
    interface = allocInterface(GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
    game = allocGame();

    /* Boucle principale */
    int loop = 1;
    while(loop){
        /* Initialisation loop */
        // Récupération du temps au debut de la boucle
        Uint32 startTime = SDL_GetTicks();

        // Nettoyage de la fenetre
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1);
        // Désactivage du double buffering
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
        
        /* Code de dessin */
        // Objets (constructions et monstres)
        drawImageMap(&imageMap, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
        drawMonsters(monsters);
        drawRangeTowers(towers);
        drawRangeBuildings(buildings);
        drawTowers(towers);
        drawBuildings(buildings);
        drawInfosConstructions(interface);
        drawGameElements(interface, game);
        if(help == 1){
            drawHelp(interface);
        }
        if((game->end) == 1){
            drawEnd(interface);
        }

        // Échange du front et du back buffer : mise à jour de la fenêtre
        SDL_GL_SwapBuffers();

        /* Évènements jeu */
        // Création de vagues de monstres
        addWave(game, &monsters, counter, nodes, nbOfNodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
        // Déplacement de la vague de monstre;
        moveMonsters(&monsters, nodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
        // Destruction monstres
        killMonsters(&monsters, &towers, counter);
        
        /* Évènements joueur */
        float x,y;
        Tower* tempT; Building* tempB;
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            x = -GL_VIEW_WIDTH + GL_VIEW_WIDTH*2 * (e.button.x) / WINDOW_WIDTH;
            y = -(-GL_VIEW_HEIGHT + GL_VIEW_HEIGHT*2 * (e.button.y) / WINDOW_HEIGHT);

            // L'utilisateur ferme la fenêtre
            if(e.type == SDL_QUIT){
                loop = 0;
                break;
            }else if(e.type == SDL_KEYDOWN 
                && (e.key.keysym.sym == SDLK_ESCAPE)){
                loop = 0; 
                break;
            }else if(e.type == SDL_MOUSEBUTTONUP 
                &&  quitSelected(interface, x, y) && help == -1 && towerToBuild == -1 && buildingToBuild == -1){
                loop = 0; 
                break;
            }
            
            switch(e.type){
                // Redimensionnement fenêtre
                case SDL_VIDEORESIZE:
                    reshape(&surface, e.resize.w, e.resize.h);
                    break;

                // Bouge souris
                case SDL_MOUSEMOTION:
                    if(towerToBuild != -1){
                        tempT = allocTower(towerToBuild, x, y);
                        drawTower(tempT);
                    }else if(buildingToBuild != -1){
                        
                    }
                    break;

                // Clic souris
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    printf("coord (%f, %f)\n", x, y);
                    strcpy(interface->infosConstructions, "");
                    switch(e.button.button){
                        case SDL_BUTTON_LEFT:
                            if(towerToBuild != -1){
                                tempT = allocTower(towerToBuild, x, y);
                                if(!(constructionIntersection(buildings, towers, tempT->x, tempT->y, tempT->size, tempT->shape)) 
                                   && (tempT->cost <= game->money)
                                   && (doesCircleIntersectsPath(e.button.x, e.button.y, tempT->size, nodes, WINDOW_WIDTH, WINDOW_HEIGHT) == 0)){
                                    addTower(tempT, &towers);
                                    giveBonusTower(tempT, &buildings);
                                    game->money -= tempT->cost;
                                }else{
                                    free(tempT);
                                }
                            }else if(buildingToBuild != -1){
                                tempB = allocBuilding(buildingToBuild, x, y);
                                if(!(constructionIntersection(buildings, towers, tempB->x, tempB->y, tempB->size, tempB->shape)) 
                                   && (tempB->cost <= game->money)
                                   && (doesCircleIntersectsPath(e.button.x, e.button.y, tempB->size, nodes, WINDOW_WIDTH, WINDOW_HEIGHT) == 0)){
                                    addBuilding(tempB, &buildings);
                                    giveBonusTowers(tempB, &towers);
                                    game->money -= tempB->cost;
                                }else{
                                    free(tempB);
                                }
                            }else{
                                tempT = towerSelected(towers, x, y);
                                tempB = buildingSelected(buildings, x, y);
                                if(tempT != NULL){
                                    drawInfosTower(tempT, interface->infosConstructions);
                                }else if(tempB != NULL){
                                    drawInfosBuilding(tempB, interface->infosConstructions);
                                }
                            }
                            break;

                        case SDL_BUTTON_RIGHT:
                            tempT = towerSelected(towers, x, y);
                            tempB = buildingSelected(buildings, x, y);
                            if(tempT != NULL){
                                removeBonusTower(tempT, &buildings);
                                deleteTower(tempT, &towers);
                                game->money += tempT->cost;
                            }else if(tempB != NULL){
                                removeBonusTowers(tempB, &towers);
                                deleteBuilding(tempB, &buildings);
                                game->money += tempB->cost;
                            }
                            break;

                        default:
                            break;
                    }
                    break;
                
                // Touche clavier enfoncée
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    // Sélection des tours
                    if(e.key.keysym.sym == SDLK_a){
                        towerToBuild = TRED;
                    }
                    if(e.key.keysym.sym == SDLK_z){
                        towerToBuild = TPURPLE;
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
                    // Aide
                    if(e.key.keysym.sym == SDLK_h){
                        help = 1;
                    }
                    break;

                // Touche clavier retirée
                case SDL_KEYUP:
                    // Désélection des tours
                    if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_r) {
                        towerToBuild = -1;
                    }
                    // Désélection des bâtiments
                    if(e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_d) {
                        buildingToBuild = -1;
                    }
                    // Quitter aide
                    if(e.key.keysym.sym == SDLK_h){
                        help = -1;
                    }
                    break;
                    
                default:
                    break;
            }
        }

        /* Temps écoulé */
        counter += 100;
        // Calcul du temps écoulé
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        // Si trop peu de temps s'est écoulé, on met en pause le programme
        if(elapsedTime < FRAMERATE_MILLISECONDS){
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Libération ressources */
    // Libération des ressources associees a la SDL
    SDL_Quit();
    // Libération allocations et mémoire texture 
    freeGame(game);
    freeInterface(interface);
    freeImageMap(&imageMap);
    freeTowers(&towers);
    freeBuildings(&buildings);
    freeMonsters(&monsters);
    
    return EXIT_SUCCESS;
}