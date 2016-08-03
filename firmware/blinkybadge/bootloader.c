/*
 * bootloader.c
 *
 * Created: 8/1/2016 1:27:28 AM
 *  Author: supersat
 */ 

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#define LAST_BOOT_ENTRY 0x3ffe
void (* const boot_flash_page_erase_and_write)(unsigned long adr)=(void (*)(unsigned long))(LAST_BOOT_ENTRY-12);
uint8_t (* const boot_flash_read_sig) (unsigned long adr)=(uint8_t (*)(unsigned long))(LAST_BOOT_ENTRY-10);
uint8_t (* const boot_flash_read_fuse) (unsigned long adr)=(uint8_t (*)(unsigned long))(LAST_BOOT_ENTRY-8);
void (* const boot_flash_fill_temp_buffer) (unsigned int data,unsigned int adr)=(void (*)(unsigned int, unsigned int))(LAST_BOOT_ENTRY-6);
void (* const boot_flash_prg_page) (unsigned long adr)=(void (*)(unsigned long))(LAST_BOOT_ENTRY-4);
void (* const boot_flash_page_erase) (unsigned long adr)=(void (*)(unsigned long))(LAST_BOOT_ENTRY-2);
void (* const boot_lock_wr_bits) (unsigned char val)=(void (*)(unsigned char))(LAST_BOOT_ENTRY);
void (* const boot_entry) ()=(void (*)())(0x3800);

void enterBootloader()
{
	// TABLE FLIP
	cli();
	UDCON = _BV(DETACH); // Detach USB
	USBCON = _BV(FRZCLK);  // disable USB
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;
	// clear timers
	TCCR0B = 0;
	TCCR0A = 0;
	TIMSK0 = 0;
	TIFR0 = 7;
	
	_delay_ms(2000);
	
	boot_entry();
}