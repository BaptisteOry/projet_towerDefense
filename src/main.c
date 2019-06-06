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
#include "../include/game.h"
#include "../include/construction.h"
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
    // Initialisation random
    srand(time(NULL));
    // Initialisation des valeurs principales
    static ImageMap imageMap;
    static char imageMap01[255]="images/map01";
    static ItdEltsInfos* infos = NULL;
    static char* itdFile = "data/map01.itd";
    static Node* nodes = NULL;
    static Link* links = NULL;
    static Game* game;
    static Interface* interface;
    static MonsterList monsters;
    static TowerList towers;
    static BuildingList buildings;
    static Tower* towerToBuild = NULL;
    static Tower* towerSelect = NULL; 
    static Building* buildingToBuild = NULL;
    static Building* buildingSelect = NULL;
    float x = 0;
    float y = 0;
    int counter = 0;

    // Charger image ppm (placer dans l'image carte)
    loadImageMapPPM(&imageMap, imageMap01);
    // Vérification carte et itd
    int nbOfNodes = 0;
    itdCheck(itdFile, &imageMap, &infos, &nodes, &links, &nbOfNodes);
    createLinkedNodeList(nodes, links);

    // Vérification du ppm
    mapCheck(&imageMap, infos, nodes);

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

        /* Dessin des éléments */
        if(game->status == 1 || game->status == 2){
            drawImageMap(&imageMap, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
            drawMonsters(monsters);
            drawRangeTowers(towers);
            drawRangeBuildings(buildings);
            drawTowers(towers);
            drawBuildings(buildings);
            drawInfosConstructions(interface);
            drawGameElements(interface, game);
            drawTower(towerToBuild);
            drawBuilding(buildingToBuild);
        }
        if(game->status == 0){
            drawBeginning(interface);
        }else if(game->status == 2){
            drawHelp(interface);
        }else if(game->status == 3){
            drawEnd(interface, game);
        }

        /* Évènements en jeu */
        if(game->status == 1){
            // Création de vagues de monstres
            addWave(game, &monsters, counter, nodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
            // Destruction monstres
            killMonsters(game, &monsters, &towers, counter);
            // Déplacement de la vague de monstre;
            (game->lose) = moveMonsters(&monsters, nodes, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
            // Fin du jeu
            endGame(game, &monsters);
            // Avancer le compteur
            counter += 100;
        }

        // Échange du front et du back buffer : mise à jour de la fenêtre
        SDL_GL_SwapBuffers();
        
        /* Évènements joueur */
        SDL_Event e;
        while(SDL_PollEvent(&e)){

            // Fermer la fenêtre
            if(e.type == SDL_QUIT){
                loop = 0;
                break;
            }else if(e.type == SDL_KEYDOWN 
                && (e.key.keysym.sym == SDLK_ESCAPE)){
                loop = 0; 
                break;
            }else if(e.type == SDL_MOUSEBUTTONUP 
                && quitSelected(interface, x, y) && (game->status) == 1 && towerToBuild == NULL && buildingToBuild == NULL){
                loop = 0; 
                break;
            }

            // Redimensionner la fenêtre
            if(e.type == SDL_VIDEORESIZE){
                reshape(&surface, e.resize.w, e.resize.h);
            }

            // Évènements joueur selon le statut du jeu
            if(game->status == 0){
                x = -GL_VIEW_WIDTH + GL_VIEW_WIDTH*2 * (e.button.x) / WINDOW_WIDTH;
                y = -(-GL_VIEW_HEIGHT + GL_VIEW_HEIGHT*2 * (e.button.y) / WINDOW_HEIGHT);

                if(e.type == SDL_MOUSEBUTTONUP 
                    && menuBSelected(interface, x, y)){
                    game->status = 1;
                }

            } else if(game->status == 1){

                switch(e.type){

                    // Bouge souris
                    case SDL_MOUSEMOTION:
                        x = -GL_VIEW_WIDTH + GL_VIEW_WIDTH*2 * (e.button.x) / WINDOW_WIDTH;
                        y = -(-GL_VIEW_HEIGHT + GL_VIEW_HEIGHT*2 * (e.button.y) / WINDOW_HEIGHT);

                        if(towerToBuild != NULL){
                            towerToBuild->x = x; towerToBuild->y = y;
                            towerConstructible(towerToBuild, buildings, towers, nodes, game, x, y, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
                        }else if(buildingToBuild != NULL){
                            buildingToBuild->x = x; buildingToBuild->y = y;
                            buildingConstructible(buildingToBuild, buildings, towers, nodes, game, x, y, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
                        }
                        break;

                    // Clic souris
                    case SDL_MOUSEBUTTONUP:
                        strcpy(interface->infosConstructions, "");

                        // Quitter le jeu avec le bouton
                        if(quitSelected(interface, x, y) 
                            && (game->status) == 1 && towerToBuild == NULL && buildingToBuild == NULL){
                            loop = 0; 
                            break;
                        }

                        switch(e.button.button){
                            case SDL_BUTTON_LEFT:
                                if(towerToBuild != NULL && towerToBuild->constructible != 0){
                                    addTower(towerToBuild, &towers);
                                    giveBonusTower(towerToBuild, &buildings);
                                    game->money -= towerToBuild->cost;
                                    towerToBuild = allocTower(towerToBuild->type, x, y);
                                }else if(buildingToBuild != NULL && buildingToBuild->constructible != 0){
                                    addBuilding(buildingToBuild, &buildings);
                                    giveBonusTowers(buildingToBuild, &towers);
                                    game->money -= buildingToBuild->cost;
                                    buildingToBuild = allocBuilding(buildingToBuild->type, x, y);
                                }else{
                                    towerSelect = towerSelected(towers, x, y);
                                    buildingSelect = buildingSelected(buildings, x, y);
                                    if(towerSelect != NULL){
                                        drawInfosTower(towerSelect, interface->infosConstructions);
                                    }else if(buildingSelect != NULL){
                                        drawInfosBuilding(buildingSelect, interface->infosConstructions);
                                    }
                                }
                                break;

                            case SDL_BUTTON_RIGHT:
                                towerSelect = towerSelected(towers, x, y);
                                buildingSelect = buildingSelected(buildings, x, y);
                                if(towerSelect != NULL){
                                    removeBonusTower(towerSelect, &buildings);
                                    deleteTower(towerSelect, &towers);
                                    game->money += (towerSelect->cost)/2;
                                    towerSelect = NULL;
                                }else if(buildingSelect != NULL){
                                    removeBonusTowers(buildingSelect, &towers);
                                    deleteBuilding(buildingSelect, &buildings);
                                    game->money += (buildingSelect->cost)/2;
                                    buildingSelect = NULL;
                                }
                                break;

                            default:
                                break;
                        }
                        break;
                    
                    // Touche clavier enfoncée
                    case SDL_KEYDOWN:
                        // Sélection des tours
                        if(e.key.keysym.sym == SDLK_a){
                            towerToBuild = allocTower(TRED, x, y);
                        }
                        if(e.key.keysym.sym == SDLK_z){
                            towerToBuild = allocTower(TPURPLE, x, y);
                        }
                        if(e.key.keysym.sym == SDLK_e){
                            towerToBuild = allocTower(TYELLOW, x, y);
                        }
                        if(e.key.keysym.sym == SDLK_r){
                            towerToBuild = allocTower(TBLUE, x, y);
                        }
                        if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_r) {
                            towerConstructible(towerToBuild, buildings, towers, nodes, game, x, y, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
                        }
                        // Sélection des bâtiments
                        if(e.key.keysym.sym == SDLK_q){
                            buildingToBuild = allocBuilding(RADAR, x, y);
                        }
                        if(e.key.keysym.sym == SDLK_s){
                            buildingToBuild = allocBuilding(FACTORY, x, y);
                        }
                        if(e.key.keysym.sym == SDLK_d){
                            buildingToBuild = allocBuilding(STOCK, x, y);
                        }
                        if(e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_d) {
                            buildingConstructible(buildingToBuild, buildings, towers, nodes, game, x, y, GL_VIEW_WIDTH, GL_VIEW_HEIGHT);
                        }
                        // Aide
                        if(e.key.keysym.sym == SDLK_h){
                            (game->status) = 2;
                        }
                        break;

                    // Touche clavier retirée
                    case SDL_KEYUP:
                        // Désélection des tours
                        if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_e || e.key.keysym.sym == SDLK_r) {
                            towerToBuild = freeTower(towerToBuild);
                        }
                        // Désélection des bâtiments
                        if(e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_d) {
                            buildingToBuild = freeBuilding(buildingToBuild);
                        }
                        
                    default:
                        break;

                }

            } else if (game->status == 2) {

                // Quitter aide
                if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_h){
                    (game->status) = 1;
                }

            } else if (game->status == 3) {
                x = -GL_VIEW_WIDTH + GL_VIEW_WIDTH*2 * (e.button.x) / WINDOW_WIDTH;
                y = -(-GL_VIEW_HEIGHT + GL_VIEW_HEIGHT*2 * (e.button.y) / WINDOW_HEIGHT);

                if(e.type == SDL_MOUSEBUTTONUP 
                    && menuBSelected(interface, x, y)){
                    strcpy(interface->infosConstructions, "");
                    counter = 0;
                    freeTowers(&towers);
                    freeBuildings(&buildings);
                    freeMonsters(&monsters);
                    towerToBuild = freeTower(towerToBuild);
                    buildingToBuild = freeBuilding(buildingToBuild);
                    freeGame(game);
                    game = allocGame();
                    game->status = 1;
                }
            }

        }
        /* Temps écoulé */
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
    freeTower(towerToBuild);
    freeBuilding(buildingToBuild);
    
    return EXIT_SUCCESS;
}
