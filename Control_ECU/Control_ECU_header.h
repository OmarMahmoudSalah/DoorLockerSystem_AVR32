/*
 * Control_ECU_header.h
 *
 *  Created on: Nov 5, 2023
 *      Author: Omar
 */

#ifndef CONTROL_ECU_HEADER_H_
#define CONTROL_ECU_HEADER_H_

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





void SYS_Create_Password_CTRL(uint8 *Ptr1ToPass);
void SYS_Change_Password_CTRL(void);
void SYS_Check_Password_CTRL(void);
void SYS_MainMenu_CTRL(void);
void TimerCounterCallBack (void);
void SYS_ComparePasswords(const uint8 *Ptr1ToPass, const uint8 *Ptr2ToPass);
void SYS_EEPROM_Compare(void);
void SYS_EEPROM_Write(void);
#endif /* CONTROL_ECU_HEADER_H_ */
