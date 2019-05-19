#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "../include/controller.h"
#include "../include/imageMap.h"

int newImageMap(ImageMap *imageMap, unsigned int width, unsigned int height, char *fileName)
{
  // memory allocation
  imageMap->data = (unsigned char*) malloc(sizeof(unsigned char) * width*height*3);
  if(imageMap->data == NULL){
    printf("newImageMap: error bad memory allocation.\n");
    return EXIT_FAILURE;
  }

  // update width and height
  imageMap->width  = width;
  imageMap->height = height;

  // load sprite
  char fileNameJpg[255]= "";
  strcat(strcpy(fileNameJpg, fileName), ".jpg");
  imageMap->sprite = loadTexture(fileNameJpg);

  return EXIT_SUCCESS;
}

void freeImageMap(ImageMap *imageMap)
{
  if(imageMap != NULL) {
    if(imageMap->data != NULL)	{
	    free(imageMap->data);
      imageMap->data= NULL;
    }

    imageMap->width  = 0;
    imageMap->height = 0;
	}
}

// reads P3
int loadImageMapPPM(ImageMap* imageMap, char *fileName){
  FILE *myFile = NULL;
  char chaine[255];
  unsigned int width,height;

  // open the file for reading (rt)
  char fileNamePpm[255]= "";
  strcat(strcpy(fileNamePpm, fileName), ".ppm");
  if (!(myFile = fopen(fileNamePpm, "rt"))){ 
    printf("loadImageMapPPM: error opening file %s.\n", fileName);
    return EXIT_FAILURE;
  }

  // read header "P3"
  fscanf(myFile, "%s\n", chaine);

  // read comments
  do{
    fgets(chaine, 255, myFile);
  } while (chaine[0]=='#');
  
  // read width and height
  sscanf(chaine, "%d %d", &width, &height);

  // read the "255"
  fscanf(myFile, "%s\n", chaine);

  // memory allocation
  if(newImageMap(imageMap, width, height, fileName) == EXIT_FAILURE){
    printf("newImage: memory allocation error\n");
    return EXIT_FAILURE;
  }

  // read the store data
  fread(imageMap->data, sizeof(unsigned char), width*height*3, myFile);

  // close the file
  fclose(myFile);

  return EXIT_SUCCESS;
}

int testPixel(int pixel){
  if(pixel>=0 && pixel<=255){
    return 0;
  }
  return 1;
}

void drawImageMap(ImageMap* imageMap, int width, int height){
  glPushMatrix();
      drawPicture(imageMap->sprite, width, height); // Taille image map
  glPopMatrix();
}