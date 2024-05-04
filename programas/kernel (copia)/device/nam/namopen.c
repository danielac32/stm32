#include <os.h>


devcall	namopen(
	  struct dentry *devptr,	/* Entry in device switch table */
	  char	*name,			/* Name to open			*/
	  char	*mode			/* Mode argument		*/
	)
{

	return OK;
}
