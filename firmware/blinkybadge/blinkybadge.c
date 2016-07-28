/*
  Copyright 2015  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Copyright 2016  Karl Koscher

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
 *  Main source file for the GenericHID demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include <avr/sleep.h>
#include <touch_api.h>
#include "blinkybadge.h"
#include "animation.h"
#include "ws2812.h"
#include "touch.h"
#include "u2f_hid.h"

uint16_t qt_measurement_period_msec = QT_MEASUREMENT_PERIOD_MS;
uint16_t time_ms_inc=0;

struct APP_DATA appdata;

/* flag set by timer ISR when it's time to measure touch */
volatile uint8_t time_to_measure_touch = 0u;

/* current time, set by timer ISR */
volatile uint32_t current_time_ms_touch = 0u;

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t FIDO_HID_Interface =
	{
		.Config =
			{
				.InterfaceNumber              = INTERFACE_ID_FIDO_U2F,
				.ReportINEndpoint             =
					{
						.Address              = FIDO_IN_EPADDR,
						.Size                 = FIDO_U2F_EPSIZE,
						.Banks                = 1,
					},
				.PrevReportINBuffer           = NULL,
				.PrevReportINBufferSize       = 64,
			},
	};


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */

extern uint8_t currentAnimation;

int main(void)
{
	uint8_t led[24];
	uint8_t lastButtonState = 0;
	
	for (int i = 0; i < 24; i++)
		led[i] = 0xff;
		
	SetupHardware();
	GlobalInterruptEnable();
	initAnimation();
	
	for (;;)
	{
		HID_Device_USBTask(&FIDO_HID_Interface);
		USB_USBTask();
		touch_measure();
		if (!lastButtonState && (qt_measure_data.qt_touch_status.sensor_states[0] & 1)) {
			currentAnimation ^= 1;
			beginAnimation();
		}
		lastButtonState = qt_measure_data.qt_touch_status.sensor_states[0] & 1;
		sleep_cpu();
	}
}

uint32_t get_ms()
{
	return current_time_ms_touch;
}

void initTimerISR()
{
	/*  set timer compare value (how often timer ISR will fire set to 1 ms timer interrupt) */
	OCR0A = (TICKS_PER_MS * QT_TIMER_PERIOD_MSEC);

	/*  enable timer ISR on compare A */
	TIMSK0 |= (1 << OCIE0A);

	/*  timer prescaler = system clock / 64  */
	TCCR0B |= (1 << CS01) | (1 << CS00);
	
	/*  timer mode = CTC (count up to compare value, then reset)    */
	TCCR0A |= ( 1 << WGM01 );
}

ISR(TIMER0_COMPA_vect)
{
	time_ms_inc += QT_TIMER_PERIOD_MSEC;

	if(time_ms_inc >= qt_measurement_period_msec)
	{
		time_ms_inc = 0;
		/*  set flag: it's time to measure touch    */
		time_to_measure_touch = 1u;
	}

	/*  update the current time */
	current_time_ms_touch += QT_TIMER_PERIOD_MSEC;
	animationTick();
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	/* disable pull-ups */
	//MCUCR |= (1u << PUD);	
	initTimerISR();
	USB_Init();
	touch_init();
	DDRD = _BV(PORTD0);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	//usb_write("TEST", 4);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= HID_Device_ConfigureEndpoints(&FIDO_HID_Interface);

	USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	HID_Device_ProcessControlRequest(&FIDO_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&FIDO_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
static uint8_t reportData[64];
uint8_t reportSize = 0;

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
	memcpy(ReportData, reportData, reportSize);
	*ReportSize = reportSize;
	reportSize = 0;
	return *ReportSize;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
	u2f_hid_request((struct u2f_hid_msg *)ReportData);
}

void usb_write(uint8_t *buf, int len)
{
	memcpy(reportData, buf, len);
	reportSize = len;
}

uint8_t U2F_BUTTON_IS_PRESSED()
{
	return qt_measure_data.qt_touch_status.sensor_states[0] & 1;
}

static uint8_t error;

void set_app_error(uint8_t ec)
{
	error = ec;
}

uint8_t get_app_error()
{
	return error;
}

void u2f_delay(uint16_t ms)
{
	uint32_t endTime = get_ms() + ms;
	while (get_ms() < endTime) {
		sleep_cpu();
	};
}