#include "crc.h"

// CRC-16 appropriate for a byte model interrupt routine.
uint16_t feed_crc(uint16_t crc, uint8_t b)
{
	crc ^= b;
	crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
	crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
	crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
	crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
	crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
	crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
	crc = crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
	return crc & 1 ? (crc >> 1) ^ 0xa001 : crc >> 1;
}

// Resulting CRC should be reversed to be correct CRC-16
uint16_t reverse_bits(uint16_t crc)
{
	// efficient bit reversal for 16 bit int
	crc = (((crc & 0xaaaa) >> 1) | ((crc & 0x5555) << 1));
	crc = (((crc & 0xcccc) >> 2) | ((crc & 0x3333) << 2));
	crc = (((crc & 0xf0f0) >> 4) | ((crc & 0x0f0f) << 4));
	return (((crc & 0xff00) >> 8) | ((crc & 0x00ff) << 8));
}
