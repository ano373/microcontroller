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
#define control_port PORTD
#define RS 5
#define RW 6
#define E 7

#define SET_BIT(REG,BIT) (REG |= (1<<BIT))
#define RST_BIT(REG,BIT) (REG &= ~(1<<BIT))
#define GET_BIT(REG,BIT) ((REG & (1<<BIT))>>BIT)

void pulse();
void LCD_init();
void LCDcmd(unsigned char cmd);
void LCDdata(unsigned char data);
void LCDprint(const char *str);
void adc_init();
int main(void)
{
    LCD_init();
	

    LCDprint("hello friends"); 
    while (1){}
}

void pulse()
{
	control_port = SET_BIT(control_port,E);
	_delay_ms(1);
	control_port = RST_BIT(control_port,E);
}

void LCD_init()
{
	DDRC = 0xFF;
	
	DDRD = SET_BIT(DDRD,RS);
	DDRD = SET_BIT(DDRD,RW);
	DDRD = SET_BIT(DDRD,E);
	control_port = RST_BIT(control_port,E);
	_delay_ms(15);
	LCDcmd(0x38);   // 8 BIT Mode 2 lines
	LCDcmd(0x0E);   // start
	LCDcmd(0x06);	// move cursor to right
	LCDcmd(0x01);	// clear dis
	_delay_ms(5);
}

void LCDcmd(unsigned char cmd)
{
	control_port = RST_BIT(control_port,RS);
	control_port = RST_BIT(control_port,RW);
	LCD = cmd;
	pulse();
	_delay_ms(5);
}


void LCDdata(unsigned char data)
{
	control_port = SET_BIT(control_port,RS);
	control_port = RST_BIT(control_port,RW);
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

void adc_init(){
	//choosing the second mode (VCC refrence)
	ADMUX|=(1<<REFS0);

	ADMUX|=(1<<ADLAR);
	//ADC enable and prescale it to 8
	ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0)| (1<<ADPS2);
}

