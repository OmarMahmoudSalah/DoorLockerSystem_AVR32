/*
 * Control_ECU_main.c
 *
 *  Created on: Oct 28, 2023
 *      Author: Omar
 */
#include "Control_ECU_header.h"
#include "MCAL/TIMER1/timer1.h"
#include "General/std_types.h"
#include "MCAL/UART/uart.h"
#include "HAL/EEPROM/external_eeprom.h"
#include "MCAL/I2C/twi.h"
#include "HAL/DCMotor/dcmotor.h"
#include <util/delay.h>

////////////////////////// Global Variables Declaration ///////////////////////////////
/* Global array to store user password for first time */
uint8 passArray1[5];
/* Global array to store user password for second time */
uint8 passArray2[5];
/* Global array to store user password for third time */
uint8 passArray3[5];
/* Global array to test from EEPROM */
uint8 passtest[5];
uint8 volatile g_uart_order = 0;
uint8 volatile g_timerCounts = 0;
uint8 volatile g_errorCount = 0;
uint8 volatile g_WrongPassRead = 0;
uint8 volatile g_Tries = 0;

/////////////////////////////////////////////////////////////////////////////
UART_ConfigType UARTConfiguration = { BIT_8, Disabled, BIT_1, 9600 };
Timer1_ConfigType TimerConfiguration = { 0, 31250, CLK_256, COMPARE };

/////////////////////////////////////////////////////////////////////////////

int main() {

	UART_init(&UARTConfiguration);
	Buzzer_init();
	DcMotor_Init();
	Timer1_setCallBack(TimerCounterCallBack);
	TWI_ConfigType I2C_Configuration = { 1, 2 };
	TWI_init(&I2C_Configuration);
	while (UART_receiveByte() != HMI_ECU_READY) {
	}
	UART_sendByte(CONTROL_ECU_READY);
	UART_sendByte(SetNewPassword);

	while (1) {

		g_uart_order = UART_receiveByte();
		switch (g_uart_order) {

		case SetNewPassword:
			UART_sendByte(SetNewPassword);
			break;


		case Password1Ready:
			g_uart_order = 0;
			SYS_Create_Password_CTRL(passArray1);
			UART_sendByte(Password1Saved);
			break;

		case Password2Ready:
			g_uart_order = 0;
			SYS_Create_Password_CTRL(passArray2);
			UART_sendByte(Password2Saved);
			SYS_ComparePasswords(passArray1, passArray2);
			if (g_errorCount == 1) {
				UART_sendByte(PasswordUnmatched);
				UART_sendByte(SetNewPassword);
			} else {
				UART_sendByte(PasswordMatched);
				SYS_EEPROM_Write();
			}
			break;

		case SendRequestedPass:
			g_uart_order = 0;
			SYS_Create_Password_CTRL(passArray3);
			SYS_EEPROM_Compare();
			if (g_WrongPassRead == 0) {
				UART_sendByte(PasswordCorrect);
				g_Tries=0;
			} else {
				UART_sendByte(PasswordInCorrect);
				++g_Tries;
			}
			if (g_Tries == 2) {
				UART_sendByte(THEIF);
				g_Tries = 0;
			}
			break;

		case THEIF:
			g_uart_order = 0;
			Buzzer_on();
			Timer1_init(&TimerConfiguration);
			while (g_timerCounts != 2) {
			}
			g_timerCounts=0;
			Timer1_deInit();
			Buzzer_off();
			break;

		case OpenDoor:
			Timer1_init(&TimerConfiguration);
			DcMotor_Rotate(CW, 100);
			while (g_timerCounts != 2) {
			}
			g_timerCounts=0;
			Timer1_deInit();

			Timer1_init(&TimerConfiguration);
			DcMotor_Rotate(STOP, 100);
			while (g_timerCounts != 2) {
			}
			g_timerCounts=0;
			Timer1_deInit();

			Timer1_init(&TimerConfiguration);
			DcMotor_Rotate(ACW, 100);
			while (g_timerCounts != 2) {
			}
			g_timerCounts=0;
			Timer1_deInit();
			DcMotor_Rotate(STOP, 100);
			UART_sendByte(MainMenu);

		}

	}

}

void SYS_Create_Password_CTRL(uint8 *Ptr1ToPass) {
	uint8 i;
	for (i = 0; i < PasswordSize; i++) {
		Ptr1ToPass[i] = UART_receiveByte();
	}

}

void TimerCounterCallBack(void) {
// As Configured Timer Ticks Interrupt Every 1 sec
	g_timerCounts++;
}

void SYS_ComparePasswords(const uint8 *Ptr1ToPass, const uint8 *Ptr2ToPass) {
	g_errorCount = 0;
	uint8 i;
	for (i = 0; i < PasswordSize; i++) {
		if (Ptr1ToPass[i] != Ptr2ToPass[i]) {
			g_errorCount = 1;
		}
	}

}

void SYS_EEPROM_Write(void) {
	uint8 i;
	for (i = 0; i < PasswordSize; i++) {
		EEPROM_writeByte(0X50 + i, passArray2[i]);
		_delay_ms(500);
	}

}

void SYS_EEPROM_Compare(void) {
	uint8 i;
	g_WrongPassRead = 0;
	for (i = 0; i < PasswordSize; i++) {
		EEPROM_readByte(0X50 + i, &passtest[i]);
	}
	uint8 k;
	for (k = 0; k < PasswordSize; k++) {
		if (passtest[k] != passArray3[k]) {
			g_WrongPassRead = 1;
		}

	}

}

/*
 if (g_WrongPassRead == 0) {
 UART_sendByte(PasswordCorrect);
 } else if (g_WrongPassRead == 3) {

 } else {

 }
 */
