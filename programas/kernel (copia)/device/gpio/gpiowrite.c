#include <os.h>


devcall	gpiowrite(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char *value,			/* Pointer to 32-bit buffer	*/
	  int32 pin			/* Pins to be written written	*/
	)
{

	return OK;
}
