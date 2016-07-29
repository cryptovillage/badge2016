/*
  Copyright 2015  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Copyright 2016  Karl Koscher
  Copyright (c) 2016 Conor Patrick

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

/** \file
 *
 *  Header file for blinkybadge.c.
 */

#ifndef _BLINKYBADGE_H_
#define _BLINKYBADGE_H_

#include <stdint.h>

typedef enum
{
	ERROR_NOTHING = 0,
	ERROR_I2C_ERRORS_EXCEEDED = 2,
	ERROR_READ_TRUNCATED = 6,
	ERROR_ATECC_VERIFY = 0x01,
	ERROR_ATECC_PARSE = 0x03,
	ERROR_ATECC_FAULT = 0x05,
	ERROR_ATECC_EXECUTION = 0x0f,
	ERROR_ATECC_WAKE = 0x11,
	ERROR_ATECC_WATCHDOG = 0xee,
	ERROR_ATECC_CRC = 0xff,
	ERROR_I2C_CRC = 0x15,
	ERROR_SEQ_EXCEEDED = 0x12,
	ERROR_BAD_KEY_STORE = 0x13,
	ERROR_USB_WRITE = 0x14,
	ERROR_I2C_BAD_LEN = 0x16,
	ERROR_HID_BUFFER_FULL = 0x17,
	ERROR_HID_INVALID_CMD = 0x18,
	ERROR_DAMN_WATCHDOG = 0x19,
	ERROR_OUT_OF_CIDS = 0x20,
	ERROR_I2C_RESTART = 0x21,
}
APP_ERROR_CODE;

struct APP_DATA
{
	// must be at least 70 bytes
	uint8_t tmp[70];
};

extern struct APP_DATA appdata;

#define U2F_ATTESTATION_KEY_SLOT	15

/* Includes: */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Descriptors.h"
#include "Config/AppConfig.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

/* Function Prototypes: */
void SetupHardware(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize);
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize);
										  
uint32_t get_ms();
void usb_write(uint8_t *buf, int len);
void u2f_init();
uint8_t U2F_BUTTON_IS_PRESSED();
void set_app_error(uint8_t ec);
uint8_t get_app_error();
void u2f_delay(uint16_t ms);

#endif

