#ifndef MAPVERIF_H__
#define MAPVERIF_H__

// Verifies if the .idt file is correct
void itdCheck(char* itdFile);

// Verifies if the .itd file corresponds to the map
void mapCheck(ImageMap map, char* itdFile);

#endif