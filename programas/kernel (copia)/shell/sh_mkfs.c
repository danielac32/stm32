/* xsh_memstat.c - xsh_memstat */

#include <os.h>

 
/*------------------------------------------------------------------------
 * xsh_memstat - Print statistics about memory use and dump the free list
 *------------------------------------------------------------------------
 */
shellcmd xsh_mkfs(int nargs, char *args[])
{

	/* For argument '--help', emit help about the 'memstat' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("use: %s \n\n", args[0]);
		printf("Description:\n");
		printf("mkfs 512 100\n");
		printf("mkfs exit\n");
		printf("mkfs status\n");
		return 0;
	}
    
    if(!strcmp(args[1],"exit")){
        delete();
    }else if(!strcmp(args[1],"status")){
        if(active){
           printf("numblocks :%d  , blocksize :%d -> size: %d kb\n", dev0_numblocks,dev0_blocksize,(dev0_numblocks * dev0_blocksize)/1024);
        }else{
           printf("no mount\n");
        }
    }else mkbsdev(0, atoi(args[1]), atoi(args[2])); //int dev, int blocksize, int numblocks
	/* Check for valid number of arguments */

	return 0;
}


