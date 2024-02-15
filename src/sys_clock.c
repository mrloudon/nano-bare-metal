#include "sys_clock.h"
#include <util/atomic.h>
#include <avr/interrupt.h>

volatile uint32_t timer_2_count = 0;

ISR (TIMER2_COMPA_vect)      
{
    timer_2_count++;
}

void init_sysclock_2 (void)          
{
    // Initialize timer 2 for debouncing buttons and soft-defined reset
    // TCCR2A [ COM2A1 COM2A0 COM2B1 COM2B0 0 0 WGM21 WGM20 ] = 00000001
    // WGM22 WGM20 => PWM, Phase Correct, TOP = OCRA
    // TCCR2B [ FOC2A FOC2B 0 0 WGM22 CS22 CS21 CS20 ] = 00001011
    // CS22 CS20 => scalar of 32
    // Frequency = 16 x 10^6 / 32 / 255 = 2000Hz
    // Counter performs another divide by 2 => 1000hz
    // Test using example/millis (delay(1000) = 999 ticks)
    // OCR2A value of 255 results in 999
    TCCR2A |= (_BV(WGM20));
    TCCR2B |= ( _BV(WGM22) | _BV(CS21) | _BV(CS20) ) ;
    OCR2A = 255;
    TIMSK2 |= _BV(OCIE2A);
    sei ();
}

uint32_t millis(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        return(timer_2_count);
    }
    return 0;   
}