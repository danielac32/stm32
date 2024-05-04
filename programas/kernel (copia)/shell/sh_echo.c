/* xsh_argecho.c - xsh_argecho */

#include <os.h>

/*------------------------------------------------------------------------
 * xhs_argecho - display argecho message that lists shell commands
 *------------------------------------------------------------------------
 */
shellcmd xsh_echo(int nargs, char *args[])
{
	int32	i;			/* walks through args array	*/

	if (nargs > 1) {
		printf("%s", args[1]);

		for (i = 2; i < nargs; i++) {
			printf(" %s", args[i]);
		}
	}
	printf("\n");
	return 0;
}
