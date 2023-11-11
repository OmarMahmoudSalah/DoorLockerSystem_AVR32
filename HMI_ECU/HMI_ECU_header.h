/*
 * HMI_ECU_header.h
 *
 *  Created on: Nov 5, 2023
 *      Author: Omar
 */

#ifndef HMI_ECU_HEADER_H_
#define HMI_ECU_HEADER_H_

#include "General/std_types.h"
#define HMI_ECU_READY                  0X10
#define CONTROL_ECU_READY              0X20
#define PasswordSize                  	5
#define SetNewPassword 				   'E'

#define Password1Ready 				   'D'
#define Password1Saved 				   'G'

#define Password2Ready 				   'e'
#define Password2Saved 				   'S'

#define PasswordUnmatched              'D'
#define PasswordMatched                'I'

#define SendRequestedPass              'P'
#define ReadyRequestPass               'L'

#define THEIF						   'O'

#define PasswordCorrect                'M'
#define PasswordInCorrect              'A'

#define ChangePass             		   'm'
#define OpenDoor           			   't'

#define MainMenu					   'g'



void SYS_Create_Password_HMI(void);
void SYS_Check_Password_HMI(void);
void SYS_Send_Password_HMI(uint8 *Ptr1ToPass);
void SYS_MainMenu_HMI(void);
void TimerCounterCallBack(void);
void SYS_IncorrectPass (void);

#endif /* HMI_ECU_HEADER_H_ */
