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
 */

#ifndef CUSTOM_H_
#define CUSTOM_H_

#include "u2f_hid.h"

#define U2F_CUSTOM_GET_SERIAL_NUMBER	0xc0
#define U2F_CUSTOM_GET_CONFIG			0xc1
#define U2F_CUSTOM_INIT_CONFIG			0xc2
#define U2F_CUSTOM_LOCK_CONFIG			0xc3
#define U2F_CUSTOM_GEN_ATT_KEY			0xc4
#define U2F_CUSTOM_WRITE_ATT_CERT		0xc5
#define U2F_CUSTOM_GET_RNG				0xc6
#define U2F_CUSTOM_SEED_RNG				0xc7
#define U2F_CUSTOM_WIPE_KEYS			0xc8
#define U2F_CUSTOM_ENTER_BOOTLOADER		0xc9
#define U2F_CUSTOM_INC_COUNT			0xca

uint8_t custom_command(struct u2f_hid_msg * msg);

#endif