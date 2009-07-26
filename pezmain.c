/*			      PEZMAIN.C

	      Main driver program for interactive PEZ

     Designed and implemented in January of 1990 by John Walker.

		This program is in the public domain.

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <signal.h>
#include "pezdef.h"

#define FALSE	0
#define TRUE	1

/*  Globals imported  */

#ifndef HIGHC

/*  CTRLC  --  Catch a user console break signal.  If your C library
	       does not provide this Unix-compatibile facility
	       (registered with the call on signal() in main()),
	       just turn this code off or, better still, replace it
	       with the equivalent on your system.  */

static void ctrlc(sig)
int sig;
{
	if(sig == SIGINT)
		pez_break();
}
#endif				/* HIGHC */

int print_usage(FILE *s, char *pname)
{
	return fprintf(s,
			"Usage:  %s [options] [input file] [pez args]\n"
			"        Options:\n"
			"           -D     Treat file as definitions\n"
			"           -Hn    Heap length n\n"
			"           -Ifile Include named definition file\n"
			"           -Rn    Return stack length n\n"
			"           -Sn    Stack length n\n"
			"           -T     Set TRACE mode\n"
			"           -U     Print this message\n", 
			pname);
}

static void init_pez_argv(int argc)
{
	int size = sizeof(char *) * argc;
	pez_argv = malloc(size);
	if(!pez_argv) { 
		fprintf(stderr, "Couldn't allocate enough memory to duplicate "
				"argv (%d bytes).\n"
				"Something's real bad wrong.\n", size);
		exit(2);
	}
	memset(pez_argv, 0, size);
}


/*  MAIN  --  Main program.  */

int main(argc, argv)
int argc;
char *argv[];
{
	int i;
	int fname = FALSE, defmode = FALSE;
	int optdone = 0;
	FILE *ifp;
	char *include[20];
	char *cp, opt;
	int in = 0;
	char **pez_argv_current;

	init_pez_argv(argc);
	pez_argv_current = pez_argv;

	ifp = stdin;

	for(i = 1; i < argc; i++) {
		cp = argv[i];
		if(*cp == '-') {
			opt = *(++cp);
			if(islower(opt))
				opt = toupper(opt);
			// TODO:  No more case-insensitive command-line opts,
			// for one, and try to make these a little closer to
			// what users would expect, like -h printing help rather
			// than -u or -?.
			// Aside from that, I suppose we should have a real
			// parser here.
			switch (opt) {

			case 'D':
				defmode = TRUE;
				break;

			case 'H':
				pez_heaplen = atol(cp + 1);
				break;

			case 'I':
				include[in++] = cp + 1;
				break;

			case 'R':
				pez_rstklen = atol(cp + 1);
				break;

			case 'S':
				pez_stklen = atol(cp + 1);
				break;

			case 'T':
				pez_trace = TRUE;
				break;

			case '-':
				optdone = 1;
				break;

			case '?':
			case 'U':
				print_usage(stdout, argv[0]);
				return 0;
			default:
				optdone = 1;
				break;
			}
		} else {
			break;
		}
	}

	// Keeping i from before:
	while(i < argc) {
		cp = argv[i];
		char fn[132];

		if(fname) {
			(*(pez_argv_current++)) = cp;
		} else {
			fname = TRUE;
			strcpy(fn, cp);
			ifp = fopen(fn, "r");
			if(ifp == NULL) {
				fprintf(stderr, "Unable to open file %s\n", fn);
				return 1;
			}
		}
		i++;
	}

	/* If any include files were named, load each in turn before
	   we execute the program. */

	for(i = 0; i < in; i++) {
		int stat;
		char fn[132];
		FILE *fp;

		strcpy(fn, include[i]);
		if(strchr(fn, '.') == NULL)
			strcat(fn, ".pez");
		fp = fopen(fn,
#ifdef FBmode
			   "rb"
#else
			   "r"
#endif
		    );
		if(fp == NULL) {
			fprintf(stderr, "Unable to open include file %s\n",
				  include[i]);
			return 1;
		}
		stat = pez_load(fp);
		fclose(fp);
		if(stat != PEZ_SNORM) {
			printf("\nError %d in include file %s\n", stat,
				 include[i]);
		}
	}

	/* Now that all the preliminaries are out of the way, fall into
	   the main PEZ execution loop. */

#ifndef HIGHC
	signal(SIGINT, ctrlc);
#endif				/* HIGHC */
	while(TRUE) {
		char t[132];

		if(!fname)
			printf(pez_comment ? "(  " :	/* Show pending comment */
				 /* Show compiling state */
				  (((heap != NULL) && state) ? ":> " : "-> "));
		if(fgets(t, 132, ifp) == NULL) {
			if(fname && defmode) {
				fname = defmode = FALSE;
				ifp = stdin;
				continue;
			}
			break;
		}
		pez_eval(t);
	}
	if(!fname)
		printf("\n");
	return 0;
}
