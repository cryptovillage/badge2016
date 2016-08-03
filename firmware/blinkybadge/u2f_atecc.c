/*
 * Copyright (c) 2016, Conor Patrick
 * Copyright (c) 2016, Karl Koscher
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 *
 *
 * u2f_atecc.c
 * 		platform specific functionality for implementing U2F
 *
 */

#include "blinkybadge.h"
#include "u2f.h"
#include "u2f_hid.h"
#include "eeprom.h"
#include "atecc508a.h"

#include <util/delay.h>
#include <avr/sleep.h>

struct key_handle
{
	uint8_t index;
	uint8_t entropy[3];
};

struct key_storage_header
{
	uint8_t num_keys;
	uint16_t valid_keys;
	uint8_t num_issued;
} key_store;

#define U2F_NUM_KEYS 				U2F_ATTESTATION_KEY_SLOT
#define U2F_KEYS_ADDR				0
#define U2F_KEY_ADDR(k)				(U2F_KEYS_ADDR + ((k)*U2F_KEY_HANDLE_SIZE))


#define IS_KEY_VALID(mask,key)	((~mask) & (1<<key))

static struct u2f_hid_msg res;
static uint8_t* resbuf = (uint8_t*)&res;
static uint8_t resseq = 0;
static uint8_t serious = 0;

// 0 if same, 1 otherwise
static uint8_t key_same(struct key_handle * k1, struct key_handle * k2)
{
	uint8_t i;
	if (k1->index != k2->index) return 1;
	for (i=0; i < U2F_KEY_HANDLE_SIZE-1; i++)
	{
		if (k1->entropy[i] != k2->entropy[i])
		{
			return 1;
		}
	}
	return 0;
}

static void flush_key_store()
{
	eeprom_write(U2F_EEPROM_CONFIG, (uint8_t* )&key_store, sizeof(struct key_storage_header));
}

int8_t u2f_wipe_keys()
{
	uint8_t presses = 5;
	serious = 1;
	while(presses--)
	{
		if (u2f_get_user_feedback() != 0)
		{
			goto nowipe;
		}
	}

	// wipe
	serious = 0;
	eeprom_erase();
	u2f_init();
	return 0;

	nowipe:
	serious = 0;
	return -1;
}

void u2f_init()
{
	int8_t i,ec;
	uint8_t * clear = 0;
	struct atecc_response res;
	
	if (is_locked(appdata.tmp)) {	
		eeprom_read(U2F_EEPROM_CONFIG, (uint8_t* )&key_store, sizeof(struct key_storage_header));

		// initialize key handles
		if (key_store.num_keys != U2F_NUM_KEYS)
		{
			//watchdog();
	
			for (i=0; i < U2F_NUM_KEYS; i++)
			{
				//watchdog();
				ec = atecc_send_recv(ATECC_CMD_RNG,ATECC_RNG_P1,ATECC_RNG_P2,
								NULL, 0,
								appdata.tmp,
								sizeof(appdata.tmp), &res);
				if (ec != 0)
				{
					//u2f_printb("atecc_send_recv failed ",2,i,-ec);
	
					// erase eeprom
					//eeprom_erase(U2F_EEPROM_CONFIG);
	
					// erase ram
					//for (i=0; i<0x400;i++)
					//{
					//	*(clear++) = 0x0;
					//}
					// reset
					//reboot();
					return;
				}
				res.buf[0] = i+1;

				eeprom_write(U2F_KEYS_ADDR + i * U2F_KEY_HANDLE_SIZE,
							 res.buf, U2F_KEY_HANDLE_SIZE);
			}

			key_store.num_keys = U2F_NUM_KEYS;
			key_store.valid_keys = 0;
			key_store.num_issued = 0;
			flush_key_store();
		}
	}
}

void u2f_response_writeback_progmem(uint8_t PROGMEM * buf, uint16_t len)
{
	while (len--) {
		u2f_hid_writeback(pgm_read_byte(buf++), 1);
	}
}

void u2f_response_writeback(uint8_t * buf, uint16_t len)
{
	u2f_hid_writeback(buf, len);
}

void u2f_response_flush()
{
	//watchdog();
	u2f_hid_flush();
}

void u2f_response_start()
{
	//watchdog();
}

int8_t u2f_get_user_feedback()
{
	uint32_t t;
	//_delay_ms(1);
	t = get_ms();
	while(U2F_BUTTON_IS_PRESSED()){
		idleTasks();
		sleep_cpu();
	}
	while(!U2F_BUTTON_IS_PRESSED())
	{
		// turn red
		if (serious)
		{
			//rgb_hex(U2F_DEFAULT_COLOR_ERROR);
		}
		else
		{	// yellow
			//rgb_hex(U2F_DEFAULT_COLOR_INPUT);
		}
		if (get_ms() - t > 10000) {
			// FIXME: for testing. Accept after 10 second timeout
			return 0;
		}
		//	break;
		//watchdog();
		
		idleTasks();
		sleep_cpu();
	}

	if (U2F_BUTTON_IS_PRESSED())
	{
		//rgb_hex(U2F_DEFAULT_COLOR_INPUT_SUCCESS);
	}
	else
	{
		//rgb_hex(U2F_DEFAULT_COLOR_ERROR);
		return 1;
	}

	return 0;
}

static uint8_t shabuf[70];
static uint8_t shaoffset = 0;
static struct atecc_response res_digest;

void u2f_sha256_start()
{
	shaoffset = 0;
	atecc_send_recv(ATECC_CMD_SHA,
			ATECC_SHA_START, 0,NULL,0,
			appdata.tmp, sizeof(appdata.tmp), NULL);
}


void u2f_sha256_update(uint8_t * buf, uint8_t len)
{
	uint8_t i = 0;
	//watchdog();
	while(len--)
	{
		shabuf[shaoffset++] = *buf++;
		if (shaoffset == 64)
		{
			atecc_send_recv(ATECC_CMD_SHA,
					ATECC_SHA_UPDATE, 64,shabuf,64,
					appdata.tmp, sizeof(appdata.tmp), NULL);
			shaoffset = 0;
		}
	}
}


void u2f_sha256_finish()
{
	atecc_send_recv(ATECC_CMD_SHA,
			ATECC_SHA_END, shaoffset,shabuf,shaoffset,
			shabuf, sizeof(shabuf), &res_digest);
}


int8_t u2f_ecdsa_sign(uint8_t * dest, uint8_t * handle)
{
	struct atecc_response res;
	struct key_handle k;
	uint16_t keyslot = (uint16_t)((struct key_handle *)handle)->index;
	//watchdog();
	if (keyslot > U2F_NUM_KEYS)
	{
		return -1;
	}
	if (keyslot == 0)
	{
		keyslot = U2F_ATTESTATION_KEY_SLOT;
	}
	else
	{
		keyslot--;
	}

	atecc_send_recv(ATECC_CMD_SIGN,
			ATECC_SIGN_EXTERNAL, keyslot, NULL, 0,
			appdata.tmp, sizeof(appdata.tmp), &res);

	if (keyslot != U2F_ATTESTATION_KEY_SLOT)
	{
		eeprom_read(U2F_KEY_ADDR(keyslot), (uint8_t* )&k, U2F_KEY_HANDLE_SIZE);

		if (key_same((struct key_handle *)handle, &k) != 0)
		{
			return -1;
		}
	}

	memmove(dest, res.buf, 64);

	return 0;
}

// bad if this gets interrupted
int8_t u2f_new_keypair(uint8_t * handle, uint8_t * pubkey)
{
	struct atecc_response res;
	struct key_handle k;
	uint8_t keyslot = key_store.num_issued;
	if (keyslot > U2F_NUM_KEYS-1)
	{
		//app_wink(U2F_DEFAULT_COLOR_WINK_OUT_OF_SPACE);
		return -1;
	}
	//watchdog();
	atecc_send_recv(ATECC_CMD_GENKEY,
			ATECC_GENKEY_PRIVATE, keyslot, NULL, 0,
			appdata.tmp, sizeof(appdata.tmp), &res);

	memmove(pubkey, res.buf, 64);

	eeprom_read(U2F_KEY_ADDR(keyslot), (uint8_t* )&k, U2F_KEY_HANDLE_SIZE);
	if (k.index-1 != keyslot)
	{

		k.index = keyslot;
		set_app_error(ERROR_BAD_KEY_STORE);
	}
	memmove(handle, &k, U2F_KEY_HANDLE_SIZE);
	key_store.num_issued++;
	flush_key_store();

	return 0;
}

int8_t u2f_load_key(uint8_t * handle)
{
	struct key_handle k;
	uint8_t keyslot = handle[0]-1;
	if (keyslot >= U2F_NUM_KEYS)
	{
		return -1;
	}
	eeprom_read(U2F_KEY_ADDR(keyslot), (uint8_t* )&k, U2F_KEY_HANDLE_SIZE);

	if (key_same((struct key_handle *)handle, &k) != 0)
	{
		return -1;
	}
	return 0;
}

uint32_t u2f_count()
{
	struct atecc_response res;
	atecc_send_recv(ATECC_CMD_COUNTER,
			ATECC_COUNTER_INC, ATECC_COUNTER0,NULL,0,
			appdata.tmp, sizeof(appdata.tmp), &res);
	return *(uint32_t*)res.buf;
}

//const uint8_t __attribute__ ((section (".attcert"))) __attest[1024] = { 
const uint8_t PROGMEM __attest[] = { 
		0x5e, 0x01,
		0x30, 0x82, 0x01, 0x5A, 0x30, 0x82, 0x01, 0x00, 0x02, 0x01, 0x01, 0x30, 0x0A, 0x06, 0x08, 0x2A,
		0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x39, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55,
		0x04, 0x06, 0x13, 0x02, 0x56, 0x41, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13,
		0x0B, 0x43, 0x6F, 0x6E, 0x6F, 0x72, 0x43, 0x6F, 0x20, 0x4C, 0x4C, 0x43, 0x31, 0x14, 0x30, 0x12,
		0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x0B, 0x75, 0x32, 0x66, 0x7A, 0x65, 0x72, 0x6F, 0x2E, 0x63,
		0x6F, 0x6D, 0x30, 0x1E, 0x17, 0x0D, 0x31, 0x36, 0x30, 0x38, 0x30, 0x32, 0x30, 0x38, 0x33, 0x33,
		0x35, 0x39, 0x5A, 0x17, 0x0D, 0x32, 0x32, 0x30, 0x38, 0x30, 0x31, 0x30, 0x38, 0x33, 0x33, 0x35,
		0x39, 0x5A, 0x30, 0x39, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x56,
		0x41, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x0B, 0x43, 0x6F, 0x6E, 0x6F,
		0x72, 0x43, 0x6F, 0x20, 0x4C, 0x4C, 0x43, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03, 0x55, 0x04, 0x03,
		0x13, 0x0B, 0x75, 0x32, 0x66, 0x7A, 0x65, 0x72, 0x6F, 0x2E, 0x63, 0x6F, 0x6D, 0x30, 0x59, 0x30,
		0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE,
		0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x54, 0xE4, 0x5D, 0xFC, 0x4B, 0x08, 0xE6, 0xEE,
		0x63, 0x9B, 0x5C, 0xCD, 0xA1, 0x7D, 0xCF, 0x3E, 0x06, 0xBA, 0x67, 0xBC, 0x38, 0xD6, 0xB6, 0x5D,
		0x07, 0x61, 0x99, 0xD0, 0x14, 0x75, 0xCE, 0x75, 0x86, 0x42, 0x3B, 0x0D, 0x0A, 0xE7, 0x38, 0x5D,
		0x17, 0xF2, 0x81, 0x14, 0x6A, 0xF8, 0x57, 0x59, 0xD2, 0xEC, 0x31, 0x96, 0x0D, 0x8D, 0xA7, 0x06,
		0x80, 0x3A, 0xD1, 0x55, 0x64, 0x2E, 0x5F, 0xFE, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE,
		0x3D, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x21, 0x00, 0xF6, 0x1F, 0xB9, 0x90,
		0x50, 0xB0, 0xA7, 0xD9, 0xD5, 0xD5, 0x52, 0x74, 0xFB, 0x2D, 0x61, 0xA2, 0x7D, 0xB5, 0xF7, 0x5A,
		0x6F, 0xCA, 0x63, 0xAB, 0x9A, 0x3F, 0xE6, 0xFB, 0xB1, 0x94, 0xEE, 0x98, 0x02, 0x20, 0x3C, 0x2C,
		0x8E, 0xFC, 0x97, 0x65, 0x4B, 0x29, 0x4A, 0x11, 0xFF, 0x2B, 0x30, 0xB5, 0xD7, 0x12, 0x2D, 0x5A,
		0x8F, 0x49, 0x0C, 0x9D, 0xD4, 0x1A, 0xF0, 0x21, 0x72, 0x93, 0xD3, 0xA1, 0x9E, 0x4C,
	} ;


uint8_t PROGMEM * u2f_get_attestation_cert()
{
	return __attest + 2;
}

uint16_t u2f_attestation_cert_size()
{
	return pgm_read_word(__attest);
}

void set_response_length(uint16_t len)
{
	u2f_hid_set_len(len);
}