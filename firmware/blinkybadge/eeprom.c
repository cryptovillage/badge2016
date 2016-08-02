/*
 * eeprom.c
 *
 * Created: 7/29/2016 4:07:55 PM
 *  Author: supersat
 */ 

#include <avr/eeprom.h>
#include "eeprom.h"

void eeprom_read(uint16_t addr, uint8_t *buf, int len)
{
	eeprom_read_block(buf, addr, len);
}

void eeprom_write(uint16_t addr, uint8_t *buf, int len)
{
	eeprom_write_block(buf, addr, len);
}

void eeprom_erase()
{
	for (int i = 0; i < 1024; i++) {
		eeprom_update_byte(i, 0xff);
	}
}