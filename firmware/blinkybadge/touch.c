/*******************************************************************************
*   touch.c
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
/*----------------------------------------------------------------------------
                            compiler information
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                include files
----------------------------------------------------------------------------*/




#include <avr/io.h>
#include <avr/interrupt.h>

#define __delay_cycles(n)     __builtin_avr_delay_cycles(n)

/*  now include touch api.h with the localization defined above */
#include "touch_api.h"
#include "touch.h"
#ifdef _DEBUG_INTERFACE_
/* Include files for QTouch Studio integration */
#include "QDebug.h"
#include "QDebugTransport.h"
#endif

/*----------------------------------------------------------------------------
                            manifest constants
----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
                            type definitions
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                prototypes
----------------------------------------------------------------------------*/
/* This will initialize touch related code */
void touch_init( void );
/* This will call all the functions for touch related measurement */
void touch_measure(void);
/*  Assign the parameters values to global configuration parameter structure    */
static void qt_set_parameters( void );
/*  Configure the sensors */
static void config_sensors(void);

#ifdef _DEBUG_INTERFACE_
void set_timestamp1(void); // used for timestamping
#endif

/*----------------------------------------------------------------------------
                            Structure Declarations
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                    macros
----------------------------------------------------------------------------*/
#ifdef _DEBUG_INTERFACE_
 #ifdef _QDEBUG_TIME_STAMPS_
/* This below code is used for timestamping related information */



  #define TIMESTAMP0  asm("cli"); timestamp0_lword = (uint16_t)TIMER_COUNTER_L;timestamp0_lword |= (uint16_t)(TIMER_COUNTER_H << 8); timestamp0_hword = current_time_ms_touch; asm("sei");
  #define TIMESTAMP1  asm("cli"); timestamp1_lword = (uint16_t)TIMER_COUNTER_L;timestamp1_lword |= (uint16_t)(TIMER_COUNTER_H << 8); timestamp1_hword = current_time_ms_touch; asm("sei");
  #define TIMESTAMP2  asm("cli"); timestamp2_lword = (uint16_t)TIMER_COUNTER_L;timestamp2_lword |= (uint16_t)(TIMER_COUNTER_H << 8); timestamp2_hword = current_time_ms_touch; asm("sei");
  #define TIMESTAMP3  asm("cli"); timestamp3_lword = (uint16_t)TIMER_COUNTER_L;timestamp3_lword |= (uint16_t)(TIMER_COUNTER_H << 8); timestamp3_hword = current_time_ms_touch; asm("sei");
  #define TIMESTAMP4  asm("cli"); timestamp4_lword = (uint16_t)TIMER_COUNTER_L;timestamp4_lword |= (uint16_t)(TIMER_COUNTER_H << 8); timestamp4_hword = current_time_ms_touch; asm("sei");
  #define TIMESTAMP5  asm("cli"); timestamp5_lword = (uint16_t)TIMER_COUNTER_L;timestamp5_lword |= (uint16_t)(TIMER_COUNTER_H << 8); timestamp5_hword = current_time_ms_touch; asm("sei");

 #else

    #define TIMESTAMP0   {}
    #define TIMESTAMP1   {}
    #define TIMESTAMP2   {}
    #define TIMESTAMP3   {}
    #define TIMESTAMP4   {}
    #define TIMESTAMP5   {}
    
  #endif
#endif


/*----------------------------------------------------------------------------
                                global variables
----------------------------------------------------------------------------*/

#ifdef _DEBUG_INTERFACE_
 #ifdef _QDEBUG_TIME_STAMPS_
  uint16_t timestamp0_hword=0;
  uint16_t timestamp0_lword=0;
  uint16_t timestamp1_hword=0;
  uint16_t timestamp1_lword=0;
  uint16_t timestamp2_hword=0;
  uint16_t timestamp2_lword=0;
  uint16_t timestamp3_hword=0;
  uint16_t timestamp3_lword=0;
  uint16_t timestamp4_hword=0;
  uint16_t timestamp4_lword=0;
  uint16_t timestamp5_hword=0;
  uint16_t timestamp5_lword=0;
 #endif
#endif

/*----------------------------------------------------------------------------
                                extern variables
----------------------------------------------------------------------------*/
/* This configuration data structure parameters if needs to be changed will be
   changed in the qt_set_parameters function */
extern qt_touch_lib_config_data_t qt_config_data;
/* touch output - measurement data */
extern qt_touch_lib_measure_data_t qt_measure_data;
/* Get sensor delta values */
extern int16_t qt_get_sensor_delta( uint8_t sensor);
#ifdef QTOUCH_STUDIO_MASKS
extern TOUCH_DATA_T SNS_array[2][2];
extern TOUCH_DATA_T SNSK_array[2][2];
#endif

extern uint8_t time_to_measure_touch;
extern uint16_t current_time_ms_touch;

/* Output can be observed in the watch window using this pointer */
qt_touch_lib_measure_data_t *pqt_measure_data = &qt_measure_data;


/*----------------------------------------------------------------------------
                                static variables
----------------------------------------------------------------------------*/

/*============================================================================
Name    :   touch_init
------------------------------------------------------------------------------
Purpose :   This will initialize touch related code.
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

void touch_init( void )
{



#define QTOUCH_STUDIO_MASKS	1
#define NUMBER_OF_PORTS	1

#define _SNS1_SNSK1_SAME_PORT_
#define SNS1	D
#define SNSK1	D


	#ifdef QTOUCH_STUDIO_MASKS
	SNS_array[0][0]= 0x20;
	SNS_array[0][1]= 0x0;
	SNS_array[1][0]= 0x0;
	SNS_array[1][1]= 0x0;

	SNSK_array[0][0]= 0x10;
	SNSK_array[0][1]= 0x0;
	SNSK_array[1][0]= 0x0;
	SNSK_array[1][1]= 0x0;
	#endif




    /* Configure the Sensors as keys or Keys With Rotor/Sliders in this function */
    config_sensors();

    /* initialise touch sensing */
    qt_init_sensing();

    /*  Set the parameters like recalibration threshold, Max_On_Duration etc in this function by the user */
    qt_set_parameters( );

    /*  Address to pass address of user functions   */
    /*  This function is called after the library has made capacitive measurements,
    *   but before it has processed them. The user can use this hook to apply filter
    *   functions to the measured signal values.(Possibly to fix sensor layout faults)    */
       #ifdef _DEBUG_INTERFACE_
            qt_filter_callback = &set_timestamp1;
       #else
            qt_filter_callback = 0;
       #endif

#ifdef _DEBUG_INTERFACE_
    /* Initialize debug protocol */
    QDebug_Init();

    /* Process commands from PC */
    QDebug_ProcessCommands();
#endif

}


#ifdef _DEBUG_INTERFACE_
void set_timestamp1(void)
{
	TIMESTAMP1;
}
#endif

/*============================================================================
Name    :   touch_measure
------------------------------------------------------------------------------
Purpose :   This will call all the functions for touch related measurement.
Input   :   n/a
Output  :   n/a
Notes   :
============================================================================*/

void touch_measure(void)
{

	/*status flags to indicate the re-burst for library*/
    static uint16_t status_flag = 0u;
    static uint16_t burst_flag = 0u;

	  if( time_to_measure_touch )
        {

            /*  clear flag: it's time to measure touch  */
            time_to_measure_touch = 0u;

            do {
				#ifdef _DEBUG_INTERFACE_
				    TIMESTAMP0;
				#endif

                /*  one time measure touch sensors    */
                status_flag = qt_measure_sensors( current_time_ms_touch );

				#ifdef _DEBUG_INTERFACE_
				    TIMESTAMP2;
				#endif

                burst_flag = status_flag & QTLIB_BURST_AGAIN;
                
                #ifdef _DEBUG_INTERFACE_
                    /* send debug data */
                    QDebug_SendData(status_flag);
                    /* Process commands from PC */
            	    QDebug_ProcessCommands();
                #endif
			    
			    #ifdef _DEBUG_INTERFACE_
			        TIMESTAMP3;
			    #endif

                /* Time-critical host application code goes here */

            }while (burst_flag) ;

        }
        
        #ifdef _DEBUG_INTERFACE_
            TIMESTAMP4;
        #endif
        
        /* Non-Time critical host application code goes here */
        
        #ifdef _DEBUG_INTERFACE_
            TIMESTAMP5;
        #endif
        /* Host sleep code goes here */

}


/*============================================================================
Name    :   qt_set_parameters
------------------------------------------------------------------------------
Purpose :   This will fill the default threshold values in the configuration
            data structure.But User can change the values of these parameters .
Input   :   n/a
Output  :   n/a
Notes   :   Generated Code from QTouch Studio. Do not change
============================================================================*/

static void qt_set_parameters( void )
{

/*  This will be modified by the user to different values   */
qt_config_data.qt_di              = 4;
qt_config_data.qt_neg_drift_rate  = 20;
qt_config_data.qt_pos_drift_rate  = 5;
qt_config_data.qt_max_on_duration = 0;
qt_config_data.qt_drift_hold_time = 20;
qt_config_data.qt_recal_threshold = 0;
qt_config_data.qt_pos_recal_delay = 10;

}

/*============================================================================
Name    :   config_sensors -
------------------------------------------------------------------------------
Purpose :   Configure the sensors
Input   :   n/a
Output  :   n/a
Notes   :   Generated code from QTouch Studio. Do not change
============================================================================*/
static void config_sensors(void)
{
	qt_enable_key( CHANNEL_0, NO_AKS_GROUP, 10u, HYST_25 );

}


