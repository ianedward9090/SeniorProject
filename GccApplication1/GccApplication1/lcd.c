#define UART_READY 0xA3
#define SLCD_CONTROL_HEADER 0x9F
#define SLCD_CHAR_HEADER 0xFE
#define SLCD_CURSOR_HEADER 0xFF
#define SLCD_CURSOR_ACK 0x5A
#define SLCD_POWER_ON 0x83
#define SLCD_INIT_ACK 0xA5
#define SLCD_INIT_DONE 0xAA
#define SLCD_BACKLIGHT_ON 0x81

#define BAUD 9600
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>

void USART_Init (unsigned int baud){
	unsigned int baudrate;
	
	baudrate = (unsigned int)(8000000/baud/16-1);
	UBRR0H = (unsigned char)(baudrate>>8);
	UBRR0L = (unsigned char)baudrate;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);//|(1<<RXCIE0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ0);
	
	
}

void transmitchar(char data){
	
	while(!(UCSR0A & (1 << UDRE0))){
		
	}
	
	UDR0 = data;
	
}

void transmitstring(char text[], unsigned char a){
	
	int i;
	for(i=0; i < a; i++){
		transmitchar(text[i]);
	}
	
}

int LCD_Init (void){
	_delay_ms(250);
	transmitchar(SLCD_CONTROL_HEADER);
	transmitchar(SLCD_POWER_ON);
	transmitchar(SLCD_INIT_ACK);
	//transmitchar(SLCD_BACKLIGHT_ON);
	//transmitchar(0x67);
	_delay_ms(150);
	transmitchar(SLCD_CHAR_HEADER);
return 0;
}


