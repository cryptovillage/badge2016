EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:blinkybadge
LIBS:blinkybadge-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 2 3
Title "CPV 2016 Badge LiPo Power"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 3950 3400 0    60   Input ~ 0
VUSB
$Comp
L MCP73831 U?
U 1 1 5779CF87
P 5100 3500
F 0 "U?" H 4900 3700 60  0000 C CNN
F 1 "MCP73831" H 5100 3300 60  0000 C CNN
F 2 "" H 5100 3500 60  0000 C CNN
F 3 "" H 5100 3500 60  0000 C CNN
	1    5100 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3400 4650 3400
Wire Wire Line
	5550 3600 5700 3600
Wire Wire Line
	5700 3600 5700 4050
$Comp
L GND #PWR?
U 1 1 5779CFD2
P 5700 4050
F 0 "#PWR?" H 5700 3800 50  0001 C CNN
F 1 "GND" H 5700 3900 50  0000 C CNN
F 2 "" H 5700 4050 50  0000 C CNN
F 3 "" H 5700 4050 50  0000 C CNN
	1    5700 4050
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5779D02A
P 4300 3650
F 0 "C?" H 4325 3750 50  0000 L CNN
F 1 "4.7uF" H 4325 3550 50  0000 L CNN
F 2 "" H 4338 3500 50  0000 C CNN
F 3 "" H 4300 3650 50  0000 C CNN
	1    4300 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3500 4300 3400
Connection ~ 4300 3400
Wire Wire Line
	4300 3800 4300 3900
Wire Wire Line
	4300 3900 6500 3900
$Comp
L R R?
U 1 1 5779D0FC
P 5900 3700
F 0 "R?" V 5980 3700 50  0000 C CNN
F 1 "2.0k" V 5900 3700 50  0000 C CNN
F 2 "" V 5830 3700 50  0000 C CNN
F 3 "" H 5900 3700 50  0000 C CNN
	1    5900 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3500 5900 3500
Wire Wire Line
	5900 3500 5900 3550
Wire Wire Line
	5900 3900 5900 3850
Connection ~ 5700 3900
$Comp
L C C?
U 1 1 5779D16F
P 6150 3650
F 0 "C?" H 6175 3750 50  0000 L CNN
F 1 "4.7uF" H 6175 3550 50  0000 L CNN
F 2 "" H 6188 3500 50  0000 C CNN
F 3 "" H 6150 3650 50  0000 C CNN
	1    6150 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3900 6150 3800
Connection ~ 5900 3900
Wire Wire Line
	5550 3400 6900 3400
Wire Wire Line
	6150 3400 6150 3500
$Comp
L CONN_01X02 P?
U 1 1 5779D232
P 6700 3650
F 0 "P?" H 6700 3800 50  0000 C CNN
F 1 "CONN_01X02" V 6800 3650 50  0000 C CNN
F 2 "" H 6700 3650 50  0000 C CNN
F 3 "" H 6700 3650 50  0000 C CNN
	1    6700 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3900 6500 3700
Connection ~ 6150 3900
Wire Wire Line
	6500 3400 6500 3600
Connection ~ 6150 3400
Text HLabel 4850 3150 2    60   Output ~ 0
STAT
Wire Wire Line
	4650 3600 4550 3600
Wire Wire Line
	4550 3600 4550 3150
Wire Wire Line
	4550 3150 4850 3150
Text HLabel 6900 3400 2    60   Output ~ 0
VBAT
Connection ~ 6500 3400
$EndSCHEMATC
