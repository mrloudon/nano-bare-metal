#ifndef sysclock_h
#define sysclock_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

uint32_t millis(void);
void init_sysclock_2(void);

#endif