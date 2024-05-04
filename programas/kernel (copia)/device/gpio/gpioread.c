#include <os.h>


devcall	gpioread(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  char *value,			/* Pointer to a 32bit buffer	*/
	  int32 pinmask			/* Pins from which to read	*/
	)
{

	return OK;
}
