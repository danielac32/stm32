#define RAND_MAX 2147483646

extern int abs(int);
extern long labs(long);
extern int atoi(char *);
extern long atol(char *);
extern void bzero(void *, int);
extern void qsort(char *, unsigned int, int, int (*)(void));
extern int rand(void);
extern void srand(unsigned int);
extern void *malloc( int nbytes);
extern void free(void *pmem);
