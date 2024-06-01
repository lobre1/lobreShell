#ifndef BUILTIN_H

#define _POSIX2_LINE_MAX 2048

extern char *cmds[];
extern int ( *cmdsFunc[] )( char arg[] );

extern int builtinCmdNum;

#endif
