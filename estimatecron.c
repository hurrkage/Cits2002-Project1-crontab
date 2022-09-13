// header files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 10000
#define YEAR 2022

void usage(char argv0[])
{
	fprintf(stderr,"Usage: %s  [month] [crontab file] [estimates file]\n        where [month] = 0,1,2,...11 or jan,feb,mar,...dec\n\n",
	argv0);
	exit(EXIT_FAILURE);
}


// READ CRONTAB FILE
void read_crontab(char filename[])
{
	FILE *fp = fopen(filename,"r");
        char line[BUFFERSIZE];

	if(fp == NULL) {
		fprintf(stderr,"File: %s cannot be opened.\n%s does not exist?, %s is empty?\n",filename,filename,filename);
		exit(EXIT_FAILURE);
	}
        
	while(fgets(line,sizeof(line),fp) != NULL) {
		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES)
		int count = 0;
		
		if(line[0] == '#') {
			count++;

                        if(count > 20) {
			        fprintf(stderr,"Error: Too many comment lines in %s\n",filename);
			        exit(EXIT_FAILURE);
			}
		}
		// FILE DEBUGGING (CHECK FOR NO. OF CHARACTERS IN A LINE)
	        if(strlen(line) > 100) {
			fprintf(stderr,"Error: Too many characters in a line in %s\n",filename);
			exit(EXIT_FAILURE);
		}

		// READ COMMAND LINES
		if(line[0] != '#') {
			printf("%s",line);
		}
	}
	fclose(fp);
}


// READ ESTIMATES FILE
void read_estimates(char filename[])
{
	FILE *fp = fopen(filename,"r");
        char line[BUFFERSIZE];

	if(fp == NULL) {
		fprintf(stderr,"File: %s cannot be opened.\n%s does not exist?, %s is empty?\n",filename,filename,filename);
		exit(EXIT_FAILURE);
	}
        
        
	// PRINTS COMMAND LINES
	while(fgets(line,sizeof(line),fp) != NULL) {
		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES) 	
		
		// FILE DEBUGGING (CHECK FOR NO. OF CHARACTERS IN A LINE)
	        if(strlen(line) > 100) {
			fprintf(stderr,"Error: Too many characters in a line in %s\n",filename);
			exit(EXIT_FAILURE);
		}
		// READ COMMAND LINES
		if(line[0] != '#') {
			printf("%s",line);
		}
	}
	fclose(fp);
}

// TO CHECK WHETHER THE INPUT MONTH IS CORRECT OR NOT
int check_month(char month[])
{
	
	// CONVERT STRING TO LOWERCASE
	
        char array_month[50];
	for(size_t i = 0; i < strlen(month); i++) {
		array_month[i] = tolower(month[i]);
        }
	array_month[strlen(month)] = '\0';
	
	// DECLARE ALL THE VALID INPUT MONTH IN LETTERS
	char valid_month[24][50] = {"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec","0","1","2","3","4","5","6","7","8","9","10","11"};

	for(int i = 0; i < 24; i++) {
		if(strcmp(array_month,valid_month[i]) == 0) {
			return 0;
		}
	}
	return 1;
}



int main(int argc, char *argv[])
{
	if(argc != 4) {
		usage(argv[0]);
	}

	if(check_month(argv[1]) == 1) {
	        usage(argv[0]);
	}


	printf("\n");
	printf("running program...\n");

	read_crontab(argv[2]);
	printf("\n");
	read_estimates(argv[3]);

	return EXIT_SUCCESS;
}
