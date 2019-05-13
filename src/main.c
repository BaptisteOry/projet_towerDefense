#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "../include/controller.h"
#include "../include/imageMap.h"

// Dimensions initiales et titre de la fenetre
static const unsigned int WINDOW_WIDTH = 600;
static const unsigned int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "Tower defense";
// Espace fenetre virtuelle
static const float GL_VIEW_WIDTH = 150.;
static const float GL_VIEW_HEIGHT = 150.;
// Nombre de bits par pixel de la fenetre
static const unsigned int BIT_PER_PIXEL = 32;
// Nombre minimal de millisecondes separant le rendu de deux images
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape(SDL_Surface** surface, unsigned int width, unsigned int height)
{
    SDL_Surface* surface_temp = SDL_SetVideoMode(
        width, height, BIT_PER_PIXEL,
        SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
    if(NULL == surface_temp) 
    {
        fprintf(
            stderr, 
            "Erreur lors du redimensionnement de la fenetre.\n");
        exit(EXIT_FAILURE);
    }
    *surface = surface_temp;

    glViewport(0, 0, (*surface)->w, (*surface)->h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(
        -GL_VIEW_WIDTH / 2, GL_VIEW_WIDTH / 2, 
        -GL_VIEW_HEIGHT / 2, GL_VIEW_HEIGHT / 2);
}

/* Objets canoniques */
void drawSquare(int filled, int r, int g, int b) 
{
    float radio = 0.5;
    if(filled) 
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3ub(r, g, b);
        glVertex2f(0.0, 0.0);
    }
    else 
    {
        glBegin(GL_LINE_STRIP);
        glColor3ub(red, green, blue);
    }
    glVertex2f(radio , -radio);
    glVertex2f(radio , radio);
    glVertex2f(-radio , radio);
    glVertex2f(-radio , -radio);
    glVertex2f(radio , -radio);

    glEnd();
}


int main(int argc, char** argv) 
{
    // Initialisation des valeurs principales
    char imageMap01[255]="images/map01.ppm";

    // Creer image carte
    ImageMap imageMap;

    // Charger image ppm (placer dans l'image carte)
    if(loadImageMapPPM(&imageMap, imageMap01) != EXIT_SUCCESS){
        return EXIT_FAILURE;
    }

    // Initialisation de la SDL
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) 
    {
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Pour le temps 
    time_t rawtime;
    struct tm* timeinfo;*/

    /* Boucle principale */
    int loop = 1;
    while(loop) 
    {
        // Recuperation du temps au debut de la boucle
        Uint32 startTime = SDL_GetTicks();
        
        // Placer ici le code de dessin
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1);

        /* Récupération de l'heure
        time(&rawtime);
        timeinfo = localtime(&rawtime);*/

        // Dessin rectangle
        glPushMatrix();
            glScalef(3, 35, 0);
            drawSquare(1, 0, 255, 255);
        glPopMatrix();

        // Echange du front et du back buffer : mise a jour de la fenetre
        SDL_GL_SwapBuffers();
        
        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {
            // L'utilisateur ferme la fenetre
            if(e.type == SDL_QUIT) 
            {
                loop = 0;
                break;
            }

            if( e.type == SDL_KEYDOWN 
                && (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
            {
                loop = 0; 
                break;
            }
            
            // Quelques exemples de traitement d'evenements
            switch(e.type) 
            {
                // Redimensionnement fenetre
                case SDL_VIDEORESIZE:
                    reshape(&surface, e.resize.w, e.resize.h);
                    break;

                // Clic souris
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    break;
                
                // Touche clavier
                case SDL_KEYDOWN:
                    printf("touche pressee (code = %d)\n", e.key.keysym.sym);
                    break;
                    
                default:
                    break;
            }
        }

        // Calcul du temps ecoule
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        // Si trop peu de temps s'est ecoule, on met en pause le programme
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    // Liberation des ressources associees a la SDL
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
