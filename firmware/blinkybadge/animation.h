/*
 * animation.h
 *
 * Created: 7/26/2016 2:45:05 PM
 *  Author: supersat
 */ 


#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <stdint.h>

typedef struct {
	uint8_t green;
	uint8_t red;
	uint8_t blue;	
} led_t;

typedef struct {
	led_t leds[8];
	uint16_t delay;	
} frame_t;

typedef struct {
	uint16_t framesOffset;
	uint16_t numFrames;
} animation_t;

void initAnimation();
void beginAnimation();
void animationTick();

#endif /* ANIMATION_H_ */