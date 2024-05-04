#include <os.h>


devcall	gpioinit(
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
    printf("gpioinit initialized\n");
	return OK;
}
