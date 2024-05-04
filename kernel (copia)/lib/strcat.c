


static int	__strlen(
	  char		*str		/* string to use		*/
	)
{
	int	len;

	len = 0;

	while(*str++ != '\0') {
		len++;
	}
	return  len;
}


static int	__strcpy(
	  char		*tar,		/* target string		*/
	  char		*src		/* source string		*/
	)
{
	while ( (*tar++ = *src++) != '\0') {
		;
	}
	return 0;
}


char * strcat(char *dest, char *src)
{
	int offset;

	offset = __strlen(dest);
	__strcpy(dest+offset, src);
	return dest;
}
