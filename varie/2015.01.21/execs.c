/*
 * s2argv: convert strings to argv
 * Copyright (C) 2014 Renzo Davoli. University of Bologna. <renzo@cs.unibo.it>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "execs.h"

//char *sn[] = { "END", "SPACE", "CHAR", "SGLQ", "DBLQ", "ESCAPE", "SEMIC", "VAR", "ESCVAR", "DBLESC" };
#define END 0
#define SPACE 1
#define CHAR 2
#define SGLQ 3 //single quote
#define DBLQ 4 //double quote
#define ESCAPE 5 // escape '\'
#define SEMIC 6 // semicolon ;
#define VAR 7 // $name
#define ESCVAR 8 // '\' in variable name
#define DBLESC 9 // '\' in double quoted text
#define NSTATES (DBLESC+1)

#define NEWARG 0x1 // beginning of a new argument
#define CHCOPY 0x2 // copy the char in current argv
#define ENDARG 0x4 // end of an argument
#define ENDVAR 0x8 // end of a variable
#define ENDCMD 0x10 // end of a command

/* This is the FSA used to get the lexical items of the command line */

char nextstate[NSTATES][NSTATES-1]= {
	{END,    0,   0,   0,   0,     0,    0,   0}, // END
	{END,SPACE,CHAR,SGLQ,DBLQ,ESCAPE,SEMIC, VAR}, // SPACE
	{END,SPACE,CHAR,SGLQ,DBLQ,ESCAPE,SEMIC,CHAR}, // CHAR
	{END, SGLQ,SGLQ,CHAR,SGLQ,  SGLQ, SGLQ,SGLQ}, // SGLQ
	{END, DBLQ,DBLQ,DBLQ,CHAR,DBLESC, DBLQ,DBLQ}, // DBLQ
	{END, CHAR,CHAR,CHAR,CHAR,  CHAR, CHAR,CHAR}, // ESCAPE
	{END,SEMIC,CHAR,SGLQ,DBLQ,ESCAPE,SEMIC, VAR}, // SEMIC
	{END,SPACE, VAR, VAR, VAR,   VAR,SEMIC, VAR}, // VAR
	{END,  VAR, VAR, VAR, VAR,   VAR,  VAR, VAR}, // ESCVAR
	{END, DBLQ,DBLQ,DBLQ,DBLQ,  DBLQ, DBLQ,DBLQ}}; // DBLESC

char action[NSTATES][NSTATES-1]= {
	{ENDCMD|     0,     0,            0,     0,     0,     0,            0,     0}, //END
	{ENDCMD|     0,     0,NEWARG|CHCOPY,NEWARG,NEWARG,NEWARG,       ENDCMD,NEWARG}, //SPACE
	{ENDCMD|ENDARG,ENDARG,       CHCOPY,     0,     0,     0,ENDCMD|ENDARG,     0}, //CHAR
	{ENDCMD|ENDARG,CHCOPY,       CHCOPY,     0,CHCOPY,CHCOPY,       CHCOPY,CHCOPY}, //SNGQ
	{ENDCMD|ENDARG,CHCOPY,       CHCOPY,CHCOPY,     0,CHCOPY,       CHCOPY,CHCOPY}, //DBLQ
	{ENDCMD|ENDARG,CHCOPY,       CHCOPY,CHCOPY,CHCOPY,CHCOPY,       CHCOPY,CHCOPY}, //ESCAPE
	{ENDCMD|     0,     0,NEWARG|CHCOPY,NEWARG,NEWARG,NEWARG,            0,NEWARG}, //SEMIC
	{ENDCMD|ENDVAR,ENDVAR,       CHCOPY,     0,     0,     0,ENDCMD|ENDVAR,     0}, //VAR
	{ENDCMD|ENDVAR,CHCOPY,       CHCOPY,CHCOPY,CHCOPY,CHCOPY,       CHCOPY,CHCOPY}, //ESCVAR
	{ENDCMD|ENDARG,CHCOPY,       CHCOPY,CHCOPY,CHCOPY,CHCOPY,       CHCOPY,CHCOPY}}; //DBLESC

char *getvar_null(const char *name);
s2argv_getvar_t s2argv_getvar=getvar_null;
int (* execs_fork_security)(void *execs_fork_security_arg);
void *execs_fork_security_arg;

char *getvar_null(const char *name) {
	return "";
}

static int args_fsa(const char *args, char **argv, char *buf)
{
	int state=SPACE;
	int argc=0;
	char *thisarg=NULL;
	for (;state != END;args++) {
		int this;
		switch (*args) {
			case 0:
				this=END;
				break;
			case ' ':
			case '\t':
			case '\n':
				this=SPACE;
				break;
			case '\'':
				this=SGLQ;
				break;
			case '"':
				this=DBLQ;
				break;
			case '\\':
				this=ESCAPE;
				break;
			case ';':
				this=SEMIC;
				break;
			case '$':
				this=VAR;
				break;
			default:
				this=CHAR;
		}
		if (argv) {
			if (action[state][this] & NEWARG)
				thisarg=buf;
			if (action[state][this] & CHCOPY)
				*buf++=*args;
			if (action[state][this] & ENDARG) {
				*buf++=0;
				*argv++=thisarg;
			}
			if (action[state][this] & ENDVAR) {
				*buf++=0;
				if (s2argv_getvar) {
					*argv=s2argv_getvar(thisarg);
					if (*argv == NULL)
						*argv="";
				} else
					*argv="";
				argv++;
			}
			if (action[state][this] & ENDCMD)
				*argv++=0;
		}
		if (action[state][this] & (ENDARG|ENDVAR))
			argc++;
		if (action[state][this] & ENDCMD)
			argc++;
		//printf("%c %s+%s=%s %x\n",*args,sn[state],sn[this],sn[nextstate[state][this]],action[state][this]);
		//printf("%s %d->%d\n",args,state,nextstate[state][this]);
		state=nextstate[state][this];
	}
	if (argv)
		*argv=0;
	return argc;
}

#ifndef EEXECS

char **s2argv(const char *args)
{
	int argc=args_fsa(args,NULL,NULL);
	char buf[strlen(args)+1];
	char **argv=calloc(argc+1,sizeof(char *));
	if (argv) {
		int i;
		args_fsa(args,argv,buf);
		for (i=0; i<argc+1; i++)
			argv[i]=argv[i]?strdup(argv[i]):0;
		/* for (i=0; i<argc+1; i++)
			printf("%d %s\n",i,argv[i]); */
	}
	return argv;
}

void s2argv_free(char **argv)
{
	char **scan;
	for (scan=argv; *scan != (char *)0; scan++) {
		for (; *scan != (char *)0; scan++)
			free(*scan);
	}
	free(argv);
}

size_t s2argvlen(char **argv) {
	int len;
	for (len=0; *argv; len++, argv++)
		for(; *argv; len++, argv++)
			;
	return len;
}

size_t s2argc(char **argv) {
	int argc;
	for (argc=0; *argv; argc++, argv++)
		;
	return argc;
}

int s2multiargv(const char *args, int (*f)(char **argv, void *opaque), void *opaque)
{
	int argc=args_fsa(args,NULL,NULL);
	char *argv[argc+1];
	char buf[strlen(args)+1];
	char **thisargv=argv;
	args_fsa(args,argv,buf);
	int rv=0;
	while (*thisargv && rv==0) {
		rv=f(thisargv, opaque);
		while (*thisargv) thisargv++;
		thisargv++;
	}
	return rv;
}
#endif

int execs_common(const char *path, const char *args, char *const envp[], char *buf)
{
	int argc=args_fsa(args,NULL,NULL);
	char *argv[argc+1];
	args_fsa(args,argv,buf);
	if (path)
		return execve(path, argv, envp);
	else
		return execvpe(argv[0], argv, envp);
}
