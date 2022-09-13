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


//CHECK PARAMETERS OF CRONTAB FILE
//IF CHECK FUNCTION RETURNS 100, IT IS IMPLYING THAT THE ARGUEMENT IS A WILDCARD
int check_min(char min[])
{
	if(!isdigit(*min)) {
		return 100;
	}
        
	for(size_t i = 0; i < strlen(min); i++) {
		if(!isdigit(min[i])) {
			fprintf(stderr,"ERROR: %s in the given crontab file must be an integer",min);
			exit(EXIT_FAILURE);
	        }
        }

	int int_min = atoi(min);
	if( int_min < 0 || int_min > 59 ) {
		fprintf(stderr,"ERROR: value given in minutes in the given crontab file must be within 0 and 59");
		exit(EXIT_FAILURE);
	}

	return int_min;
}

int check_hour(char hour[])
{
	if(!isdigit(*hour)) {
		return 100;
	}

	for(size_t i = 0; i < strlen(hour); i++) {
		if(!isdigit(hour[i])) {
			fprintf(stderr,"ERROR: %s in the given crontab file must be an integer",hour);
			exit(EXIT_FAILURE);
	        }
	}

	int int_hour = atoi(hour);
	if( int_hour < 0 || int_hour > 24 ) {
		fprintf(stderr,"ERROR: value given in minutes in the given crontab file must be within 0 and 24");
		exit(EXIT_FAILURE);
	}

	return int_hour;
}

int check_date(char date[])
{
	if(!isdigit(*date)) {
		return 100;
	}

	for(size_t i = 0; i < strlen(date); i++) {
		if(!isdigit(date[i])) {
			fprintf(stderr,"ERROR: %s in the given crontab file must be an integer",date);
			exit(EXIT_FAILURE);
	        }
	}

	int int_date = atoi(date);
	if( int_date < 1 || int_date > 31 ) {
		fprintf(stderr,"ERROR: integer value given in minutes in the given crontab file must be within 1 and 31");
		exit(EXIT_FAILURE);
	}

	return int_date;
}

int check_mon(char month[])
{
	if(!isdigit(*month)) {
		return 100;
	}

	for(size_t i = 0; i < strlen(month); i++) {
		if(!isdigit(month[i])) {
			fprintf(stderr,"ERROR: %s in the given crontab file must be an integer",month);
			exit(EXIT_FAILURE);
	        }
	}

	int int_month = atoi(month);
	if( int_month < 0 || int_month > 11 ) {
		fprintf(stderr,"ERROR: integer value given in month in the given crontab file must be within 0 and 11");
		exit(EXIT_FAILURE);
	}

	return int_month;
}

int check_day(char day[])
{
	if(!isdigit(*day)) {
		return 100;
	}
	for(size_t i = 0; i < strlen(day); i++) {
		if(!isdigit(day[i])) {
			fprintf(stderr,"ERROR: %s in the given crontab file must be an integer",day);
			exit(EXIT_FAILURE);
	        }
	}

	int int_day = atoi(day);
	if( int_day < 0 || int_day > 11 ) {
		fprintf(stderr,"ERROR: integer value given in month in the given crontab file must be within 0 and 11");
		exit(EXIT_FAILURE);
	}

	return int_day;
}

char check_command_name(char command_name[])
{
}


// STRUCTURE TO STORE COMMAND LINE ARGUEMENTS FROM CRONTAB

struct command_args
{
	int min;
	int hour;
	int date;
	int month;
	int day;
	char *command_name;
};

//if(strcomp() == "mon"){
//	for(int i, loopn through the day array; i++ )	
//	day = i;


// READ CRONTAB FILE
void read_crontab(char filename[])
{
	FILE *fp = fopen(filename,"r");
        char line[BUFFERSIZE];

	if(fp == NULL) {
		fprintf(stderr,"File: %s cannot be opened.\n%s does not exist?, %s is empty?\n",filename,filename,filename);
			exit(EXIT_FAILURE);
	}

	int count = 0;
	// PROCESS COMMAND LINES
        while(fgets(line,sizeof(line),fp) != NULL) {

		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES)
		if(line[0] == '#') {
			count = count + 1;

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
			char min[5], hour[5], date[5], month[5], day[5], command_name[40];

			sscanf(line,"%s %s %s %s %s %s", min, hour, date, month, day, command_name);


			
			
			
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
        
        int count = 0;
	// PROCESS COMMAND LINES
	while(fgets(line,sizeof(line),fp) != NULL) {
		
		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES) 	
		if(line[0] == '#') {
			count = count + 1;

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
	printf("Running program...\n");
	printf("\n");

	read_crontab(argv[2]);
	printf("\n");
	read_estimates(argv[3]);

	return EXIT_SUCCESS;
}
