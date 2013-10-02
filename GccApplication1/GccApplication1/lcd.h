#ifndef __LCD_H
#define __LCD_H

void USART_Init (unsigned int ubrr);
void transmitchar(char data);
void transmitstring(char text[], unsigned char a);
int LCD_Init (void);
#endif