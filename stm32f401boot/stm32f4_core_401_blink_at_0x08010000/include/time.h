 
#ifndef _TIME_H
#define _TIME_H

#include <stddef.h>

// Definiciones de tipos de datos
typedef /* implementación específica */ time_t;
typedef /* implementación específica */ clock_t;
struct tm {
    int tm_sec;    // Segundos (0-61)
    int tm_min;    // Minutos (0-59)
    int tm_hour;   // Horas (0-23)
    int tm_mday;   // Día del mes (1-31)
    int tm_mon;    // Mes (0-11)
    int tm_year;   // Año - 1900
    int tm_wday;   // Día de la semana (0-6, Domingo=0)
    int tm_yday;   // Día del año (0-365)
    int tm_isdst;  // Horario de verano activado (0,1,-1)
};

// Constantes
#define CLOCKS_PER_SEC 10/* número de pulsaciones del reloj por segundo */

// Funciones
time_t time(time_t *t);
clock_t clock(void);
double difftime(time_t time1, time_t time0);
time_t mktime(struct tm *timeptr);
size_t strftime(char *restrict s, size_t maxsize, const char *restrict format, const struct tm *restrict timeptr);
struct tm *gmtime(const time_t *timep);
struct tm *localtime(const time_t *timep);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timep);

#endif /* _TIME_H */
 