#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
#include "../include/imageMap.h"

void newImageMap(ImageMap *imageMap, unsigned int width, unsigned int height, char *fileName){
  // Allocation de mémoire
  imageMap->data = (unsigned char*) malloc(sizeof(unsigned char) * width*height*3);
  if(imageMap->data == NULL){
    printf("newImageMap : erreur d'allocation de mémoire\n");
    exit(EXIT_FAILURE);
  }

  // Actualisation hauteur et largeur
  imageMap->width  = width;
  imageMap->height = height;

  // Sprite de la carte
  char fileNameJpg[255]= "";
  strcat(strcpy(fileNameJpg, fileName), ".jpg");
  imageMap->sprite = loadTexture(fileNameJpg);
}

void freeImageMap(ImageMap *imageMap){
  if(imageMap != NULL) {
    if(imageMap->data != NULL)	{
	    free(imageMap->data);
      imageMap->data= NULL;
    }
    if(imageMap->sprite){
      glDeleteTextures(1, &(imageMap->sprite));
    }
    imageMap->width  = 0;
    imageMap->height = 0;
	}
}

void loadImageMapPPM(ImageMap* imageMap, char *fileName){
  FILE *myFile = NULL;
  char chaine[255];
  unsigned int width, height;

  // Ouvrir fichier ppm pour lecture (rt)
  char fileNamePpm[255]= "";
  strcat(strcpy(fileNamePpm, fileName), ".ppm");
  if (!(myFile = fopen(fileNamePpm, "rt"))){ 
    printf("loadImageMapPPM: erreur pour ouvrir le fichier %s.\n", fileName);
    exit(EXIT_FAILURE);
  }

  // Lire entête "P3"
  fscanf(myFile, "%s\n", chaine);

  // Lire commentaires
  do{
    fgets(chaine, 255, myFile);
  } while (chaine[0]=='#');

  // Lire hauteur et largeur
  sscanf(chaine, "%d %d", &width, &height);

  // Lire "255"
  fscanf(myFile, "%s\n", chaine);

  // Allocation de la mémoire
  newImageMap(imageMap, width, height, fileName);

  // Lire les données
  imageMap->data = malloc(width*height*3*sizeof(unsigned char));
  if(imageMap->data == NULL){
    printf("newImageMap : erreur d'allocation de mémoire\n");
    exit(EXIT_FAILURE);
  }
  for (int i=0; i<width*height*3; i++){
    if (fscanf(myFile, "%hhu", (imageMap->data)+i) != 1) {
      printf("newImageMap : ne peut pas lire le fichier ppm %s, composantes RGB invalides\n", fileName);
    }
  }

  // Fermer fichier
  fclose(myFile);
}

int testPixel(int pixel){
  if(pixel >= 0 && pixel <= 255){
    return 0;
  }
  return 1;
}

void drawImageMap(ImageMap* imageMap, int width, int height){
  glPushMatrix();
      glScalef(width, height, 0);
      drawPicture(imageMap->sprite);
  glPopMatrix();
}