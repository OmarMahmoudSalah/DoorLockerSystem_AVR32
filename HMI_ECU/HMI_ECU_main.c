/*
 * HMI_ECU_main.c
 *
 *  Created on: Oct 28, 2023
 *      Author: Omar
 */

////////////////////////// Include Header Files ///////////////////
#include "HMI_ECU_header.h"
#include "HAL/KEYPAD/keypad.h"
#include "MCAL/TIMER1/timer1.h"
#include "HAL/LCD/lcd.h"
#include "MCAL/UART/uart.h"
#include "General/std_types.h"
#include <util/delay.h>
////////////////////////// End Include Header Files ///////////////////

////////////////////////// Global Variables Declaration ///////////////////////////////

/* Global array to store user password for first time */
uint8 passArray1[5];
/* Global array to store user password for second time */
uint8 passArray2[5];
/* Global array to store user password for third time */
uint8 passArray3[5];
uint8 volatile g_uart_order = 0;
uint8 volatile g_keyPressed;
uint16 volatile g_timerCounts = 0;
/* Global flag */
uint8 volatile g_errorCount = 0;

/////////////////////////////////////////////////////////////////////////////

int main() {
	LCD_init();
	UART_ConfigType UARTConfiguration = { BIT_8, Disabled, BIT_1, 9600 };
	UART_init(&UARTConfiguration);
	Timer1_ConfigType TimerConfiguration = { 0, 31250, CLK_256, COMPARE };
	Timer1_setCallBack(TimerCounterCallBack);

	UART_sendByte(HMI_ECU_READY);
	while (UART_receiveByte() != CONTROL_ECU_READY) {
	}

	while (1) {
		g_uart_order = UART_receiveByte();
		switch (g_uart_order) {

		case MainMenu:
			SYS_MainMenu_HMI();
			break;

		case SetNewPassword:
			g_uart_order = 0;
			SYS_Create_Password_HMI();
			////////////////////////////////
			UART_sendByte(Password1Ready);
			SYS_Send_Password_HMI(passArray1);
			UART_sendByte(Password2Ready);
			SYS_Send_Password_HMI(passArray2);
			//////////////////////////////////
			break;
		case PasswordUnmatched:
			g_uart_order = 0;
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Password");
			LCD_displayStringRowColumn(1, 0, "Unmatched");
			_delay_ms(1500);
			LCD_clearScreen();
			break;
		case PasswordMatched:
			g_uart_order = 0;
			LCD_clearScreen();
			LCD_displayStringRowColumn(1, 0, "Correct");
			_delay_ms(1500);
			LCD_clearScreen();
			SYS_MainMenu_HMI();
			break;

		case PasswordInCorrect:
			SYS_IncorrectPass();
			SYS_Check_Password_HMI();
			UART_sendByte(SendRequestedPass);
			SYS_Send_Password_HMI(passArray3);
			break;

		case PasswordCorrect:
			g_uart_order = 0;
			if (g_keyPressed == '+') {
				g_keyPressed = 0;
				LCD_clearScreen();
				UART_sendByte(OpenDoor);
				Timer1_init(&TimerConfiguration);
				LCD_displayStringRowColumn(0, 0, "Door is Unlocking");
				while (g_timerCounts != 2) {
				}
				g_timerCounts = 0;
				Timer1_deInit();
				LCD_clearScreen();
				Timer1_init(&TimerConfiguration);
				LCD_displayStringRowColumn(0, 0, "Door Stopped");
				while (g_timerCounts != 2) {
				}
				g_timerCounts = 0;
				Timer1_deInit();
				LCD_clearScreen();
				Timer1_init(&TimerConfiguration);
				LCD_displayStringRowColumn(0, 0, "Door is Locking");
				g_timerCounts = 0;
				while (g_timerCounts != 2) {
				}
				Timer1_deInit();
				UART_sendByte(MainMenu);

			}
			if (g_keyPressed == '-') {
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 0, "Change Request");
				_delay_ms(1000);
				g_keyPressed = 0;
				UART_sendByte(SetNewPassword);
			}
			break;

		case THEIF:
			g_uart_order = 0;
			LCD_clearScreen();
			UART_sendByte(THEIF);
			LCD_displayStringRowColumn(0, 0, "ERROR");
			Timer1_init(&TimerConfiguration);
			while (g_timerCounts != 2) {
			}
			g_timerCounts = 0;
			Timer1_deInit();
			LCD_clearScreen();
			SYS_MainMenu_HMI();
			break;

		default:
			break;
		}

	}

}

void SYS_Create_Password_HMI(void) {
	uint8 i;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Enter Password :");
	LCD_moveCursor(1, 0);
	for (i = 0; i < PasswordSize; i++) {
		passArray1[i] = KEYPAD_getPressedKey();
		//password1 = password1 * 10 + passArray1[i];
		LCD_displayCharacter('*');
		_delay_ms(300);
	}
	LCD_clearScreen();
	while (KEYPAD_getPressedKey() != '=') {
		LCD_displayStringRowColumn(0, 1, "Press Enter");
	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Re-Enter Pass :");
	LCD_moveCursor(1, 0);
	for (i = 0; i < PasswordSize; i++) {
		passArray2[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(300);
	}
	LCD_clearScreen();
	while (KEYPAD_getPressedKey() != '=') {

		LCD_displayStringRowColumn(0, 1, "Press Enter");
	}

}

void SYS_Send_Password_HMI(uint8 *Ptr1ToPass) {
	uint8 i;
	// Send Password
	for (i = 0; i < PasswordSize; i++) {
		UART_sendByte(Ptr1ToPass[i]);
	}

}

void SYS_Check_Password_HMI(void) {
	uint8 i;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Enter Your Pass");
	LCD_moveCursor(1, 0);
	for (i = 0; i < PasswordSize; i++) {
		passArray3[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(350);
	}

}

void SYS_MainMenu_HMI(void) {
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ :Open Door");
	LCD_displayStringRowColumn(1, 0, "- :Change Pass");
	g_keyPressed = KEYPAD_getPressedKey();
	if (g_keyPressed == '+' || g_keyPressed == '-') {
		SYS_Check_Password_HMI();
		UART_sendByte(SendRequestedPass);
		SYS_Send_Password_HMI(passArray3);

	}
}

void test(void) {
	uint8 key = KEYPAD_getPressedKey();
	UART_sendByte(key);
	key = UART_receiveByte();
	LCD_moveCursor(0, 1);
	LCD_intgerToString(key);
	LCD_displayCharacter(' ');

}
void TimerCounterCallBack(void) {
	// As Configured Timer Ticks Interrupt Every 1 sec
	g_timerCounts++;
}

void SYS_IncorrectPass(void) {
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Incorrect Pass");
	LCD_displayStringRowColumn(1, 0, "  Try Again ");
	_delay_ms(1500);
	LCD_clearScreen();
}
