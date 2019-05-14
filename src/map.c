#include <stdio.h>
#include <stdlib.h>

#include "../include/mapVerif.h"

void itdCheck(char* itdFile) {

	// ITD file elements
	char code[5];
	int version;
	char comments[50];
	char keyword[10];
	int valueR, valueG, valueB;
	char ppmFileName[10];
	int nodesNumber;

	// Open .itd file
	FILE *file;

	file = fopen(itdFile, "r"); // read mode
 
	if (file == NULL) {
		printf("Error while opening .itd file.\n");
	  	return EXIT_FAILURE;
	}

	/* FILE CHECK */

	// itd code check
	fscanf(file, "%s %d", code, &version);
	printf("%s\n", code);
	if(code != "@ITD") {
		printf("ERROR : Wrong .itd file format.\n");
		EXIT_FAILURE;
	}
	if(version != 1) {
		printf("Itd version not supported.\n");
		EXIT_FAILURE;
	}

	// Read comment
	fgets(comments, sizeof comments, file);
	if(comments[0] != #) {
		printf("Second line isn't comments.")
		EXIT_FAILURE
	}
	fputs(comments, stdout);

	// Read variables & values

	// Store variables & values

	// Read nodes description

	// Store nodes descrition by creating the graph


	/* PPM MAP CHECK */


 
 	// Close .itd file
	fclose(file);
}

void mapCheck(ImageMap map, char* itdFile, ItsEltsInfos* infos) {
	
}

/*void createGraph(Graph* graph) {

}*/