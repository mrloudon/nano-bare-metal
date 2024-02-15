#include <avr/io.h>
// #include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "twi_master.h"
#include "liquid_crystal_i2c.h"
#include "sys_clock.h"

#define BAUDRATE ((F_CPU) / (9600 * 16UL) - 1)

#define PD_PIN2 (1 << 2)
#define PD_PIN3 (1 << 3)

static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
LiquidCrystalDevice_t lcd;

static int uart_putchar(char c, FILE *stream)
{
  loop_until_bit_is_set(UCSR0A, UDRE0); // wait for UDR to be clear
  UDR0 = c;                             // send the character
  return 0;
}

ISR(TIMER0_OVF_vect)
{
  PORTD ^= PD_PIN3;
}

void usart_init(void)
{
  UBRR0H = (uint8_t)(BAUDRATE >> 8);
  UBRR0L = (uint8_t)(BAUDRATE);

  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void timer_0_init(void)
{
  TCCR0A = (uint8_t)0;
  TCCR0B = (uint8_t)5;
  TIMSK0 = (uint8_t)1;
}

int main(void)
{
  DDRD = _BV(PIN2) | _BV(PIN3); // PD2 output

  usart_init();
  timer_0_init();
  sei();
  stdout = &mystdout;

  tw_init(TW_FREQ_100K, true);

  _delay_ms(10);
  lcd = lq_init(0x27, 20, 4, 0);
  lq_print(&lcd, "Hello World!");
  init_sysclock_2();

  while (1)
  {
    _delay_ms(500);
    PIND |= _BV(PIN2);
    printf("Hello World! %lu\n", millis());
  }
  return 0;
}
