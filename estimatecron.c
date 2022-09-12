// Cits2002 Project 1 2022
//  Student1:   23102622   Cura   Jericho
//  Student2:   Chin   Chin  Aaron


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
int main()
{
	FILE* fPointer;
	char ch;
	fPointer = fopen("estimates-file.txt","r");

	while((ch = fgetc(fPointer)) != EOF){
		printf("%c",ch);
	}
	fclose(fPointer);
	return 0;
} 