/*
 * UVic SENG 265, Fall 2016, A#4
 *
 * This will contain a solution to format265alt.c. In order to complete the
 * task of formatting a file, it must open the file and pass the result
 * to a routine in formatter.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include "formatter.h"

int main(int argc, char *argv[]) {
#ifdef DEBUG
//	printf("%s does nothing right now.\n", argv[0]);
#endif
	char **result;
	char **line;

	if(argc < 2){
		result = format_file(stdin);
	}else{

		FILE *data_fp = fopen(argv[1], "r");

		if(data_fp == NULL){
			fprintf(stderr, "Unable to open %s\n", argv[1]);
			exit(1);
		}
	
		result = format_file(data_fp);
	}
	if (result == NULL) {
		printf("%s: it appears 'format_file' is not yet complete\n",
			argv[0]);
		exit(1);
	}
	
	for (line = result; *line != NULL; line++) {
		printf ("%s\n", *line);
		free(*line);
	}
	if (result){
		free(result);
	}
	exit(0);
}
