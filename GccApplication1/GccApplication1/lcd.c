#define UART_READY 0xA3
#define SLCD_CONTROL_HEADER 0x9F
#define SLCD_CHAR_HEADER 0xFE
#define SLCD_CURSOR_HEADER 0xFF
#define SLCD_CURSOR_ACK 0x5A
#define SLCD_POWER_ON 0x83
#define SLCD_POWER_OFF 0x82
#define SLCD_INIT_ACK 0xA5
#define SLCD_INIT_DONE 0xAA
#define SLCD_BACKLIGHT_ON 0x81
#define SLCD_BLINK_ON 0x69 
#define SLCD_CURSOR_ON 0x67 

#define F_CPU 8000000UL
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

void USART_Init (unsigned int baud){
	unsigned int baudrate;
	baudrate = 53;
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);//|(1<<RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	
}

void transmitchar(unsigned char data){
	
	while(!(UCSR0A & (1 << UDRE0))){
		
	}
	
	UDR0 = data;
	
}

void transmitstring(char text[], int a){
	
	int i;
	for(i=0; i < a; i++){
		transmitchar(text[i]);
	}
	
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

int LCD_Init (void){

	_delay_ms(200);
	transmitchar(SLCD_CONTROL_HEADER);
	transmitchar(SLCD_POWER_ON);
	_delay_ms(5);
	transmitchar(SLCD_INIT_ACK);
	_delay_ms(10);
	//transmitchar(SLCD_BACKLIGHT_ON);
	_delay_ms(10);
	transmitchar(SLCD_CHAR_HEADER);
	_delay_ms(100);
return 0;
}

void nextline(void){
	
	transmitchar(0x9F);
	transmitchar(0xFF);
	transmitchar(0x00);
	transmitchar(0x01);
	transmitchar(0xFE);
}


