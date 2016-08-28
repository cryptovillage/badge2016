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
#include <util/delay.h>
#include "crc.h"

#define TWI_PORT	PORTB
#define TWI_DIR		DDRB
#define TWI_PIN		PINB
#define TWI_SCL		PORTB5
#define TWI_SDA		PORTB4

void twiBitDelay()
{
	_delay_us(4);
}

void twiSCLLow()
{
	TWI_PORT &= ~_BV(TWI_SCL);
	TWI_DIR |= _BV(TWI_SCL);
}

void twiSCLHigh()
{
	TWI_DIR &= ~_BV(TWI_SCL);
	TWI_PORT |= _BV(TWI_SCL);
}

void twiSDALow()
{
	TWI_PORT &= ~_BV(TWI_SDA);
	TWI_DIR |= _BV(TWI_SDA);
}

void twiSDAHigh()
{
	TWI_DIR &= ~_BV(TWI_SDA);
	TWI_PORT |= _BV(TWI_SDA);
}

void twiInit()
{
	twiSCLHigh();
	twiSDAHigh();
}

void twiWake()
{
	twiSDALow();
	_delay_us(70);
	twiSDAHigh();
	_delay_us(2600);
}

void twiStart()
{
	twiSDALow();
	twiBitDelay();
	twiSCLLow();
}

void twiStop()
{
	twiSDALow();
	twiBitDelay();
	twiSCLHigh();
	while (!(TWI_PIN & _BV(TWI_SCL)));
	twiBitDelay();
	twiSDAHigh();
	twiBitDelay();
}

void twiSendBit(uint8_t bit)
{
	if (bit) {
		twiSDAHigh();
	} else {
		twiSDALow();
	}
	twiBitDelay();
	twiSCLHigh();
	twiBitDelay();
	while (!(TWI_PIN & _BV(TWI_SCL)));
	twiSCLLow();
}

uint8_t twiRecvBit()
{
	uint8_t bit;
	
	twiSDAHigh();
	twiBitDelay();
	twiSCLHigh();
	while (!(TWI_PIN & _BV(TWI_SCL)));
	twiBitDelay();
	bit = (TWI_PIN & _BV(TWI_SDA)) ? 1 : 0;
	twiSCLLow();
	return bit;
}

uint8_t twiSendByte(uint8_t byte)
{
	for (uint8_t i = 0; i < 8; i++) {
		twiSendBit(byte & 0x80);
		byte <<= 1;
	}
	
	return twiRecvBit();
}

uint8_t twiRecvByte(uint8_t nack)
{
	uint8_t b = 0;
	for (uint8_t i = 0; i < 8; i++) {
		b <<= 1;
		b |= twiRecvBit();
	}
	twiSendBit(nack);
	return b;
}

uint8_t twiSendPkt(uint8_t addr, uint8_t *pkt, int len)
{
	twiStart();
	if (twiSendByte(addr << 1)) {
		twiStop();
		return 1;
	}
	while (len--) {
		if (twiSendByte(*pkt++)) {
			break;
		}
	}
	twiStop();
	return len;
}

uint8_t twiSendExtPkt(uint8_t addr, uint8_t *p1, int lp1, uint8_t *p2, int lp2)
{
	uint16_t crc = 0;
	
	twiStart();
	if (twiSendByte(addr << 1)) {
		twiStop();
		return 1;
	}
	if (lp1--) {
		twiSendByte(*p1++);
	}
	while (lp1--) {
		crc = feed_crc(crc, *p1);
		if (twiSendByte(*p1++)) {
			break;
		}
	}
	while (lp2--) {
		crc = feed_crc(crc, *p2);
		if (twiSendByte(*p2++)) {
			break;
		}
	}
	crc = reverse_bits(crc);
	twiSendByte(crc & 0xff);
	twiSendByte(crc >> 8);
	twiStop();
	return 0;
}

uint8_t twiRecvPkt(uint8_t addr, uint8_t *pkt, int len)
{
	twiStart();
	if (twiSendByte((addr << 1) | 1)) {
		twiStop();
		return 1;
	}
	while (len--) {
		*pkt++ = twiRecvByte(!len);
	}
	twiStop();
	return 0;
}

uint8_t twiRecvVariableLenPkt(uint8_t addr, uint8_t *pkt, int maxLen)
{
	uint8_t len;
	
	twiStart();
	if (twiSendByte((addr << 1) | 1)) {
		twiStop();
		return 0;
	}
	len = *pkt++ = twiRecvByte(0);
	if (len < maxLen) {
		maxLen = len;
	}
	if (len > maxLen) {
		len = maxLen;
	}
	while (--maxLen) {
		*pkt++ = twiRecvByte(!maxLen);
	}
	twiStop();
	return len;
}