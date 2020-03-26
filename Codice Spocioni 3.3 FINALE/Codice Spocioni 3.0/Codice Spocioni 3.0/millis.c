/* 
The millis() function known from Arduino
Calling millis() will return the milliseconds since the program started

Tested on atmega328p

Using content from http://www.adnbr.co.uk/articles/counting-milliseconds
Author: Monoclecat, https://github.com/monoclecat/avr-millis-function

REMEMBER: Add sei(); after init_millis() to enable global interrupts!
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "serial.h"



volatile unsigned long timer0_millis; 
//NOTE: A unsigned long holds values from 0 to 4,294,967,295 (2^32 - 1). It will roll over to 0 after reaching its maximum value.

ISR(TIMER0_COMPA_vect)
{
  timer0_millis++;  
  //Serial_Send_String(".\n");
}

void init_millis(unsigned long f_cpu)
{
  unsigned long long ctc_match_overflow;
  
  ctc_match_overflow = ((f_cpu / 10000) / 8); //when timer1 is this value, 1ms has passed
    
  // (Set timer to clear when matching ctc_match_overflow) | (Set clock divisor to 8)
  TCCR0A = (1<<WGM01);
  TCCR0B = (1 << CS01)|(1 << CS00);
  
  // high byte first, then low byte
 OCR0A=ctc_match_overflow;
 
  // Enable the compare match interrupt
  TIMSK0 |= (1 << OCIE0A);
 
  //REMEMBER TO ENABLE GLOBAL INTERRUPTS AFTER THIS WITH sei(); !!!
}

unsigned long millis()
{
  unsigned long millis_return;
 
  // Ensure this cannot be disrupted
  
    millis_return = timer0_millis;
	return millis_return;
} 