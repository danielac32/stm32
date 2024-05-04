static char	*strchr(
	  const char	*s,		/* String to search		*/
	  int		c		/* Character to locate		*/
	)
{
    for (; *s != '\0'; s++)
    {
        if (*s == (const char)c)
        {
            return (char *)s;
        }
    }

    if ((const char)c == *s)
    {
        return (char *)s;
    }

    return 0;
}


int strcspn(const char *string,const char *reject){
int count=0;
while(strchr(reject,*string)==0){
	++count,++string;
}
return count;
}