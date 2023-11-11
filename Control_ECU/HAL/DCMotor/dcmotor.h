/*
 * dcmotor.h
 *
 *  Created on: Oct 22, 2023
 *      Author: Omar
 */

#ifndef HAL_DCMOTOR_DCMOTOR_H_
#define HAL_DCMOTOR_DCMOTOR_H_

#include "../../General/std_types.h"
#include "../../MCAL/GPIO/gpio.h"
#include "../../MCAL/Timer0_PWM/Timer_0PWM.h"


#define MOTOR_PORT								PORTB_ID
#define IN1										PIN5_ID
#define IN2										PIN4_ID

#define MAX_PWM									256


typedef enum {
	STOP, CW, ACW
} DcMotor_State;

void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* HAL_DCMOTOR_DCMOTOR_H_ */
