#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "execution.h"

int status=1;
int loop();

int loop(){
	char inp[_POSIX2_LINE_MAX];
	char *inpTok[_POSIX2_LINE_MAX];

	char temp[100];
	int tokenCount=0;

	printf(">");
	fgets(inp, 1024, stdin);
	inp[strcspn(inp, "\n")]='\0';

	char * check=strtok(inp, " ");
	if (inp[0]=='\0' || check==NULL) {return 0;}


	while (check) {
		inpTok[tokenCount++]=check;
		check = strtok(NULL, " ");
	}

	free(check);
	inpTok[tokenCount]=NULL;
	exec(inpTok);
	return 1;
}

int main( int argc, char *argv[] ){
	while (status) {
		loop();
	}
	return 0;
}
