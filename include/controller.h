#ifndef CONTROLLER_H__
#define CONTROLLER_H__

/// \enumeration
typedef enum {red, green, blue, all} exColours;

/// \structure
typedef struct exStruct{
  unsigned char *ex1;
  unsigned int ex2;
}Image;

/// \description
/// \param param:
/// \return
int myFunction(unsigned int param);

#endif