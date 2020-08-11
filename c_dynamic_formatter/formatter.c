/*
 * UVic SENG 265, Fall 2016,  A#4
 *
 * This will contain the bulk of the work for the fourth assignment. It
 * provide similar functionality to the class written in Python for
 * assignment #3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formatter.h"

#include <assert.h>

//Gets rid of '\n' at end of line
void chomp(char *line){
	assert(line != NULL);
	
	int len = strlen(line);
	if (line[len-1] == '\n'){
		line[len-1] = '\0';
	}
}

//Add word to lines[currline]
char * addword(char *lineptr, char *word, int *spaceleft){
	strncpy(lineptr, word,strlen(word)); //Add word to line (without the null terminator)
	*spaceleft = *spaceleft - strlen(word); //Update spaceleft
	lineptr = lineptr + strlen(word); //move pointer
	return lineptr;
}

//Add marginspacing if new line
char * marginspacing(char *lineptr, char *line, int *spaceleft, int lm){
	if(lm > 0 && lineptr == line){
		int i;
		for(i = 0; i < lm; i++){
			strncpy(lineptr, " ", 1);
			*spaceleft = *spaceleft - 1;
			lineptr = lineptr + 1;
		}
	}
	return lineptr;
}

char ** realloc_check(char **lines, int currline, int *linessize){
	//REALLOC if necessary
	if(currline > *linessize){
		//if not enough space, realloc
		*linessize = *linessize*2;
		lines = (char**) realloc(lines, sizeof(char*)*(*linessize));
		if(!lines){
			abort();
		}
	}
	return lines;
}

//  since lineptr = &content,  *lineptr = content
//  then ptr = &lineptr  *ptr = &content,  **ptr = content
//  line = lines[currline-1]  (address)
char * setnextline(char *line, char **ptr, int *spaceleft, int lw){
	//Start next line
	//Space for lw+1 chars, position lw will be for null char
	line = (char*) malloc(sizeof(char)*(lw+1));
	if(!line){
		abort();
	}
	
	//set max length
	*ptr = line;
	*ptr = *ptr + lw;
	**ptr = '\0';
	//start at new line
	*ptr = line;
	*spaceleft = lw;
	return line;
}

int lmvaluecheck(char *lmstring, int lm, int lw){	
	/*Check if relative value*/
	if(strncmp(lmstring, "+", 1) == 0){
		lm = lm + atoi(lmstring);
	}else if(strncmp(lmstring, "-", 1) == 0){
		lm = lm + atoi(lmstring);
	}else{
		lm = atoi(lmstring);
	}
	/*Check if valid margin*/
	if (lm < 0){
		lm = 0;
	}else if(lm > (lw-20)){
		lm = lw - 20;
		if(lm < 0){
			lm = 0;
		}
	}
	return lm;
}
	

char **format_file(FILE *infile) {
	char *buffer = NULL; 
	size_t len = 0;
	ssize_t read;  /*length of buffer including null*/

	char **lines; /*formatted lines*/
	int linessize = 5; /*initial lines size*/
	int currline = 0;
	char *lineptr; /*points to where next word should be added in lines[currline]*/
	int spaceleft;
	
	int spaceadded = 0;
	int firstlineft = 0;

	char* lwtest = ".LW";
	int lw = 0;

	char* lmtest = ".LM";
	int lm = 0;

	char* lstest = ".LS";
	int ls = 0;

	char* fttest = ".FT";
	char* ftoff = "off";
	char* fton = "on";
	int ft = 0;

	/* ARRAY OF STRINGS INITIALIZATION*/
	lines = (char**) malloc(sizeof(char*)*linessize);
	if(!lines){
		printf("Error allocating memory. A");
		abort();
	}

	//For every line of input
	while((read = getline(&buffer, &len, infile)) != -1){
		/*Check for .LW command*/
		if(strncmp(buffer, lwtest, 3) == 0){
			char* lwstring = strtok(buffer, " ");
			lwstring = strtok(NULL, " ");
			lw = atoi(lwstring);
			
			if(ft == 0){		
				ft = 1;
				firstlineft = 1;
			}
		/*Check for .FT command*/
		}else if(strncmp(buffer, fttest, 3) == 0){
			char* ftstring = strtok(buffer, " ");
			ftstring = strtok(NULL," ");
			if(strncmp(ftstring, ftoff, 3) == 0){
				//Formatting picket so no extra blank space
				if(ft == 1 && lineptr == lines[currline-1]){
					lines[currline-1] = NULL;
				}

				ft = 0;
				firstlineft = 1;
			}else if(strncmp(ftstring, fton, 2) == 0){
				ft = 1;
			}
		/* Check for .LM command*/
		}else if(strncmp(buffer, lmtest, 3) == 0){
			char* lmstring = strtok(buffer, " ");
			lmstring = strtok(NULL, " ");
			lm = lmvaluecheck(lmstring, lm, lw);	
		/* Check for .LS command*/
		}else if(strncmp(buffer, lstest, 3) == 0){
			char* lsstring = strtok(buffer, " ");
			lsstring = strtok(NULL, " ");
			ls = atoi(lsstring);

		}else{	
			if(ft == 0){
				//no formatting
				chomp(buffer);
				
				//if first line or ft was turned off
				if(currline == 0 || lines[currline-1] != NULL){
					currline = currline + 1;
				}
				
				lines = realloc_check(lines, currline, &linessize);
				
				//add into line with no formatting
				lines[currline-1] = (char*) malloc(sizeof(char)*read);
				if(!lines[currline-1]){
					printf("Error allocating memory. B");
					abort();
				}
				strncpy(lines[currline-1],buffer,read);
			}else{
				//with formatting	
				chomp(buffer);
				char* word = strtok(buffer, " ");
				
				//if firstline with ft on
				if(firstlineft == 1){
					currline = currline + 1;
					lines[currline-1] = setnextline(lines[currline-1], &lineptr, &spaceleft, lw);
					
					firstlineft = 0;
				}
		
				//If buffer line was a '\n'
				if(strncmp(buffer, "\0",1) == 0){
					if(lineptr != lines[currline-1]){
						//End line.
						*lineptr = '\0';
						currline = currline + 1;
						lines = realloc_check(lines, currline, &linessize);
						lines[currline-1] = setnextline(lines[currline-1], &lineptr, &spaceleft, lw);
					}				

					//Line Spacing
					if(ls != 0){
						int i;
						for(i = 0; i < ls; i++){
							*lineptr = '\0';
							currline = currline + 1;
							lines = realloc_check(lines, currline, &linessize);
							lines[currline-1] = setnextline(lines[currline-1], &lineptr, &spaceleft, lw);
						}
					}
					
					//Add Blank line
					*lineptr = '\0';
					currline = currline + 1;
					lines = realloc_check(lines, currline, &linessize);
					lines[currline-1] = setnextline(lines[currline-1], &lineptr, &spaceleft, lw);
			
					//Line Spacing
					if(ls != 0){
						int i;
						for(i = 0; i < ls; i++){
							*lineptr = '\0';
							currline = currline + 1;
							lines = realloc_check(lines, currline, &linessize);
							lines[currline-1] = setnextline(lines[currline-1], &lineptr, &spaceleft, lw);
						}
					}
				}

				//For each word in buffer
				while(word != NULL){

					//Add margin if new line
					lineptr = marginspacing(lineptr, lines[currline-1], &spaceleft, lm);

					//Add space only when "not a new line", "enough space for space and next work"
					int insertspacecheck = spaceleft - 1 - strlen(word);
					if(lineptr != (lines[currline-1]+lm) && insertspacecheck >= 0){
						strncpy(lineptr, " ", 1);
						spaceleft = spaceleft - 1;
						lineptr = lineptr + 1;
						
						spaceadded = 1;
					}
				
					//If not enough space left in line
					if ((strlen(word)) > spaceleft || (strlen(word) == spaceleft && spaceadded == 0)){
						//End line, lineptr reset to beginning of next
						*lineptr = '\0';

						currline = currline + 1;
						
						lines = realloc_check(lines, currline, &linessize);
						lines[currline-1] = setnextline(lines[currline-1], &lineptr, &spaceleft, lw);

						//Line Spacing
						if(ls != 0){
							int i;
							for(i = 0; i < ls; i++){
								*lineptr = '\0';
								currline = currline + 1;
								lines = realloc_check(lines, currline, &linessize);
								lines[currline-1] = setnextline(lines[currline-1], &lineptr, &spaceleft, lw);
							}
						}
			
						//Add margin if new line
						lineptr = marginspacing(lineptr, lines[currline-1], &spaceleft, lm);
		
						//Add word to line
						lineptr = addword(lineptr, word, &spaceleft);
					}else{
						//Add word to line
						lineptr = addword(lineptr, word, &spaceleft);

						spaceadded = 0;
					}
					word = strtok(NULL, " ");
				}
			}
		}
	}

	//Formatting picket
	if(ft == 1 && lineptr != lines[currline-1]){
		//End line.
		*lineptr = '\0';
	}else if(ft == 1 && lineptr == lines[currline-1]){
		//formatting picket so no extra blank space if \n is last
		lines[currline-1] = NULL;
	}

	if (buffer) {
		free(buffer);
	}
	return lines;
}


char **format_lines(char **lines, int num_lines) {	
	char **result;

#ifdef DEBUG
	result = (char **)malloc(sizeof(char *) * 2);
	if (result == NULL) {
		return NULL;
	}

	result[0] = (char *)malloc(sizeof(char) * 80);
	if (result[0] == NULL) {
		return NULL;
	}
	strncpy(result[0], "(machine-like voice) EXTERMINATE THEM!", 79);

	result[1] = (char *)malloc(sizeof(char) * 2);
	if (result[1] == NULL) {
		return NULL;
	}
	result[1][0] = '\0';
#endif

	FILE *f = fopen("linesasfile.txt", "w");
	if (f == NULL){
		printf("Error");
		exit(1);
	}
	int i;
	for(i = 0; i < num_lines; i++){	
		fprintf(f, "%s\n", lines[i]);
	}
	fclose(f);

	FILE *data_fp = fopen("linesasfile.txt", "r");
	if(data_fp == NULL){
		fprintf(stderr, "Unable to open linesasfile.txt");
		exit(1);
	}
	
	result = format_file(data_fp);
	return result;
}
