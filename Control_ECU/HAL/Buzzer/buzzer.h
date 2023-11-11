/*
 * buzzer.h
 *
 *  Created on: Nov 5, 2023
 *      Author: Omar
 */

#ifndef HAL_BUZZER_BUZZER_H_
#define HAL_BUZZER_BUZZER_H_

#include "../../General/std_types.h"
#include "../../MCAL/GPIO/gpio.h"

#define BUZZER_PORT						PORTB_ID
#define BUZZER_PIN						PIN6_ID


void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);





#endif /* HAL_BUZZER_BUZZER_H_ */
