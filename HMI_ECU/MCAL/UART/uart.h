/*
 * uart.h
 *
 *  Created on: Oct 13, 2023
 *      Author: Omar
 */

#ifndef UART_H_
#define UART_H_
#include "../../General/std_types.h"



typedef enum {
	BIT_5,BIT_6,BIT_7,BIT_8,BIT_9=7
} UART_BitData;

typedef enum {
	Disabled,Even_Parity=2,Odd_Parity
} UART_Parity;

typedef enum {
	BIT_1,BIT_2
} UART_StopBit;

typedef uint32 UART_BaudRate;

typedef struct {
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_BaudRate baud_rate;
} UART_ConfigType;

void UART_init(const UART_ConfigType *Config_Ptr);
uint8 UART_receiveByte(void);
void UART_sendByte(uint8 data);
void UART_sendString(const uint8 *str);
void UART_receiveString(uint8 *Str);



#endif /* UART_H_ */
