#include <stdlib.h>
#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>

#include "builtin.h"

#define _POSIX2_LINE_MAX 2048

extern char **environ;

int pwdFunc( char arg[] );
int envFunc( char arg[] );
int cdFunc( char arg[] );
int exitFunc( char arg[] );

char *cmds[]={
	"cd",
	"exit",
	"pwd",
	"env"
};

int ( *cmdsFunc[] )( char arg[] )={
	&cdFunc,
	&exitFunc,
	&pwdFunc,
	&envFunc
};

int builtinCmdNum=sizeof(cmds)/sizeof(char*);

int pwdFunc( char arg[] ){
	char pwd[PATH_MAX];
	if(getcwd(pwd, sizeof(pwd))!=NULL){
		printf("%s\n", pwd);
	}else{
		perror("pwd");
		return 1;
	}
	return 0;
}

int cdFunc( char arg[] ){
	if (chdir(arg) == -1) {
		perror("cd");
		return 1;
	}
	return 0;
}

int exitFunc( char arg[] ){
	exit(0);
	return 0;
}

int envFunc( char arg[] ){
	for (int i=0; environ[i]!=NULL; i++) {
		printf("%s\n", environ[i]);
	}
	return 0;
}

