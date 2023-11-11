/*
 * timer.c
 *
 *  Created on: Oct 28, 2023
 *      Author: Omar
 */

#include "timer1.h"
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER1_COMPA_vect) {
	if (g_callBackPtr != NULL_PTR) {
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_OVF_vect) {
	if (g_callBackPtr != NULL_PTR) {
		(*g_callBackPtr)();
	}
}

void Timer1_init(const Timer1_ConfigType *Config_Ptr) {

	// Put Initial Value to TCNT1
	TCNT1 = Config_Ptr->initial_value;
	// Using Channel A in Timer1
	//COM1A1 COM1A0 (0-0)
	SET_BIT(TCCR1A, FOC1A);
	//0- Normal  OR 1 -Compare IN WGM12
	//Another Solution than masking is to make if condition to value of enum
	// With Set of Clear bit WGM12
	TCCR1B = (TCCR1B & 0XEF) | ((Config_Ptr->mode) << 4);
	// Now Mask All Register bits except CS12 CS11 CS10
	TCCR1B = (TCCR1B & 0xEF) | (Config_Ptr->prescaler);
	OCR1A = (Config_Ptr->compare_value);
	if ((Config_Ptr->mode) == 0) {
		// Normal Mode
		//OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 OCIE0 TOIE0
		// 1     1		0		0		0	0		1	 1
		// Clear Any Older Flag
		TIMSK = (TIMSK & 0XC3);
		//Bit 2 – TOIE1: Timer/Counter1, Overflow Interrupt Enable
		SET_BIT(TIMSK, TOIE1);
	} else {
		// Clear Any Older Flag
		TIMSK = (TIMSK & 0XC3);
		//Bit 4 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
		SET_BIT(TIMSK, OCIE1A);
	}
	//Enable INTERRUPTS At End of Init
	SET_BIT(SREG, 7);

}

void Timer1_setCallBack(void (*a_ptr)(void)) {
	g_callBackPtr = a_ptr;
}

void Timer1_deInit(void) {
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;

}

