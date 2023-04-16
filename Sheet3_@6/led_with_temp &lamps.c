/*
 * main.c
 *
 * Created: 4/15/2023 9:01:20 AM
 *  Author: ANO
 */ 

#define F_CPU 16000000UL
#define SET_BIT(REG,BIT) (REG |= (1<<BIT))
#define RST_BIT(REG,BIT) (REG &= ~(1<<BIT))
#define GET_BIT(REG,BIT) ((REG & (1<<BIT))>>BIT)

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define LCD PORTC
#define control_port PORTD
#define RS 5
#define RW 6
#define E 7
#define green_pin 0
#define yellow_pin 1
#define red_pin 2


unsigned int data;
void pulse();
void LCD_init();
void LCDcmd(unsigned char cmd);
void LCDdata(unsigned char data);
void LCDprint(const char *str);
void adc_init();
unsigned int adc_read(unsigned int channel);

int main(void)
{
    LCD_init();
    LCDprint("temp: "); 
	SET_BIT(DDRA,0);
	SET_BIT(DDRD,green_pin);
	SET_BIT(DDRD,yellow_pin);
	SET_BIT(DDRD,red_pin);
	adc_init();
	
	
    while (1)
	{
		data = adc_read(0);
		float temperature = (data * 5 / 1024.0) * 100.0;
		char ASCi[16];
		sprintf(ASCi, "%.2f", temperature);
		LCDprint(ASCi);
		_delay_ms(500);
		LCDcmd(0x01);
		
		if (temperature <=25) 
			{
			 SET_BIT(PORTD, green_pin);
			 RST_BIT(PORTD,yellow_pin);
			 RST_BIT(PORTD,red_pin);
			}
			 else if (temperature >= 26 && temperature <70) 
			 {
				 RST_BIT(PORTD, green_pin);
				 SET_BIT(PORTD,yellow_pin);
				 RST_BIT(PORTD,red_pin);
			 }
			 else if (temperature >= 70)
			 {
				RST_BIT(PORTD, green_pin);
				RST_BIT(PORTD,yellow_pin);
				SET_BIT(PORTD,red_pin);
			 }
		
		
		
		
		
	}
}

void pulse()
{
	SET_BIT(control_port,E);
	_delay_ms(1);
	 RST_BIT(control_port,E);
}

void LCD_init()
{
	DDRC = 0xFF;
	
	SET_BIT(DDRD,RS);
	SET_BIT(DDRD,RW);
	SET_BIT(DDRD,E);
	RST_BIT(control_port,E);
	_delay_ms(15);
	LCDcmd(0x38);   // 8 BIT Mode 2 lines
	LCDcmd(0x0E);   // start
	LCDcmd(0x06);	// move cursor to right
	LCDcmd(0x01);	// clear dis
	_delay_ms(200);
}

void LCDcmd(unsigned char cmd)
{
	 RST_BIT(control_port,RS);
	 RST_BIT(control_port,RW);
	 LCD = cmd;
	pulse();
	_delay_ms(5);
}


void LCDdata(unsigned char data)
{
	SET_BIT(control_port,RS);
	RST_BIT(control_port,RW);
	LCD =  data;
	pulse();
	_delay_ms(20);
}


void LCDprint(const char *str)
{
    while (*str)
    {
        LCDdata(*str++);
		_delay_ms(10);
    }
}

void adc_init(){
	//choosing the second mode (VCC refrence)
	ADMUX|=(1<<REFS0);
	// ADMUX|=(1<<ADLAR);
	//ADC enable and prescale it to 128
	ADCSRA|=(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0)| (1<<ADPS2);
}

unsigned int adc_read(unsigned int channel){
	ADMUX|=channel;
	SET_BIT(ADCSRA,ADSC);
	while((ADCSRA&(1<<ADIF))==0);
	SET_BIT(ADCSRA,ADIF);
	return ADC;

}

