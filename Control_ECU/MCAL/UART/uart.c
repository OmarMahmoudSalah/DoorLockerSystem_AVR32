/*
 * uart.c
 *
 *  Created on: Oct 13, 2023
 *      Author: Omar
 */

#include "uart.h"
#include <avr/io.h>
#include "../../General/common_macros.h"

void UART_init(const UART_ConfigType *Config_Ptr) {
	uint16 ubrr_value = 0;

	// UCSRA - USART Control and Status Register A
	//U2X: Double the USART Transmission Speed
	UCSRA = (1 << U2X);

	// UCSRB - USART Control and Status Register B
	/*
	 * Bit 4 – RXEN: Receiver Enable
	 * Bit 3 – TXEN: Transmitter Enable
	 *
	 * */
	UCSRB = (1 << TXEN) | (1 << RXEN);

	// UCSRC - USART Control and Status Register C

	/*Bit 7 – URSEL: Register Select
	 * To Enable Any Edit to this Register
	 *Bit 6 – UMSEL: USART Mode Select
	 This bit selects between Asynchronous and Synchronous mode of operation.
	 *• Bit 5:4 – UPM1:0: Parity Mode
	 *•  Bit 3 – USBS: Stop Bit Select (1/2 BITS)
	 *•  Bit 2:1 – UCSZ1:0: DATA Size)
	 *
	 * */

	//URSEL: Register Select
	SET_BIT(UCSRC, URSEL);
	//Asynchronous Operation
	CLEAR_BIT(UCSRC, UMSEL);

	//Parity Configuration
	UCSRC = (UCSRC & 0XCF) | ((Config_Ptr->parity) << 4);
	//Stop bit Configuration
	UCSRC = (UCSRC & 0XF7) | ((Config_Ptr->stop_bit) << 3);
	//Data Size Configuration
	UCSRC = (UCSRC & 0XF9) | ((Config_Ptr->bit_data) << 1);
	//Get Value of Last bit in a 3 bit Number and UCSZ2
	//Easier Solution - Only the 1 appears in 9bit so can do if cond. with it
	UCSRB = (UCSRB & 0XFB) | ((GET_BIT(Config_Ptr->bit_data, 2)) << 2);

	ubrr_value = (uint16) (((F_CPU / ((Config_Ptr->baud_rate) * 8UL))) - 1);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value >> 8;
	UBRRL = ubrr_value;
}

uint8 UART_receiveByte(void) {
	/*Bit 7 – RXC: USART Receive Complete
	 This flag bit is set when there are unread data in the receive buffer
	 and cleared when the receive buffer is empty (i.e., does not contain any unread data).
	 * */
	while ((BIT_IS_CLEAR(UCSRA, RXC)))
		;
	return UDR;
}

void UART_sendByte(uint8 data) {
	/*Bit 5 – UDRE: USART Data Register Empty
	 * The UDRE Flag indicates if the transmit buffer (UDR) is ready to receive new data.
	 * If UDRE is one, the buffer is empty, and therefore ready to be written.
	 * */
	while (BIT_IS_CLEAR(UCSRA, UDRE))
		;
	UDR = data;
}

void UART_sendString(const uint8 *str) {
	uint8 i = 0;

	while (str[i] != '\0') {
		UART_sendByte(str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str) {
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_receiveByte();

	/* Receive the whole string until the '#' */
	while (Str[i] != '#') {
		i++;
		Str[i] = UART_receiveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}

