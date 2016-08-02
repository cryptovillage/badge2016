/*
 * crc.h
 *
 * Created: 7/31/2016 11:08:41 PM
 *  Author: supersat
 */ 


#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

uint16_t feed_crc(uint16_t crc, uint8_t b);
uint16_t reverse_bits(uint16_t crc);

#endif /* CRC_H_ */