/*
 * bootloader.c
 *
 * Created: 8/1/2016 1:27:28 AM
 *  Author: supersat
 */ 

#include <stdint.h>

#define LAST_BOOT_ENTRY 0x7ffe
void (*boot_flash_page_erase_and_write)(unsigned long adr)=(void (*)(unsigned long))(LAST_BOOT_ENTRY-12);
uint8_t (*boot_flash_read_sig) (unsigned long adr)=(uint8_t (*)(unsigned long))(LAST_BOOT_ENTRY-10);
uint8_t (*boot_flash_read_fuse) (unsigned long adr)=(uint8_t (*)(unsigned long))(LAST_BOOT_ENTRY-8);
void (*boot_flash_fill_temp_buffer) (unsigned int data,unsigned int adr)=(void (*)(unsigned int, unsigned int))(LAST_BOOT_ENTRY-6);
void (*boot_flash_prg_page) (unsigned long adr)=(void (*)(unsigned long))(LAST_BOOT_ENTRY-4);
void (*boot_flash_page_erase) (unsigned long adr)=(void (*)(unsigned long))(LAST_BOOT_ENTRY-2);
void (*boot_lock_wr_bits) (unsigned char val)=(void (*)(unsigned char))(LAST_BOOT_ENTRY);
void (*boot_entry) ()=(void (*)())(0x7800);

void enterBootloader()
{
	boot_entry();
}