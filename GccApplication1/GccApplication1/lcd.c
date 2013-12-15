#define F_CPU 8000000UL

#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

void USART_Init (unsigned int baud){
	unsigned int baudrate;
	baudrate = 53; //Baudrate for this particular LCD
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);//|(1<<RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void transmitchar(char data){
	while(!(UCSR0A & (1 << UDRE0))){	
	}
	UDR0 = data;
}

void transmitstring(char text[], int length){
	
	int i;
	for(i=0; i < length; i++){
		transmitchar(text[i]);
	}
}

int LCD_Init (void){

	_delay_ms(200);
	transmitchar(SLCD_CONTROL_HEADER);
	transmitchar(SLCD_POWER_ON);
	_delay_ms(5);
	transmitchar(SLCD_INIT_ACK);
	_delay_ms(10);
	transmitchar(SLCD_CHAR_HEADER);
	_delay_ms(100);
return 0;
}

void nextline(void){ //Just a bunch of codes for a new line
	transmitchar(0x9F);
	transmitchar(0xFF);
	transmitchar(0x00);
	transmitchar(0x01);
	transmitchar(0xFE);
}


void clearlcd(void){ //Clear out the LCD
	transmitchar(0x9F);
	_delay_ms(5);
	transmitchar(0x65);
	_delay_ms(5);
	transmitchar(0x65);
	_delay_ms(5);
	transmitchar(0xFE);
}