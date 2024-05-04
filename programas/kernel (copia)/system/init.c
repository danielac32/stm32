#include <os.h>

syscall init(
      did32     descrp      /* Descriptor for device    */
    )
{
    intmask     mask;       /* Saved interrupt mask     */
    struct dentry   *devptr;    /* Entry in device switch table */
    int32       retval;     /* Value to return to caller    */

    mask = disable();
    
    devptr = (struct dentry *) &devtab[descrp];
    retval = (*devptr->dvinit) (devptr);
    restore(mask);
    return retval;
}