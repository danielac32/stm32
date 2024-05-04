#include "rp2040.h"
#ifndef SYSTICK_H_
#define SYSTICK_H_


void systick_init();
void systick_deinit();
void systick_set(unsigned int value);
unsigned int systick_get();
void systick_enable();
void systick_disable();
#endif