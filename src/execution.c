#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "builtin.h"
#include "execution.h"

int exec(char **inp);

int exec( char **inp ){
	for (int i=0; i<builtinCmdNum; i++) {
		if (strcmp(inp[0], cmds[i])==0) {
			return ( *cmdsFunc[i] )( inp[1] );
		}
	}
	pid_t p=fork();
	if(p<0){
		perror("Fork failed");
		return 1;
	}
	if(p>0){
		p=wait(NULL);
	}
	if(p==0){
		int h=execvp(inp[0], inp);
		if( h==-1 ){
			perror("Command not found");
			return 1;
		}
		exit(0);
	}
	return 0;
}
