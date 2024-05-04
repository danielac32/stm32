#ifndef __TIMER_H__
#define __TIMER_H__
#include "rp2040.h"

#define NUM_TIMERS 4

typedef struct {
    unsigned int timehw;
    unsigned int timelw;
    unsigned int timehr;
    unsigned int timelr;
    unsigned int alarm[NUM_TIMERS];
    unsigned int armed;
    unsigned int timerawh;
    unsigned int timerawl;
    unsigned int dbgpause;
    unsigned int pause;
    unsigned int intr;
    unsigned int inte;
    unsigned int intf;
    unsigned int ints;
} timer_hw_t;

#define timer_hw ((timer_hw_t *const)TIMER_BASE)

//-----------------------------------------------------------------
// Types
//-----------------------------------------------------------------
typedef unsigned long   t_time;

//-----------------------------------------------------------------
// Prototypes:
//-----------------------------------------------------------------

// General timer
void            timer_init(void);
t_time          timer_now(void);
static long     timer_diff(t_time a, t_time b) { return (long)(a - b); } 
void            timer_sleep(int timeMs);


#endif
