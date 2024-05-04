#include <string.h>
#include <kernel.h>
int	abs(
	  int		arg
	)
{
    if (arg < 0)
        arg = -arg;
    return (arg);
}



void
strrev(unsigned char *str)
{
    int i;
    int j;
    unsigned char a;
    unsigned len = strlen((const char *)str);
    for (i = 0, j = len - 1; i < j; i++, j--)
    {
        a = str[i];
        str[i] = str[j];
        str[j] = a;
    }
}


char *itoa (int n)
{
    static unsigned char str[12];
    int i = 0;

    int neg = (n < 0);
    if (neg)
        n = -n;

    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (neg)
        str[i++] = '-';

    str[i] = '\0';
    strrev(str);

    return str;
}



int	atoi(
	  char		*p
	)
{
    int n = 0, f = 0;

    for (;; p++)
    {
        switch (*p)
        {
        case ' ':
        case '\t':
            continue;
        case '-':
            f++;
        case '+':
            p++;
        }
        break;
    }

    while (*p >= '0' && *p <= '9')
    {
        n = n * 10 + *p++ - '0';
    }

    return (f ? -n : n);
}


long	atol(
		  char		*p
		)
{
    long n;
    int f;

    n = 0;
    f = 0;
    for (;; p++)
    {
        switch (*p)
        {
        case ' ':
        case '\t':
            continue;
        case '-':
            f++;
        case '+':
            p++;
        }
        break;
    }
    while (*p >= '0' && *p <= '9')
    {
        n = n * 10 + *p++ - '0';
    }
    return (f ? -n : n);
}


void	bzero(
	  void		*p,
	  int		len
	)
{
    int n;
    char *pch = (char *)p;

    if ((n = len) <= 0)
    {
        return;
    }
    do
    {
        *pch++ = 0;
    }
    while (--n);
}

long	labs(
	  long		arg
	)
{
    if (arg < 0)
        arg = -arg;
    return (arg);
}
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


int memcmp(const void *s1, const void *s2, int n)
{
    while (n-- > 0) {
        char c1 = *(const char*)s1++;
        char c2 = *(const char*)s2++;
        if (c1 != c2) {
            return c1 - c2;
        }
    }
    return 0;
}


void	*memcpy(
	  void		*s,	/* Destination address			*/
	  const void	*ct,	/* source address			*/
	  int		n	/* number of bytes to copy		*/
	)
{
    register int i;
    char *dst = (char *)s;
    char *src = (char *)ct;

    for (i = 0; i < n; i++)
    {
        *dst++ = *src++;
    }
    return s;
}

#if 0
void *
memmove (void *dest, const void *src, int len)
{
  char *d = dest;
  const char *s = src;
  if (d < s)
    while (len--)
      *d++ = *s++;
  else
    {
      char *lasts = s + (len-1);
      char *lastd = d + (len-1);
      while (len--)
        *lastd-- = *lasts--;
    }
  return dest;
}
#endif
void	*memset(
	  void		*s,		/* Address of memory block	*/
	  int		c,		/* Byte value to use		*/
	  int		n		/* Size of block in bytes 	*/
	)
{
    register int i;
    char *cp = (char *)s;

    for (i = 0; i < n; i++)
    {
        *cp = (unsigned char)c;
        cp++;
    }
    return s;
}

char	*strchr(
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



int	strcmp(
	  char		*str1,
	  char		*str2
	)
{
	while (*str1 == *str2) {
		if (*str1 == '\0') {
	            return 0;
        	}
		str1++;
		str2++;
	}
	if (*str1 < *str2) {
		return -1;
	} else {
		return  1;
	}
}

int	strcpy(
	  char		*tar,		/* target string		*/
	  char		*src		/* source string		*/
	)
{
	while ( (*tar++ = *src++) != '\0') {
		;
	}
	return 0;
}


int	strncmp(
	  const char		*s1,		/* First memory location	*/
	  const char		*s2,		/* Second memory location	*/
	  int		n		/* Length to compare		*/
	)
{

    while (--n >= 0 && *s1 == *s2++)
    {
        if (*s1++ == '\0')
        {
            return 0;
        }
    }
    return (n < 0 ? 0 : *s1 - *--s2);
}


char	*strncpy(
	  char		*s1,		/* First string			*/
	  const char	*s2,		/* Second string		*/
	  int		n		/* Length  to copy		*/
			)
{
    register int i;
    register char *os1;

    os1 = s1;
    for (i = 0; i < n; i++)
    {
        if (((*s1++) = (*s2++)) == '\0')
        {
            while (++i < n)
            {
                *s1++ = '\0';
            }
            return os1;
        }
    }
    return os1;
}

char	*strncat(
	  char		*s1,		/* first string			*/
	  const char	*s2,		/* second string		*/
	  int		n		/* length to concatenate	*/
	)
{
    char *os1;

    os1 = s1;
    while (*s1++)
        ;
    --s1;
    while ((*s1++ = *s2++))
        if (--n < 0)
        {
            *--s1 = '\0';
            break;
        }
    return (os1);
}

int	strlen(
	  const char		*str		/* string to use		*/
	)
{
	int	len;

	len = 0;

	while(*str++ != '\0') {
		len++;
	}
	return  len;
}

 
int	strnlen(
	  const char	*s,		/* string			*/
	   int	len		/* max length			*/
		)
{
    int n;

    n = 0;
    while (*s++ && n < len)
        n++;

    return (n);
}

char	*strrchr(
	  const char	*s,		/* String to search		*/
	  int		c		/* Character to locate		*/
	)
{
    char *r = 0;

    for (; *s != '\0'; s++)
    {
        if (*s == (const char)c)
        {
            r = (char *)s;
        }
    }

    if ((const char)c == *s)
    {
        return (char *)s;
    }

    return r;
}

char	*strstr(
	  const char	*cs,		/* String to search		*/
	  const char	*ct		/* Substring to locate		*/
	)
{
    char *cq;
    char *cr;

    for (; *cs != '\0'; cs++)
    {
        if (*cs == *ct)
        {
            cq = (char *)cs;
            cr = (char *)ct;
            while ((*cq != '\0') && (*cr != '\0'))
            {
                if (*cq != *cr)
                {
                    break;
                }
                cq++;
                cr++;
            }
            if ('\0' == *cr)
            {
                return (char *)cs;
            }
        }
    }
    return 0;
}




char * strcat(char *dest, char *src)
{
	int offset;

	offset = strlen(dest);
	strcpy(dest+offset, src);
	return dest;
}

int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        c += 'a' - 'A';
    }
    return c;
}

int toupper(int c)
{
    if (c >= 'a' && c <= 'z')
    {
        c += 'A' - 'a';
    }
    return c;
}



 float pow(float base,float ex){
  if(ex==0){
    return 1;
  }else if(ex<0){
    return 1/pow(base,-ex);
  }else if((int)ex %2 ==0){
    float half__pow_=pow(base,ex/2);
    return half__pow_ * half__pow_;
  }else{
    return base * pow(base,ex-1);
  }
}




char * strtok(char *s,const char* delim)
{
     char *spanp;
     int c, sc;
    char *tok;
    static char *last;


    if (s == NULL && (s = last) == NULL)
        return (NULL);

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = *s++;
    for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto cont;
    }

    if (c == 0) {       /* no non-delimiter characters */
        last = NULL;
        return (NULL);
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;) {
        c = *s++;
        spanp = (char *)delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                last = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}

/*
float atof(const char* string)
{
    float sign;
    float value;
    int   c;

    // skip whitespace
    while (*string <= ' ')
    {
        if (!*string)
        {
            return 0;
        }
        string++;
    }

    // check sign
    switch (*string)
    {
    case '+':
        string++;
        sign = 1;
        break;
    case '-':
        string++;
        sign = -1;
        break;
    default:
        sign = 1;
        break;
    }

    // read digits
    value = 0;
    c     = string[0];
    if (c != '.')
    {
        do
        {
            c = *string++;
            if (c < '0' || c > '9')
            {
                break;
            }
            c -= '0';
            value = value * 10 + c;
        } while (1);
    }
    else
    {
        string++;
    }

    // check for decimal point
    if (c == '.')
    {
        float fraction;

        fraction = 0.1;
        do
        {
            c = *string++;
            if (c < '0' || c > '9')
            {
                break;
            }
            c -= '0';
            value += c * fraction;
            fraction *= 0.1;
        } while (1);
    }

    // not handling 10e10 notation...

    return value * sign;
}
*/


int
strcspn(s1, s2)
    const char *s1;
    register const char *s2;
{
    register const char *p, *spanp;
    register char c, sc;

    /*
     * Stop as soon as we find any character from s2.  Note that there
     * must be a NUL in s2; it suffices to stop when we find that, too.
     */
    for (p = s1;;) {
        c = *p++;
        spanp = s2;
        do {
            if ((sc = *spanp++) == c)
                return (p - 1 - s1);
        } while (sc != 0);
    }
    /* NOTREACHED */
}















