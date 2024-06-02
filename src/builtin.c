#include <stdlib.h>
#include <stdio.h>
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"

#define _POSIX2_LINE_MAX 2048

extern char **environ;

int pwdFunc( char arg[] );
int envFunc( char arg[] );
int cdFunc( char arg[] );
int exitFunc( char arg[] );
int typeFunc( char arg[] );

char *cmds[]={
	"exit",
	"cd",
	"pwd",
	"env",
	"type"
};

int ( *cmdsFunc[] )( char arg[] )={
	&exitFunc,
	&cdFunc,
	&pwdFunc,
	&envFunc,
	&typeFunc
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
	exit(EXIT_SUCCESS);
	return 0;
}

int envFunc( char arg[] ){
	for (int i=0; environ[i]!=NULL; i++) {
		printf("%s\n", environ[i]);
	}
	return 0;
}

int typeFunc( char arg[] ){
	char path[PATH_MAX];
	for (int i=0; i<builtinCmdNum; i++) {
		if (strcmp(cmds[i], arg)==0) {
			printf("%s is a shell builtin command\n", cmds[i]);
			return 0;
		}
	}
	getcwd(path, sizeof(path));
	if (getcwd(path, sizeof(path))==NULL) {
		printf("%s is not a command\n", arg);
		return 0;
	}
	printf("%s\n", path);
	return 0;
}

