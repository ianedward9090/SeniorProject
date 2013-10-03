#ifndef __LCD_H
#define __LCD_H

void USART_Init (unsigned int ubrr);
void transmitchar(unsigned char data);
void transmitstring(char text[], int a);
unsigned char USART_Receive();
int LCD_Init (void);
#endif