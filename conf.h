#ifndef CONF_H_
#define CONF_H_

/********************************************************************/
/*  SETUP CONFIGURATION FILE (for ports, etc)						*/
/*																	*/
/*  DUE TO F_CPU, conf.h MUST BE INCLUDED BEFORE ANY <avr/delay.h>	*/
/********************************************************************/

/* ATMega328P CLOCK SPEED */
#define F_CPU 16000000UL

/*---------------------------------------------------------------------------/
/ I/O PORTS
/---------------------------------------------------------------------------*/

/* SPI */
#define CS	 2	//PB2, CS
#define MOSI 3	//PB3, DIN
#define MISO 4	//PB4
#define SCK  5	//PB5, CLK

/* NOKIA 5110 LCD */
#define DC 5	//D5 PD5
#define RST 6	//D6 PD6

/* 1602 LCD */
#define LCD_RS (1<<0) //PCF P0
#define LCD_E (1<<2) //P2
#define LCD_BL (1<<3) //P3 goes to backlight A/K on LCD
#define LCD_D4 (1<<4) //P4
#define LCD_D5 (1<<5) //P5
#define LCD_D6 (1<<6) //P6
#define LCD_D7 (1<<7) //P7

/*---------------------------------------------------------------------------/
/ ADDRESSES
/---------------------------------------------------------------------------*/

/* 1602 LCD I2C ADDRESS */
//#define LCD_I2C_ADDRESS 0x27 //PCF8574T LCD Backpack
#define LCD_I2C_ADDRESS 0x20 //PCF8574T 9 Pin Board and PCF8574P
//#define LCD_I2C_ADDRESS 0x38 //PCF8574AN

#endif /* CONF */
