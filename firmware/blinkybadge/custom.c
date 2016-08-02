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
 * These are commands are "middleware" for HID messages.  So they don't need
 * the HID U2F layer to be called.  Thus they are custom commands not necessary for U2F.
 *
 */

#include <stdint.h>
#include "blinkybadge.h"
#include "custom.h"
#include "atecc508a.h"
#include "bootloader.h"

uint8_t custom_command(struct u2f_hid_msg * msg)
{
	struct atecc_response res;
	struct u2f_hid_msg * reply = (struct u2f_hid_msg *)appdata.tmp;
	uint8_t ec;

	if (msg->cid != U2FHID_BROADCAST) return 0;

	switch(msg->pkt.init.cmd)
	{
		case U2F_CUSTOM_GET_SERIAL_NUMBER:
			if (atecc_send_recv(ATECC_CMD_READ, ATECC_RW_CONFIG | ATECC_RW_EXT, 0,
				NULL, 0, reply->pkt.init.payload, sizeof(reply->pkt.init.payload), &res) == 0 )
			{
				if (res.len > 15)
					res.len = 15;
				reply->cid = msg->cid;
				reply->pkt.init.cmd = msg->pkt.init.cmd;
				U2FHID_SET_LEN(reply, res.len);
				usb_write((uint8_t*)reply, 64);
			}
			else
			{
				reply->cid = msg->cid;
				reply->pkt.init.cmd = msg->pkt.init.cmd;
				U2FHID_SET_LEN(reply, 0);
				usb_write((uint8_t*)reply, 64);
			}
			break;
		case U2F_CUSTOM_GET_CONFIG:
			if (atecc_send_recv(ATECC_CMD_READ, ATECC_RW_CONFIG | ATECC_RW_EXT,
				(msg->pkt.init.payload[0] << 8) | msg->pkt.init.payload[1], NULL, 0, 
				reply->pkt.init.payload, sizeof(reply->pkt.init.payload), &res) == 0)
			{
				if (res.len > 40)
					res.len = 40;
				reply->cid = msg->cid;
				reply->pkt.init.cmd = msg->pkt.init.cmd;
				U2FHID_SET_LEN(reply, res.len);
				usb_write((uint8_t*)reply, 64);
			}
			else
			{
				U2FHID_SET_LEN(msg, 0);
				usb_write((uint8_t*)msg, 64);
			}
			break;
		case U2F_CUSTOM_INIT_CONFIG:
			atecc_setup_config();
			U2FHID_SET_LEN(msg, 0);
			usb_write((uint8_t*)msg, 64);
			break;
		case U2F_CUSTOM_LOCK_CONFIG:
			if (is_locked(appdata.tmp)) {
				msg->pkt.init.payload[0] = 0xff;
				U2FHID_SET_LEN(msg, 1);
				usb_write((uint8_t*)msg, 64);
			}
			else if (atecc_send_recv(ATECC_CMD_LOCK, ATECC_LOCK_CONFIG,
				(msg->pkt.init.payload[0] << 8) | msg->pkt.init.payload[1], NULL, 0, 
				appdata.tmp, sizeof(appdata.tmp), NULL))
			{
				msg->pkt.init.payload[0] = 0xfe;
				U2FHID_SET_LEN(msg, 1);
				usb_write((uint8_t*)msg, 64);
			}
			else
			{
				msg->pkt.init.payload[0] = 0x00;
				U2FHID_SET_LEN(msg, 1);
				usb_write((uint8_t*)msg, 64);
			}
			break;
		case U2F_CUSTOM_GEN_ATT_KEY:
			if (atecc_send_recv(ATECC_CMD_GENKEY, ATECC_GENKEY_PRIVATE, U2F_ATTESTATION_KEY_SLOT,
				NULL, 0, appdata.tmp, sizeof(appdata.tmp), &res) == 0 && res.len <= 64)
			{
				U2FHID_SET_LEN(msg, res.len - 1);
				memmove(msg->pkt.init.payload, res.buf, res.len - 1);
				usb_write((uint8_t*)msg, 64);
			}
			else
			{
				U2FHID_SET_LEN(msg, 0);
				usb_write((uint8_t*)msg, 64);
			}
			break;
			/*
		case U2F_CUSTOM_GET_RNG:
			if (atecc_send_recv(ATECC_CMD_RNG,ATECC_RNG_P1,ATECC_RNG_P2,
				NULL, 0,
				appdata.tmp,
				sizeof(appdata.tmp), &res) == 0 )
			{
				memmove(msg->pkt.init.payload, res.buf, 32);
				U2FHID_SET_LEN(msg, 32);
				usb_write((uint8_t*)msg, 64);
			}
			else
			{
				U2FHID_SET_LEN(msg, 0);
				usb_write((uint8_t*)msg, 64);
			}

			break;
		case U2F_CUSTOM_SEED_RNG:
			ec = atecc_send_recv(ATECC_CMD_NONCE,ATECC_NONCE_RNG_UPDATE,0,
							msg->pkt.init.payload, 20,
							appdata.tmp,
							sizeof(appdata.tmp), &res);
			U2FHID_SET_LEN(msg, 1);
			msg->pkt.init.payload[0] = ec == 0 ? 1 : 0;
			usb_write((uint8_t*)msg, 64);
			break;
			*/
		case U2F_CUSTOM_WIPE_KEYS:

			U2FHID_SET_LEN(msg, 1);
			ec=u2f_wipe_keys();
			msg->pkt.init.payload[0] = ec == 0 ? 1 : 0;
			usb_write((uint8_t*)msg, 64);

			break;
			
		case U2F_CUSTOM_ENTER_BOOTLOADER:
			USB_Detach();
			enterBootloader();
			break;
		case U2F_CUSTOM_INC_COUNT:
			if (atecc_send_recv(ATECC_CMD_COUNTER,
							ATECC_COUNTER_INC, ATECC_COUNTER0,NULL,0,
							appdata.tmp, sizeof(appdata.tmp), &res) == 0)
			{
				memmove(msg->pkt.init.payload, res.buf, res.len);
				U2FHID_SET_LEN(msg, res.len);
				usb_write((uint8_t*)msg, 64);
			}
			else
			{
				U2FHID_SET_LEN(msg, 0);
				usb_write((uint8_t*)msg, 64);
			}
			break;				
		default:
			return 0;
	}
	return 1;
}
