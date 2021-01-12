/* 
 * 1602 LCD I2C Driver
 *
 * LCD COMMANDS http://www.dinceraydin.com/lcd/commands.htm
 * HD44780 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/HD44780-Datasheet.pdf
 * PCF8574 DATASHEET https://www.makerguides.com/wp-content/uploads/2019/02/PCF8574-Datasheet.pdf
 * PCF8574T to HD44780 PINOUT https://www.playembedded.org/blog/hd44780-backpack-stm32/
 */

#include "lcd1602_i2c.h"

#define LCD_CMD_MODE 0x00
#define LCD_DATA_MODE 0x01

void lcd1602__write(unsigned char data, uint8_t mode) {
	
	uint8_t dataTemp;
	
	twi_start(LCD_I2C_ADDRESS);
    
    // Send upper nibble
	dataTemp = (data & 0xF0) | LCD_BL | LCD_E; //Loads upper nibble onto last four bits P4-P7
	if (mode == LCD_CMD_MODE)
    {
		dataTemp = (dataTemp & ~LCD_RS); //Set RS to 0 to signify command mode and Toggle E on -> When enable is toggled on, LCD knows to execute instructions given
    }        
	else
    {	
		dataTemp |= LCD_RS; //Set RS to 1 to signify character mode
    }  
	twi_write(dataTemp); //Send upper nibble while toggling E ON
    _delay_ms(1);
    dataTemp &= ~LCD_E; //Toggle E off
	twi_write(dataTemp); //Send "toggle E OFF"
    _delay_ms(1);
    
    // Send lower nibble 
	dataTemp = (data << 4) | LCD_BL | LCD_E; //Loads lower nibble onto last four bits P4-P7 and Toggle E on
	if (mode == LCD_CMD_MODE)
	{
    	dataTemp = (dataTemp & ~LCD_RS); //Set RS to 0 to signify command mode and Toggle E on -> When enable is toggled on, LCD knows to execute instructions given
	}
	else
	{
    	dataTemp |= LCD_RS; //Set RS to 1 to signify character mode
	}
    twi_write(dataTemp); //Lower nibble
    _delay_ms(1);
	dataTemp &= ~LCD_E; //Toggle E off
	twi_write(dataTemp); //Send "toggle E off"
    _delay_ms(1);
    
	twi_stop();
}

void lcd1602__cmd(unsigned char cmd)
{
	lcd1602__write(cmd, LCD_CMD_MODE);
}

void lcd1602__char(unsigned char data)
{
	lcd1602__write(data, LCD_DATA_MODE);
}

void lcd1602__hex(unsigned char data) //Displays byte in the form of two hex nibbles/digits
{
	unsigned char upperNibble, lowerNibble;
	upperNibble = (data & 0xF0) >> 4;
	if (upperNibble <= 9) {
		upperNibble += 48;
	}
	else {
		upperNibble += 55;
	}
	lowerNibble = data & 0x0F;
	if (lowerNibble <= 9) {
		lowerNibble += 48;
	}
	else {
		lowerNibble += 55;
	}
	lcd1602__char(upperNibble);
	lcd1602__char(lowerNibble);
}

/* LCD Initialize function, INITIALIZE LCD PORTS HERE! */
void lcd1602__init()
{	
	
	TCCR0A = (1<<WGM01); //Set CTC bit
	OCR0A = 156; //Timer ticks per .01s, 16MHz Clk/1024
	TIMSK0 = (1 << OCIE0A);
	TCCR0B = (1<<CS02) | (1<<CS00); //Set Clk/1024
	
	//LCD_DDR = 0xFF;			/* Make LCD port direction as o/p */ //4 bit mode, sets PORTD on MCU as output. Perhaps unnecessary in I2C since it's through SCL/SDA
	twi_init();
	
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
    // We send this three times so that we can configure 4-bit mode after soft reset
	lcd1602__cmd(0x02);		/* 1st send for 4 bit initialization of LCD  */
    _delay_ms(5);
    lcd1602__cmd(0x02);		/* 2nd send for 4 bit initialization of LCD  */
    _delay_ms(1);
    lcd1602__cmd(0x02);		/* 3rd send for 4 bit initialization of LCD  */
    _delay_ms(1); 
	lcd1602__cmd(0x28);              /* 2 line, 5 x 8 dot char font in 4-bit mode */
	lcd1602__cmd(0x0C);				/* Turns cursor off */
	lcd1602__cmd(0x01);              /* Clear display screen*/
    lcd1602__cmd(0x02);              /* Return home*/
}

void lcd1602__str(char *str)		/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		lcd1602__char (str[i]);
	}
}

void lcd1602__str_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	lcd1602__cmd((pos & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	lcd1602__cmd((pos & 0x0F)|0xC0);	/* Command of first row and required position<16 */
	lcd1602__str(str);		/* Call LCD string function */
}

void lcd1602__int(int num) {
	char buff[4];
	itoa(num, buff, 10);
	lcd1602__str(buff);
}

void lcd1602__clr()
{
	lcd1602__cmd (0x01);		/* Clear display */
	_delay_ms(2);
	lcd1602__cmd (0x80);		/* Cursor at home position */
}

void lcd1602__clr_ln(int lineNo){ //1st line = 0, 2nd line = 1
	if (lineNo == 0)
		lcd1602__cmd(0x80);
	else
		lcd1602__cmd(0xC0);
	lcd1602__str("                ");
	lcd1602__cmd(0x80);
}

void lcd1602__wake() {
	lcd1602__clr();
	lcd1602__cmd(0x80); //1st line
	
	for(int k = 177; k < 182; k++) { //177-182 to print A I U E O
		lcd1602__clr();
		lcd1602__cmd(0x80); //1st line
		for (int l = 0; l < 16; l++) {
			lcd1602__char(k);
			//_delay_ms(5);
		}
		lcd1602__cmd(0xC0); //2nd line
		for (int m = 0; m < 16; m++) {
			lcd1602__char(k);
			//_delay_ms(5);
		}
		_delay_ms(250); //Wait 350ms per character change
	}
	lcd1602__cmd(0x80); //1st line
	for (int i = 0; i < 16; i++) { //print black bars on first line
		lcd1602__char(0xFF);
		_delay_ms(10);
	}
	lcd1602__cmd(0xC0); //2nd line
	for (int j = 0; j < 16; j++) { //print black bars on second line
		lcd1602__char(0xFF);
		_delay_ms(10);
	}
	_delay_ms(500); //Wait a 500ms before clearing
	lcd1602__clr();
	lcd1602__cmd(0x83);
	lcd1602__str("LCD Ready!");
	_delay_ms(400);
	lcd1602__clr();

}