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

#ifndef S2ARGV_H
#define S2ARGV_H
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/* This header file declares all the functions defined in
	 the libexecs and libeexecs libraries.  
	 libeexecs is a minimal subset of the libexecs library designed 
	 for embedded systems with strict memory requirements. 
	 It implements only the execs* functions. Programs using libexecs
	 can also use the esystem (a.k.a. system_eexecsp) inline function */

/* fork security: if defined this function gets called for each child process created
	 by this library. If it returns a non zero value it fails and exec is aborted */
/* it can be used to drop privileges such as capabilities */
extern int (* execs_fork_security)(void *execs_fork_security_arg);
extern void *execs_fork_security_arg;

/***************** library functions defined both in libexecs and in libeexecs ********/

/* execs is like execv: argv is computed by parsing args */
/* execsp is like execvp: argv is computed by parsing args,
	 argv[0] is the executable file to be searched for along $PATH */
/* execse and execspe permit the specification of the environment 
	 (as in execve or execvpe) */
/* execs, execse, execsp and execspe do not require dynamic allocation *but*
	 require an extra copy of args on the stack */
/* in all eexecs* functions, the string args is modified 
	 (no extra copies on the stack, args is parsed on itself): */
int execs_common(const char *path, const char *args, char *const envp[], char *buf);

static inline int execse(const char *path, const char *args, char *const envp[]) {
	char buf[strlen(args)+1]; 
	return execs_common(path, args, envp, buf);
}

#define execs(path, args) execse((path),(args),environ)
#define execsp(args) execse(NULL,(args),environ)
#define execspe(args,env) execse(NULL,(args),(env))

#define eexecs(path, args) execs_common((path),(args),environ,(args))
#define eexecse(path, args, env) execs_common((path),(args),(env),(args))
#define eexecsp(args) execs_common(NULL,(args),environ,(args))
#define eexecspe(args,env) execs_common(NULL,(args),(env),(args))

static inline int system_eexecsp(const char *command) {
	int status;
	pid_t pid;
	switch (pid=fork()) {
		case -1:
			return -1;
		case 0:
			if (__builtin_expect(execs_fork_security == NULL || execs_fork_security(execs_fork_security_arg) == 0, 1))
				execs_common(NULL, (char *) command, environ, (char *) command);
			_exit(127);
		default:
			waitpid(pid,&status,0);
			return status;
	}
}

#define esystem(cmd) system_eexecsp(cmd)

/******** library functions defined in libexecs only (not in libeexec) ********/

/* system_nosh is an "almost" drop in replacement for system(3).
	 it does not start a shell but it parses the arguments and
	 runs the command */
/* system_execs is similar to system_nosh but instead of searching the
	 executable file along the directories listed in $PATH it starts
	 the program whose path has been passed as its first arg. */
int system_execsr(const char *path, const char *command, int redir[3]);

#define system_nosh(cmd) system_execsr(NULL,(cmd),NULL)

#define system_execsrp(cmd,redir) system_execsr(NULL,(cmd),(redir))
#define system_execsra(cmd,redir) system_execsr("",(cmd),(redir))
#define system_execs(path,cmd) system_execsr((path),(cmd),NULL)
#define system_execsp(cmd) system_execsr(NULL,(cmd),NULL)
#define system_execsa(cmd) system_execsr("",(cmd),NULL)

/* popen_nosh is an "almost" drop in replacement for popen(3),
	 and pclose_nosh is its counterpart for pclose(3). */
/* popen_execs/pclose_execs do not use $PATH to search the executable file*/
FILE *popen_execs(const char *path, const char *command, const char *type);
int pclose_execs(FILE *stream);

#define popen_nosh(cmd, type) popen_execs(NULL, (cmd), (type))
#define pclose_nosh(stream) pclose_execs(stream)

#define popen_execsp(cmd, type) popen_execs(NULL, (cmd), (type))
#define pclose_execsp(stream) pclose_execs(stream)

/* run a command in coprocessing mode */
pid_t coprocess_common(const char *path, const char *command,
		char *const argv[], char *const envp[], int pipefd[2]);

#define coprocv(path, argv, pfd) coprocess_common((path),NULL,(argv), environ, pfd)
#define coprocve(path, argv, env, pfd) coprocess_common((path),NULL,(argv), (env), pfd)
#define coprocvp(file, argv, pfd) coprocess_common(NULL,(file),(argv), environ, pfd)
#define coprocvpe(file, argv, env, pfd) coprocess_common(NULL,(file),(argv), (env), pfd)

#define coprocs(path, cmd, pfd) coprocess_common((path),(cmd),NULL, environ, pfd)
#define coprocse(path, cmd, env, pfd) coprocess_common((path),(cmd),NULL, (env), pfd)
#define coprocsp(cmd, pfd) coprocess_common(NULL,(cmd),NULL, environ, pfd)
#define coprocspe(cmd, env, pfd) coprocess_common(NULL,(cmd),NULL, (env), pfd)

/* Low level argc management functions */

/* s2argv parses args. 
	 It allocates, initializes and returns an argv array, ready for execv. 
	 s2argv is able to parse several commands separated by semicolons (;).
	 The return value is the sequence of all the corresponding argv
	 (each one has a NULL element as its terminator) and one further
	 NULL element terminates the whole sequence.
	 (i.e. this multi-argv has two NULLs in a row at its end).
	 This format is compatible with the standard argv.
 */
char **s2argv(const char *args);

/* s2argv_free deallocates an argv returned by s2argv */
void s2argv_free(char **argv);

/* number of elements of argv */
size_t s2argvlen(char **argv);

/* argc of the (first) command */
/* argv=argv+s2argc(argv)+1 is the next argv */
size_t s2argc(char **argv);

/* var definition function (e.g. s2argv_getvar=getenv)*/
typedef char * (* s2argv_getvar_t) (const char *name);
extern s2argv_getvar_t s2argv_getvar;
/* getvar_null is the deafult value for s2argv_getvar,
	 it always returns an empty string for any variable name */
char *getvar_null(const char *name);


/* multi argv. Args can contain several commands semicolon (;) separated.
	 This function parses args and calls f for each command/argv in args.
	 If f returns 0 s2multiargv calls f for the following argv, otherwise
	 returns the non-zero value. 
	*/
int s2multiargv(const char *args, int (*f)(char **argv, void *opaque), void *opaque);

#endif
