#include <stdio.h>
#include <stdlib.h>

#include "../include/mapVerif.h"

void itdCheck(char* itdFile) {
	FILE *file;

	file = fopen(itdFile, "r"); // read mode
 
	if (file == NULL) {
		printf("Error while opening .itd file.\n");
	  	return EXIT_FAILURE;
	}

/*	char data; 
	fscanf(file, "%s", data&);
	printf("%s\n", data);
	if(data != "@ITD 1") {
		printf("ERROR : Wrong .itd file format.\n", );
	}*/
 
	fclose(file);
}

void mapCheck(ImageMap map, char* itdFile) {
	
}