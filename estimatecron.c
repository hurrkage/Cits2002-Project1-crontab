#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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