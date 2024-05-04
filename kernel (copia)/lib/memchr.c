


void *memchr(const void *cs, int c, int n)
{
    char *cp = (char *)cs;

    for (; *cp != '\0'; cp++)
    {
        if (*cp == (unsigned char)c)
        {
            return (void *)cp;
        }
    }
    return 0;
}
