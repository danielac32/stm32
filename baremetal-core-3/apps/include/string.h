/* string.h */

extern	char	*strncpy(char *, const char *, int);
extern	char	*strncat(char *, const char *, int);
extern	int	strncmp( char *,  char *, int);
extern	char	*strchr(const char *, int);
extern	char	*strrchr(const char *, int);
extern	char	*strstr(const char *, const char *);
extern	int	strnlen(const char *,unsigned int);
extern	int	strlen(const char *str);
extern int	strcmp(char	*,char *);
extern int	strcpy(char *,char *);
extern char * strcat(char *, char *);
extern void *memmove (void *, const void *, int);
extern void	*memset(void*,int,int);
extern void strrev(unsigned char *str);
extern void *memchr(const void *cs, int c, int n);
extern int	memcmp(
	  const void	*s1,		/* first memory location	*/
	  const void	*s2,		/* second memory location	*/
	  int		n		/* length to compare		*/
	);
extern void	*memcpy(
	  void		*s,	/* Destination address			*/
	  const void	*ct,	/* source address			*/
	  int		n	/* number of bytes to copy		*/
	);
