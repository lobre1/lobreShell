#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "builtin.h"
#include "execution.h"

typedef struct cmdSplit{
	char *left[_POSIX2_LINE_MAX];
	char *right[_POSIX2_LINE_MAX];
}cmdDecompo;

int exec(char **inp);

cmdDecompo splitter( char **inp, int opPos, int tokenCount ){
	cmdDecompo cmdSplit={0};
	int i=0, j=0;
	while (i<opPos) {
		cmdSplit.left[i]=inp[i];
		i++;
	}
	cmdSplit.left[i]=NULL;
	i++;
	while (i<tokenCount){
		cmdSplit.right[j++]=inp[i];
		i++;
	}
	cmdSplit.right[j]=NULL;
	return cmdSplit;
}

int execLogic( char **inp, int tokenCount ){
	int opPos=0;

	bool isAnd=false;
	bool isOr=false;
	bool isPipe=false;
	bool isExAnd=false;

	for (int i=0; i<tokenCount; i++) {

		if ( strcmp(inp[i], "&&")==0 ){
			isAnd=true;
			opPos=i;
			break;
		}
		if ( strcmp(inp[i], "||")==0 ){
			isOr=true;
			opPos=i;
			break;
		}
		if ( strcmp(inp[i], "|")==0 ){ 
			isPipe=true;
			opPos=i;
			break;
		}
		if ( strcmp(inp[i], ";")==0 ){ 
			isExAnd=true;
			opPos=i;
			break;
		}
	}
	cmdDecompo splitCmd=splitter(inp, opPos, tokenCount);
	if (isAnd==true) {
		int lstat=execLogic(splitCmd.left, opPos);
		if (lstat==0){
			int rstat=execLogic(splitCmd.right, tokenCount-opPos-1);
			return rstat;
		}else{
			return lstat;
		}
	}
	if (isOr==true) {
		int lstat=execLogic(splitCmd.left, opPos);
		if (lstat!=0) {
			int rstat = execLogic(splitCmd.right, tokenCount - opPos - 1);
			return rstat;
		}else{
			return lstat;
		}
	}
	if (isExAnd==true) {
		int lstat=execLogic(splitCmd.left, opPos);
		int rstat=execLogic(splitCmd.left, opPos);
		return lstat && rstat;
	}
	if (isAnd==false && isOr==false && isPipe==false) {
		int ret=exec(inp);
		return ret;
	}
	return 1;
}

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
		return 0;
	}
	if(p==0){
		int h=execvp(inp[0], inp);
		if( h==-1 ){
			perror("Command not found");
			return 1;
		}
		exit(0);
	}
	return 1;
}
