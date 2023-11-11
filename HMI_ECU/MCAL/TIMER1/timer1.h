/*
 * timer.h
 *
 *  Created on: Oct 28, 2023
 *      Author: Omar
 */

#ifndef MCAL_TIMER1_TIMER1_H_
#define MCAL_TIMER1_TIMER1_H_
#include<avr/io.h>
#include<avr/interrupt.h>


#include "../../General/common_macros.h"
#include "../../General/std_types.h"

typedef enum {
	NORMAL, COMPARE
} Timer1_Mode;
typedef enum {
	NO_CLOCK, CLK_1, CLK_8, CLK_64, CLK_256, CLK_1024
} Timer1_Prescaler;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;

void Timer1_init(const Timer1_ConfigType *Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void (*a_ptr)(void));

#endif /* MCAL_TIMER1_TIMER1_H_ */
