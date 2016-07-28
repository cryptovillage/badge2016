/*******************************************************************************
*   touch.h
*   Atmel Corporation:  http://www.atmel.com \n
*   Support email:  www.atmel.com/design-support/
******************************************************************************/
/*  License
*   Copyright (c) 2010, Atmel Corporation All rights reserved.
*
*   Redistribution and use in source and binary forms, with or without
*   modification, are permitted provided that the following conditions are met:
*
*   1. Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer.
*
*   2. Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
*   3. The name of ATMEL may not be used to endorse or promote products derived
*   from this software without specific prior written permission.
*
*   THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
*   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
*   SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
*   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
*   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*  This is an automatically generated file. Do not modify the contents manually.
    Instead, use the Project Builder Wizard to make changes to the design.
*/


/**
  * Acquisition Technology Name.
  *
  * Possible values: _QMATRIX_, _QTOUCH_ .
  */
#define _QTOUCH_

/**
  * Device Name.
  *.
  */
#define __ATmega32U2__


/**
  * Number of Channels(dependent on the library used).
  *
  * Possible values: 4, 8, 12, 16.
  */
#define QT_NUM_CHANNELS 4

/**
  * Delay cycles that determine the capacitance charge pulse width.
  *
  * Possible values: 1 to 255
  */
#ifndef QT_DELAY_CYCLES
#define QT_DELAY_CYCLES 16
#endif

/**
  * Enabling the _ROTOR_SLIDER_ constant will link the library need for using rotors
  * and sliders.
  *
  * Possible values: comment/uncomment the define
  */

//#define _ROTOR_SLIDER_ 

/**
  * Enabling _POWER_OPTIMIZATION_ will lead to a 40% reduction in power consumed
  * by the library, but by disabling spread spectrum feature. When power optimization
  * is enabled the unused pins, within a port used for QTouch, may not be usable for
  * interrupt driven applications. This option is available only for ATtiny and ATmega
  * devices.
  *
  * Possible values: 0 or 1 (For ATtiny and ATmega devices)
  *                  0 (For ATxmega devices)
  */

#ifndef _POWER_OPTIMIZATION_
#define _POWER_OPTIMIZATION_ 0
#endif




/**
  * Define the ports to be used for SNS1,SNS2 and SNSK1,SNSK2 pins. SNS1,SNS2 and
  * SNSK1,SNSK2 port pins can be available on the same port or on different ports.
  * _SNS1_SNSK1_SAME_PORT_ is defined if first port pair SNS1 and SNSK1 are on
  * same port. _SNS2_SNSK2_SAME_PORT_ is defined if SNS2 and SNSK2 are on same port.
  * Possible values:
  */

#define QTOUCH_STUDIO_MASKS	1
#define NUMBER_OF_PORTS	1

#define _SNS1_SNSK1_SAME_PORT_
#define SNS1	D
#define SNSK1	D



/**
  * Provide the number of timer clock ticks (cycles) required to provide a 1 millisecond time interval.
  * TICKS_PER_MS = (CLK_FREQ/TIMER_PRESCALER)*(1/1000)
  * Example, TICKS_PER_MS = (8MHz/64)*(1/1000) = 125
  */
#define TICKS_PER_MS 249u


/**
  * Provide the periodic interrupt interval for which the timer is configured.
  * Example, QT_TIMER_PERIOD_MSEC  2u
  * Timer ISR will fire at every 2 milliseconds.
  */
#define QT_TIMER_PERIOD_MSEC  1u

/**
  * Provide the periodic interval for touch measurement.
  * Example, QT_MEASUREMENT_PERIOD_MS 50u
  * Perform a single touch measurement every 50msec.
  */
#define QT_MEASUREMENT_PERIOD_MS 50u


/******************************************************************************
*   Debug Interface Settings
******************************************************************************/

// #define _DEBUG_INTERFACE_






