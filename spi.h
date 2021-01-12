#ifndef SPI_H_
#define SPI_H_

//#define F_CPU 16000000UL //Defined in conf.h

#include "conf.h"
#include <avr/io.h>

void spi__init(void); //Initialize dedicated SPI I/O
void spi__start(void); //Start SPI
void spi__stop(void); //Stop SPI
void spi__write(uint8_t data); //Write (arg) data via SPDR register
unsigned char spi__receive(void); //Receive (return) data via SPDR register
unsigned char spi__tranceiver (uint8_t data); //Write (arg) and receive (return) data via SPDR register


#endif /* SPI_H_ */