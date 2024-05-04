
#include "os.h"
char *
strcat(char *dest, const char *src)
{
	size_t offset;

	offset = strlen(dest);
	strcpy(dest+offset, src);
	return dest;
}
