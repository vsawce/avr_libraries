#ifndef LCD1602_I2C_H_
#define LCD1602_I2C_H_

/* 
 * 1602 LCD I2C Driver
 *
 * DEFINE CORRECT LCD I2C ADDRESS!!!
 *
 * LCD COMMANDS http://www.dinceraydin.com/lcd/commands.htm
 * HD44780 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/HD44780-Datasheet.pdf
 * PCF8574 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/PCF8574-Datasheet.pdf
 * PCF8574T to HD44780 PINOUT https://www.playembedded.org/blog/hd44780-backpack-stm32/
 */

//#define F_CPU 16000000UL //Defined in conf.h

#include "conf.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"

void lcd1602__write(unsigned char data, uint8_t mode); /* Writes byte via I2C. Mode is for LCD data transmission */
void lcd1602__cmd(unsigned char cmd); /* Sends byte to LCD in LCD_CMD_MODE mode */
void lcd1602__char(unsigned char data); /* Sends byte to LCD in LCD_DATA_MODE mode. Displays char */
void lcd1602__hex(unsigned char data); /* Displays two hex characters representing a byte */
void lcd1602__init(); /* Initializes LCD. Essential. */
void lcd1602__str(char *str); /* Uses LCD_char. Displays string. */
void lcd1602__str_xy (char row, char pos, char *str); /* Displays string at a certain row/col position */ 
void lcd1602__int(int num); /* Displays an integer given an integer as an argument */
void lcd1602__clr(); /* Clears LCD screen, resets cursor to start */
void lcd1602__wake(); /* Testing functionality for LCD */

#endif /* LCD1602_I2C_H_ */