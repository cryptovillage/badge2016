/*
  Copyright 2016 Karl Koscher

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

#ifndef TWI_H_
#define TWI_H_

uint8_t twiSendPkt(uint8_t addr, uint8_t *pkt, int len);
uint8_t twiRecvPkt(uint8_t addr, uint8_t *pkt, int len);
uint8_t twiSendExtPkt(uint8_t addr, uint8_t *p1, int lp1, uint8_t *p2, int lp2);
uint8_t twiRecvVariableLenPkt(uint8_t addr, uint8_t *pkt, int maxLen);

#endif /* TWI_H_ */