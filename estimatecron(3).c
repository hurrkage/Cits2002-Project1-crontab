// header files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

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
	if(strcmp(min,"*") == 0) {
		return 100;
	}
        
	for(size_t i = 0; i < strlen(min); i++) {
		if(!isdigit(min[i])) {
			fprintf(stderr,"ERROR: %s(mins) in the given crontab file must be an integer\n",min);
			exit(EXIT_FAILURE);
	        }
        }

	int int_min = atoi(min);
	if( int_min < 0 || int_min > 59 ) {
		fprintf(stderr,"ERROR: value given in minutes in the given crontab file must be within 0 and 59\n");
		exit(EXIT_FAILURE);
	}

	return int_min;
}

int check_hour(char hour[])
{
	if(strcmp(hour,"*") == 0) {
		return 100;
	}

	for(size_t i = 0; i < strlen(hour); i++) {
		if(!isdigit(hour[i])) {
			fprintf(stderr,"ERROR: %s(hours) in the given crontab file must be an integer\n",hour);
			exit(EXIT_FAILURE);
	        }
	}

	int int_hour = atoi(hour);
	if( int_hour < 0 || int_hour > 24 ) {
		fprintf(stderr,"ERROR: value given in hours in the given crontab file must be within 0 and 24\n");
		exit(EXIT_FAILURE);
	}

	return int_hour;
}

int check_date(char date[])
{
	if(strcmp(date,"*") == 0) {
		return 100;
	}

	for(size_t i = 0; i < strlen(date); i++) {
		if(!isdigit(date[i])) {
			fprintf(stderr,"ERROR: %s(date) in the given crontab file must be an integer\n",date);
			exit(EXIT_FAILURE);
	        }
	}

	int int_date = atoi(date);
	if( int_date < 1 || int_date > 31 ) {
		fprintf(stderr,"ERROR: integer value given in date in the given crontab file must be within 1 and 31\n");
		exit(EXIT_FAILURE);
	}

	return int_date;
}

int check_month(char month[])
{
	if(strcmp(month,"*") == 0) {
		return 100;
	}

	for(size_t i = 0; i < strlen(month); i++) {
		if(!isdigit(month[i])) {
			fprintf(stderr,"ERROR: %s(month) in the given crontab file must be an integer\n",month);
			exit(EXIT_FAILURE);
	        }
	}

	int int_month = atoi(month);
	if( int_month < 0 || int_month > 11 ) {
		fprintf(stderr,"ERROR: integer value given in month in the given crontab file must be within 0 and 11\n");
		exit(EXIT_FAILURE);
	}

	return int_month;
}

int check_day(char day[])
{
	if(strcmp(day,"*") == 0) {
		return 100;
	}

	if(isdigit(day[0])) {
		int int_day = atoi(day);
		return int_day;
	}

	char lowered_day[50];

	for(size_t i = 0; i < strlen(day); i++) {
		lowered_day[i] = tolower(day[i]);
	}
        
	char valid_days[7][50] = {"mon","tue","wed","thu","fri","sat","sun"};

	for(int i = 0; i < 7; i++) {
		if( strcmp(lowered_day,valid_days[i]) == 0 ) {
			return i;
		}
	}

        fprintf(stderr,"ERROR: Day given in crontab is an invalid data\nUsage: day = mon/tue/wed/...sun OR 0/1/2/...6\n");
	exit(EXIT_FAILURE);	
}

void check_command_name(char command_name[])
{
	if(strlen(command_name) > 40) {
		fprintf(stderr,"ERROR: Command name in crontab file exceed the character limit(40)\n");
		exit(EXIT_FAILURE);
	}
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

int first_day_of_month(int month, int year)
{
	struct tm   tm;

	memset(&tm, 0, sizeof(tm));
	tm.tm_mday = 1;
	tm.tm_mon  = month-1;
        tm.tm_year = year-1900;

	mktime(&tm);
	return tm.tm_wday;
}



// READ CRONTAB AND ESTIMATE FILE
void read_file(char crontab[], char estimates[],int input_month)
{
	FILE *fp0 = fopen(crontab,"r");
        char line0[BUFFERSIZE];

	if(fp0 == NULL) {
		fprintf(stderr,"File: %s cannot be opened.\n%s does not exist?, %s is empty?\n",crontab,crontab,crontab);
			exit(EXIT_FAILURE);
	}

	int count = 0;
	// PROCESS COMMAND LINES
        while(fgets(line0,sizeof(line0),fp0) != NULL) {

		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES)
		if(line0[0] == '#') {
			count = count + 1;

                        if(count > 20) {
			        fprintf(stderr,"Error: Too many comment lines in %s\n",crontab);
			        exit(EXIT_FAILURE);
			}
		}
		// FILE DEBUGGING (CHECK FOR NO. OF CHARACTERS IN A LINE)
	        if(strlen(line0) > 100) {
			fprintf(stderr,"Error: Too many characters in a line in %s\n",crontab);
			exit(EXIT_FAILURE);
		}

		// READ COMMAND LINES
		if(line0[0] != '#') {
			char min[5], hour[5], date[5], month[5], day[5], command_name[40];

			sscanf(line0,"%s %s %s %s %s %s", min, hour, date, month, day, command_name);

			int processed_min = check_min(min);
			int processed_hour = check_hour(hour);
			int processed_date = check_date(date);
			int processed_month = check_month(month);
			int processed_day = check_day(day);

			int first_day = first_day_of_month(input_month,YEAR);

			//struct command_args mycommand_args = {min, hour, date, month, day, command_name};	
			


		}
	}
	fclose(fp0);


	FILE *fp1 = fopen(estimates,"r");
        char line1[BUFFERSIZE];

	if(fp1 == NULL) {
		fprintf(stderr,"File: %s cannot be opened.\n%s does not exist?, %s is empty?\n",estimates,estimates,estimates);
		exit(EXIT_FAILURE);
	}
        
        int count2 = 0;
	// PROCESS COMMAND LINES
	while(fgets(line1,sizeof(line1),fp1) != NULL) {
		
		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES) 	
		if(line1[0] == '#') {
			count2 = count2 + 1;

                        if(count2 > 20) {
			        fprintf(stderr,"Error: Too many comment lines in %s\n",estimates);
			        exit(EXIT_FAILURE);
			}
		}
		// FILE DEBUGGING (CHECK FOR NO. OF CHARACTERS IN A LINE)
	        if(strlen(line1) > 100) {
			fprintf(stderr,"Error: Too many characters in a line in %s\n",estimates);
			exit(EXIT_FAILURE);
		}
		// READ COMMAND LINES
		if(line1[0] != '#') {
			char command_name[40], duration[5];
			 sscanf(line1,"%s %s", command_name, duration);


		}
	}
	fclose(fp1);
}



// TO CHECK WHETHER THE INPUT MONTH IS CORRECT OR NOT AND CONVERTS MONTH IN ALPHABETS TO MONTH IN INTEGER
int check_valid_month(char month[])
{
		
	// CONVERT STRING TO LOWERCASE
	
        char array_month[50];
	for(size_t i = 0; i < strlen(month); i++) {
		array_month[i] = tolower(month[i]);
        }
	array_month[strlen(month)] = '\0';
	
	// DECLARE ALL THE VALID INPUT MONTH IN LETTERS AND CHECK WHETHER THE GIVEN MONTH IN LETTERS IS VALID
	char valid_month[12][50] = {"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};

	for(int i = 0; i < 12; i++) {
		if(strcmp(array_month,valid_month[i]) == 0) {
			return i;
		}
	}

	// REJECT ALPHANUMERIC INPUTS AND INPUTS THAT ARE NOT A VALID MONTH (IN LETTERS)
	
	size_t c = 0; // INITIALIZE A COUNT TO CHECK WHETHER THE NUMBER OF DIGITS IN THE INPUT MATCHES THE LENGTH OF THE INPUT
	for(size_t i = 0; i < strlen(month); i++) {
	       if(isdigit(month[i])) {
		       c = c + 1;
	       }	       
	}

	if( c != strlen(month) ) {
		usage("estimatecron.c");
	}

	int int_month = atoi(month);

	if(int_month < 0 || int_month > 11) {
		usage("estimatecron.c");
	}

	return int_month;
}


int main(int argc, char *argv[])
{
	if(argc != 4) {
		usage(argv[0]);
	}

	int int_month = check_valid_month(argv[1]);
	printf("month is %d\n",int_month);


	printf("\n");
	printf("Running program...\n");
	printf("\n");

	read_file(argv[2],argv[3],int_month);

	return EXIT_SUCCESS;
}
