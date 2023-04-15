/*
 * main.c
 *
 * Created: 4/15/2023 9:01:20 AM
 *  Author: ANO
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD PORTC
#define RS 0
#define RW 1
#define E 2

#define SET_BIT(REG,BIT) (REG |= (1<<BIT))
#define RST_BIT(REG,BIT) (REG &= ~(1<<BIT))
#define GET_BIT(REG,BIT) ((REG & (1<<BIT))>>BIT)

void pulse();
void LCD_init();
void LCDcmd(unsigned char cmd);
void LCDdata(unsigned char data);
void LCDprint(const char *str);

int main(void)
{
    LCD_init();
	DDRA |= (1 << RW) | (1 << E);
	PORTA &= ~(1 << RW);
	PORTA &= ~(1 << E);

    LCDprint("hello friends"); 
    while (1){}
}

void pulse()
{
	PORTA = SET_BIT(PORTA,E);
	_delay_ms(1);
	PORTA = RST_BIT(PORTA,E);
}

void LCD_init()
{
	DDRC = 0xFF;
	
	DDRA = SET_BIT(DDRA,RS);
	DDRA = SET_BIT(DDRA,RW);
	DDRA = SET_BIT(DDRA,E);
	PORTA = RST_BIT(PORTA,E);
	_delay_ms(15);
	LCDcmd(0x38);   // 8 BIT Mode 2 lines
	LCDcmd(0x0E);   // start
	LCDcmd(0x06);	// move cursor to right
	LCDcmd(0x01);	// clear dis
	_delay_ms(5);
}

void LCDcmd(unsigned char cmd)
{
	PORTA = RST_BIT(PORTA,RS);
	PORTA = RST_BIT(PORTA,RW);
	LCD = cmd;
	pulse();
	_delay_ms(5);
}


void LCDdata(unsigned char data)
{
	PORTA = SET_BIT(PORTA,RS);
	PORTA = RST_BIT(PORTA,RW);
	LCD =  data;
	pulse();
	_delay_ms(5);
}


void LCDprint(const char *str)
{
    while (*str)
    {
        LCDdata(*str++);
		_delay_ms(50);
    }
}

