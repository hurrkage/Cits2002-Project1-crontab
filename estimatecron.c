//  CITS2002 Project 1 2022
//  Student1:   23102622   Cura   Jericho
//  Student2:   23286189   Chin   Aaron

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

//GLOBAL VARIABLES

#define BUFFERSIZE 10000
#define YEAR 2022

int total_commands_ran = 0;
int nrunning = 0;
int max_nrunning = 0;
int max_command_value = 0;
char max_command_value_name[40];


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
        
	char valid_days[7][50] = {"sun","mon","tue","wed","thu","fri","sat"};

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

int check_duration(char duration[]) {
	
	for(size_t i = 0; i < strlen(duration); i++) {
		if(!isdigit(duration[i])) {
			fprintf(stderr,"ERROR: %s(duration) in the given estimates file must be an integer\n",duration);
			exit(EXIT_FAILURE);
	        }
	}

        int int_duration = atoi(duration);

	if(int_duration <= 0) {
		fprintf(stderr,"ERROR: duration given in estimates file must be a non-zero positive integer\n");
		exit(EXIT_FAILURE);
	}
	return int_duration;


}
// CALCULATE THE FIRST DAY OF THE MONTH, CODE BORROWED FROM CHRIS MCDONALD
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

// STRUCT TO STORE THE NECCESSARY DATA FOR THE TIMER FUCNTION TO READ
struct command_args {
	int min;
	int hour;
	int date;
	int month;
	int day;
	char command_name[40];
	int duration;
	int counter;
};
	
struct command_args command_lines[20];

int command_count = 0; //NO. UNIQUE COMMANDS RUNNING DURING THE GIVEN MONTH


// READ CRONTAB AND ESTIMATE FILE
void read_file(char crontab[], char estimates[],int input_month)
{
	// READ CRONTAB
	FILE *fp0 = fopen(crontab,"r");
        char line0[BUFFERSIZE];

	if(fp0 == NULL) {
		fprintf(stderr,"File: %s cannot be opened.\n%s does not exist?, %s is empty?\n",crontab,crontab,crontab);
			exit(EXIT_FAILURE);
	} 

	int non_comment_line_count = 0;

	// PROCESS COMMAND LINES
        while(fgets(line0,sizeof(line0),fp0) != NULL) {

		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES)
		if(line0[0] != '#') {
			non_comment_line_count = non_comment_line_count + 1;

                        if(non_comment_line_count > 20) {
			        fprintf(stderr,"Error: Too many non-comment lines in %s\n",crontab);
			        exit(EXIT_FAILURE);
			}
		}
		// FILE DEBUGGING (CHECK FOR NO. OF CHARACTERS IN A LINE)
	        if(strlen(line0) > 100) {
			fprintf(stderr,"Error: Too many characters in a line in %s\n",crontab);
			exit(EXIT_FAILURE);
		}

		// READ COMMAND LINES AND STORE COMMAND ARGS ONTO STRUCT COMMAND_ARGS
		if(line0[0] != '#') {
			char min[5], hour[5], date[5], month[5], day[5], command_name[40];

			sscanf(line0,"%s %s %s %s %s %s", min, hour, date, month, day, command_name);

			int cron_min = check_min(min);
			int cron_hour = check_hour(hour);
			int cron_date = check_date(date);
			int cron_month = check_month(month);
			int cron_day = check_day(day);

                        if(cron_month == input_month || cron_month == 100) {
				command_lines[command_count].min = cron_min;
				command_lines[command_count].hour = cron_hour;
				command_lines[command_count].date = cron_date;
				command_lines[command_count].month = cron_month;
				command_lines[command_count].day = cron_day;
				strcpy(command_lines[command_count].command_name, command_name);

				command_count = command_count + 1;
			}
		}
	}
	
	fclose(fp0);

	// READ ESTIMATES FILE
	FILE *fp1 = fopen(estimates,"r");
        char line1[BUFFERSIZE];

	if(fp1 == NULL) {
		fprintf(stderr,"File: %s cannot be opened.\n%s does not exist?, %s is empty?\n",estimates,estimates,estimates);
		exit(EXIT_FAILURE);
	}
        
        int non_comment_line_count2 = 0;
	// PROCESS COMMAND LINES
	while(fgets(line1,sizeof(line1),fp1) != NULL) {
		
		// FILE DEBUGGING (CHECK FOR NO. OF COMMENT LINES) 	
		if(line1[0] != '#') {
			non_comment_line_count2 = non_comment_line_count2 + 1;

                        if(non_comment_line_count2 > 20) {
			        fprintf(stderr,"Error: Too many non-comment lines in %s\n",estimates);
			        exit(EXIT_FAILURE);
			}
		}
		// FILE DEBUGGING (CHECK FOR NO. OF CHARACTERS IN A LINE)
	        if(strlen(line1) > 100) {
			fprintf(stderr,"Error: Too many characters in a line in %s\n",estimates);
			exit(EXIT_FAILURE);
		}
		// READ COMMAND LINES AND STORE COMMAND DURATION ONTO THE STRUCT COMMAND_ARGS
		if(line1[0] != '#') {
			char command_name[40], duration[5];
			sscanf(line1,"%s %s", command_name, duration);

			int est_duration = check_duration(duration);

			for(int i = 0; i < command_count; i++) {
				if(strcmp(command_name,command_lines[i].command_name) == 0) {
					command_lines[i].duration = est_duration;
				}
			}
		}
	}



	fclose(fp1);
}


// TEST WHETHER THE GIVEN COMMAND IS GETTING INVOKED DURING THE CURRENT TIME
bool invoke(int cron_min,int cron_hour,int cron_date,int cron_day,int timer_min,int timer_hour,
	int timer_date,int timer_day) {

	bool result = false;
	if(cron_min == timer_min || cron_min == 100) {
		if(cron_hour == timer_hour || cron_hour == 100) {
			if(cron_date == timer_date || cron_date == 100) {
				if(cron_day == timer_day || cron_day == 100) {
					result = true;
				}
			}
		}
	}
	return result;
}

// TEST WHETHER THE RUNNING COMMAND GETS TERMINATED DURING THE CURRENT TIME
bool terminate(int term_min,int term_hour,int term_date,int timer_min,int timer_hour,int timer_date) {

	bool result = false;
	if(term_min == timer_min) {
		if(term_hour == timer_hour) {
			if(term_date == timer_date) {
				result = true;
			}
		}
	}
	return result;
}



// STRUCT TO STORE THE TERMINATION TIME OF EACH INVOKED PROCESSES
struct terminate_time {
	int term_min;
	int term_hour;
	int term_date;
	char command_name[40];
};

struct terminate_time terminate_times[1000];
int terminate_size_count = 0;



// START A TIMER COUTING MIN BY MIN THROUGH OUT THE MONTH
void timer(int input_month) {

	// COUNT NUM OF DAYS IN THE GIVEN MONTH
	int days_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

	int days = 0;
	for(int i = 0; i < 12; i++) {
		if(input_month == i) {
			days = days_month[i];
		}
	}
	int weekday = first_day_of_month(input_month + 1,YEAR);
	int mins_in_month = 1440 * days;


	//STARTS COUNTING MIN BY MIN
	for(int i = 0; i < mins_in_month; i++) {
		int hr = i/60 - 24*(i/1440);
		int min = i - 60*hr - 24*60*(i/1440);
		int date = i/1440 + 1;

                // TEST EACH COMMAND NAME AND SEE IF IT GETS INVOKED AT THAT TIME
		for(int i = 0; i < command_count; i++) {
			if(invoke(command_lines[i].min,command_lines[i].hour,command_lines[i].date,
					command_lines[i].day,min,hr,date,weekday)) { 
	 	        	nrunning++;
				total_commands_ran++;
				command_lines[i].counter++;

				//RECORDS THE NUMBER OF TIMES THE COMMAND HAS BEEN INVOKED
				
				//CALCULATE THE TIME WHEN THE INVOKED COMMANDS ENDS AND STORE IT INTO A STRUCT ARRAY
  	 	  	        int time_of_month_in_mins = 60*hr + (date-1)*24*60 + min;	
				int termi_time_in_mins    = time_of_month_in_mins + command_lines[i].duration;

				int term_hr = termi_time_in_mins/60 - 24*(termi_time_in_mins/1440);
				int term_min = termi_time_in_mins - 60*term_hr - 24*60*(termi_time_in_mins/1440);
				int term_date = termi_time_in_mins/1440 + 1;

				terminate_times[terminate_size_count].term_min = term_min;
				terminate_times[terminate_size_count].term_hour = term_hr;
				terminate_times[terminate_size_count].term_date = term_date;
				strcpy(terminate_times[terminate_size_count].command_name, command_lines[i].command_name);

				terminate_size_count++;

				// STORES THE MAXIMUM NUMBER OF COMMANDS RUNNING SIMULTANEOUES
				if(nrunning > max_nrunning && max_nrunning < 20) {
					max_nrunning = nrunning;
				}
			}


			if(command_lines[i].counter > max_command_value) {
           			 max_command_value = command_lines[i].counter;
		                 strcpy(max_command_value_name, command_lines[i].command_name);
			}
		}	
         
		for(int i = 0; i < terminate_size_count; i++) {
			if(terminate(terminate_times[i].term_min,terminate_times[i].term_hour,terminate_times[i].term_date,min,hr,date)) { 
				nrunning--;

			}
		}

		if(min == 59 && hr == 23) {
			weekday = weekday + 1;
			if(weekday > 6) {
				weekday = 0;
			}
		}		
	}
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

	int int_month = check_valid_month(argv[1]);  // Jan = 0, Feb = 1, Mar = 2,..... , Dec = 11

	read_file(argv[2],argv[3],int_month);

	timer(int_month);

	printf("%s %d %d\n",max_command_value_name,total_commands_ran,max_nrunning);


	return EXIT_SUCCESS;
}
