/*
  Copyright 2016 Karl Koscher

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
 */ 

#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>

#define WS2812_PORT		PORTD
#define WS2812_PIN		PORTD0

void ws2812_init()
{
	DDRD = _BV(PORTD0);
}

// FIXME: The timing here is severely broken but seems to work

void updateLEDs(uint8_t * buf, uint8_t count)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		while (count--) {
			uint8_t b = *buf++;
			for (int i = 0; i < 8; i++) {
				WS2812_PORT |= _BV(WS2812_PIN);
				if (b & 0x80) {
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					WS2812_PORT &= ~_BV(WS2812_PIN);
				} else {
					asm("nop");
					WS2812_PORT &= ~_BV(WS2812_PIN);
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
				}
				b <<= 1;
			}
		}
	}
}
