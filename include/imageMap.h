#ifndef IMAGEMAP_H__
#define IMAGEMAP_H__

/// \brief enumeration of possible colours
typedef enum {red, green, blue, all} colour;

/// \brief structure that contains an image
typedef struct ImageMap{
  unsigned char *data;
  unsigned int width;
  unsigned int height;

  GLuint sprite; // Sprite
}ImageMap;

/// \brief allocate the memory for a new image
/// \param image: pointer on the image
/// \param width: width of the required the image
/// \param widthheight: height of the required the image
/// \return EXIT_FAILURE in case of problem, else EXIT_SUCCESS
void newImageMap(ImageMap *imageMap, unsigned int width, unsigned int height, char *fileName);

/// \brief release the image memory
/// \param imageMap: pointer on the image
void freeImageMap(ImageMap* imageMap);

/// \brief load an image with PPM (P3) file format
/// \param image: pointer on the image to load
/// \param filename: char array containing the filename of the image to load
/// \return EXIT_FAILURE in case of problem, else EXIT_SUCCESS
void loadImageMapPPM(ImageMap* imageMap, char *fileName);

int testPixel(int pixel);

void drawImageMap(ImageMap* imageMap, int width, int height);

#endif
