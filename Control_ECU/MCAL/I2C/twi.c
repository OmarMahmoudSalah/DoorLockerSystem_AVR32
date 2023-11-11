/*
 * twi.c
 *
 *  Created on: Nov 5, 2023
 *      Author: Omar
 */

#include "twi.h"
#include <avr/io.h>
#include "../../General/common_macros.h"

void TWI_init(const TWI_ConfigType *Config_Ptr) {
	/*
	 * TWI Bit Rate Register – TWBR
	 * TWBR7 TWBR6 TWBR5 TWBR4 TWBR3 TWBR2 TWBR1 TWBR0
	 * Bits 7..0 – TWI Bit Rate Register
	 * TWBR selects the division factor for the bit rate generator.
	 * The bit rate generator is a frequency
	 * divider which generates the SCL clock frequency in the Master modes
	 * Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz
	 * */
	TWBR = (Config_Ptr->bit_rate);
	/*
	 *TWI Status Register – TWSR
	 *TWS7 TWS6 TWS5 TWS4 TWS3 – TWPS1 TWPS0
	 * Bits 7..3 – TWS: TWI Status
	 * These five bits reflect the status of the TWI logic and the Two-wire Serial Bus
	 *• Bits 1..0 – TWPS: TWI Prescaler Bits
	 *
	 * */
	TWSR = 0x00;
	/*
	 * TWI (Slave) Address Register – TWAR
	 * TWA6 TWA5 TWA4 TWA3 TWA2 TWA1 TWA0 TWGCE
	 * Bits 7..1 – TWA: TWI (Slave) Address Register
	 * These seven bits constitute the slave address of the TWI unit.
	 * Bit 0 – TWGCE: TWI General Call Recognition Enable Bit
	 * If set, this bit enables the recognition of a General Call given over the Two-wire Serial Bus.
	 *Reset Register to clear all initial values then push address
	 *starting from bit 1
	 *
	 * */
	TWAR = (TWAR & 0X00) | ((Config_Ptr->address) << 1);
	/*
	 * TWI Control Register – TWCR
	 * The TWCR is used to control the operation of the TWI
	 * TWINT TWEA TWSTA TWSTO TWWC TWEN – TWIE
	 *
	 * Bit 7 – TWINT: TWI Interrupt Flag :
	 * The TWINT Flag must be cleared by software by writing a logic one to it. Note that this flag is not
	 automatically cleared by hardware when executing the interrupt routine.

	 * Bit 6 – TWEA: TWI Enable Acknowledge Bit
	 * The TWEA bit controls the generation of the acknowledge pulse. If the TWEA bit is written to
	 one, the ACK pulse is generated.

	 * Bit 5 – TWSTA: TWI START Condition Bit
	 * The application writes the TWSTA bit to one when it desires to become a master on the
	 * Two wire Serial Bus
	 *
	 * Bit 4 – TWSTO: TWI STOP Condition Bit
	 * Writing the TWSTO bit to one in Master mode will generate a STOP condition on the Two-wire
	 Serial Bus
	 *
	 * Bit 2 – TWEN: TWI Enable Bit
	 * The TWEN bit enables TWI operation and activates the TWI interface. When TWEN is written to
	 one, the TWI takes control over the I/O pins connected to the SCL and SDA pins
	 *
	 * Bit 0 – TWIE: TWI Interrupt Enable
	 * When this bit is written to one, and the I-bit in SREG is set,
	 * the TWI interrupt request will be activated for as long as the TWINT Flag is high.
	 *
	 * */
	TWCR = (1 << TWEN);
}

void TWI_start(void) {
	/*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
}

void TWI_stop(void) {
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data) {
	/* Put data On TWI data Register */
	TWDR = data;
	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
}

uint8 TWI_readByteWithACK(void) {
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	/* Read Data */
	return TWDR;
}

uint8 TWI_readByteWithNACK(void) {
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	/* Read Data */
	return TWDR;
}

uint8 TWI_getStatus(void) {
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = TWSR & 0xF8;
	return status;
}
