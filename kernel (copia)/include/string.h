/* string.h */

extern	char	*strncpy(char *, const char *, int32);
extern	char	*strncat(char *, const char *, int32);
extern	int32	strncmp(const char *, const char *, int32);
extern	char	*strchr(const char *, int32);
extern	char	*strrchr(const char *, int32);
extern	char	*strstr(const char *, const char *);
extern	int32	strnlen(const char *, uint32);
extern	int	strlen(char *str);
extern int	strcmp(char	*,char *);
extern int	strcpy(char *,char *);
extern char * strcat(char *, char *);
extern void *memmove (void *, const void *, int);
extern void	*memset(void*,int,int);