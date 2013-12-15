#ifndef __LCD_H
#define __LCD_H
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


void USART_Init (unsigned int ubrr);//Initialize LCD USART
void transmitchar(char data);//Transmit a char to the LCD
void transmitstring(char text[], int length);//Transmit a string to LCD
void nextline(void);//Move LCD pointer to next line
void clearlcd(void);//Clear the LCD and reset

int LCD_Init (void);//Initialize the LCD
#endif