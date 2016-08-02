/*
 * eeprom.h
 *
 * Created: 7/29/2016 4:13:49 PM
 *  Author: supersat
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#define U2F_KEY_HEADER_ADDR		0
#define U2F_EEPROM_CONFIG		128
#define U2F_APP_CONFIG			256

void eeprom_read(uint16_t addr, uint8_t *buf, int len);
void eeprom_write(uint16_t addr, uint8_t *buf, int len);
void eeprom_erase();

#endif /* EEPROM_H_ */