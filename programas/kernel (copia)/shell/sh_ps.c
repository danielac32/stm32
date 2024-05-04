/* xsh_ps.c - xsh_ps */

#include <os.h>

/*------------------------------------------------------------------------
 * xsh_ps - shell command to print the process table
 *------------------------------------------------------------------------
 */
shellcmd xsh_ps(int nargs, char *args[])
{

int32	i;			/* walks through args array	*/

        /* check all NPROC slots */

	printf("\ntable of current processes\n");
	printf("\nname\tid\tstate\tstack base\tstack size\n--\n");
        for (i = 0; i < MAX_NUMBER_OF_TASKS; i++) {
		if (task[i].state == PR_FREE)
			continue;
		printf("%s\t%d", task[i].name, i);
		printf("\t%d ", task[i].state);
		printf("\t0x%08x ", task[i].prstkbase);
		printf("\t%d ", task[i].prstklen);
		printf("\n");
	}

	return 0;
}
