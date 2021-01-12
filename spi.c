#include "spi.h"

void spi__init(void) {
	
	/* MOSI, SCK, CS output in master mode */
	DDRB = (1<<MOSI) | (1<<CS) | (1<<SCK);
	
	/* Set SPI Control Register bits, further explained below */
	//SPCR = (1<<MSTR) | (1<<SPR0); //SPI DISABLED BY DEFAULT
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0); //SPI ENABLED BY DEFAULT
	/*	SPE: SPI Enable bit, 1 = SPI enabled									  
	 *	MSTR: Select Master bit, 1 = master mode								  
	 *	SPR0: SPI Clock Rate 0 Select bit, SPR1 0 & SPR0 1 = SCK freq of f_osc/16 
	 *		- Sets SCK freq to f_osc/16 setting, which matches MCU clock of 16MHz
	 *	CPOL & CPHA default to 0 so SPI mode is 0, which SDC uses				  */
	
}

void spi__start(void) {
	//Enable SPI, Set SPI Enable bit to 1;
	SPCR |= (1<<SPE);
}

void spi__stop(void) {
	//Disable SPI, Set SPI Enable bit to 0;
	SPCR &= ~(1<<SPE);
}

void spi__write(uint8_t data) {
	//Load SPI data register with data
	SPDR = data;
	//Wait for data transmission to finish
	while (!(SPSR & (1<<SPIF)));
}

unsigned char spi__receive(void) {
	//Wait for data transmission to finish
	while (!(SPSR & (1<<SPIF)));
	//Return received data, stored in SPDR register
	return SPDR;
}

unsigned char spi__tranceiver (uint8_t data)
{	
	//Load SPI data register with data
	SPDR = data;                                  
	//Wait for data transmission to finish
	while(!(SPSR & (1<<SPIF) ));
	//Return received data, stored in SPDR register
	return(SPDR);
}

