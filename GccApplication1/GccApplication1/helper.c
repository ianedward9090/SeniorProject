#include "helper.h"
#include <avr/interrupt.h> 
#include <avr/io.h>
#include <util/delay.h>
void clearmemory(void){
	EEPROM_erase();
}

void demo_memory(void){
		char* buffer = malloc(sizeof(char) * 1024);
		char bufferchar[20];
		int i;
		int u = 1;
		char buffer2[] ={7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
		for(i = 0; i<2700; i++){
			free(buffer);
			int l = sprintf(buffer2,"%u, ",((i*12)&0xff00)>>8);
			USART_putstring("Point # ",8);
			itoa (i,bufferchar,10);
			USART_putstring(bufferchar,strlen(bufferchar));
			USART_putstring(":   ",4);
			
			//USART_putstring(buffer2,l);
			//l = sprintf(buffer2,"%u, ",(i*12)&0xff);
			//USART_putstring(buffer2,l);
			//_delay_ms(10);
			buffer = EEPROM_read(((i*12)&0xff00)>>8,(i*12)&0xff,12);
			EEPROM_DUMP_POINT(buffer);
			USART_putchar('\n');
			USART_putchar('\r');
		}
}

void init_everything(void){
	USART_Init (9600);
	USARTPC_Init();
	ADC_Init();
	LCD_Init();
	TWI_init();
	
}

