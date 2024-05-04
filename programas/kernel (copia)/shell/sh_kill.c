/* xsh_kill.c - xsh_kill */

#include <os.h>

/*------------------------------------------------------------------------
 * xsh_kill - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_kill(int nargs, char *args[]) {

	int32	retval;			/* return value			*/
	int	pid;			/* ID of process to kill	*/
	char	ch;			/* next character of argument	*/
	char	*chptr;			/* walks along argument string	*/

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s PID\n\n", args[0]);
		printf("Description:\n");
		printf("\tterminates a process\n");
		printf("Options:\n");
		printf("\tPID \tthe ID of a process to terminate\n");
		printf("\t--help\tdisplay this help and exit\n");
		return OK;
	}

	/* Check argument count */

	if (nargs != 2) {
		printf( "%s: incorrect argument\n", args[0]);
		printf( "Try '%s --help' for more information\n",
			args[0]);
		return SYSERR;
	}

	/* compute process ID from argument string */

	chptr = args[1];
	pid = atoi(chptr);

	//retval = 
	kill(pid);
	
/*	if (retval == SYSERR) {
		printf( "%s: cannot kill process %d\n",
			args[0], pid);
		return 1;
	}*/
	return 0;
}
