#include <time.h>


time_t time(time_t *t) {
    if (t != NULL) {
        *t = (time_t)NULL; // Asigna NULL al valor apuntado por t
    }
    return (time_t)NULL; // Devuelve NULL
}

clock_t clock(void) {
    return (clock_t)NULL;
}

double difftime(time_t time1, time_t time0) {
    return (double)0;
}

time_t mktime(struct tm *timeptr) {
    return (time_t)NULL;
}

size_t strftime(char *restrict s, size_t maxsize, const char *restrict format, const struct tm *restrict timeptr) {
    return (size_t)NULL;
}

struct tm *gmtime(const time_t *timep) {
    return (struct tm *)NULL;
}

struct tm *localtime(const time_t *timep) {
    return (struct tm *)NULL;
}

char *asctime(const struct tm *timeptr) {
    return (char *)NULL;
}

char *ctime(const time_t *timep) {
    return (char *)NULL;
}