#include <os.h>


devcall	ttyinit(
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
    printf("ttyinit initialized\n");
	return OK;
}
