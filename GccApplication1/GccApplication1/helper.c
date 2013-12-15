#define LIMIT2 !(PINE & (1 << PE2))
#define LIMIT3 !(PINE & (1 << PE3))
#define F_CPU 8000000UL 

#include "helper.h"
#include "memory.h"
#include "adc.h"
#include "lcd.h"
#include "motor.h"
#include <avr/interrupt.h> 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void demo_memory(void){
	char* buffer = malloc(sizeof(char) * 1024);
	char bufferchar[20];
	int i;
	char buffer2[] ={7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
	for(i = 0; i<2700; i++){
		free(buffer);
		sprintf(buffer2,"%u, ",((i*12)&0xff00)>>8);
		USART_putstring("Point # ",8);
		itoa (i,bufferchar,10);
		USART_putstring(bufferchar,strlen(bufferchar));
		USART_putstring(":   ",4);
		buffer = EEPROM_read(((i*12)&0xff00)>>8,(i*12)&0xff,12);
		EEPROM_DUMP_POINT(buffer); //Send a point out serially
		USART_putchar('\n');//Newline
		USART_putchar('\r');//Carriage return
	}
}

void init_everything(void){ //All the initialization routines
	USART_Init (9600);
	USARTPC_Init();
	ADC_Init_C();
	LCD_Init();
	TWI_init();
	
}

unsigned int Home_Azimuth(void){
	unsigned int currentazimuth = 0;
	while(!LIMIT2){//Check to see if limit is pushed
		currentazimuth = rotate_relative_azimuth(currentazimuth, 1);
		_delay_ms(300);
	}
	return currentazimuth;
}

unsigned int Home_Elevation(void){
	unsigned int currentelevation = 0;
	while(!LIMIT3){//Check to see it limit is pushed
		currentelevation = rotate_relative_elevation(currentelevation, -1);
		_delay_ms(300);
	}
	return currentelevation;
}

float get_mw(int adcval, float current){
	float mw = 0;
	if((adcval >= 58) && (adcval <= 117)){ //20-40 mA range
		mw = (3.5585 * current) + 164.825;
	}
	else if ((adcval >= 118) && (adcval<=173)){//40-60 mA range
		mw = (2.5585 * current) +204.825;
	}
	else if ((adcval >= 174) && (adcval<=203)){//60-70 mA range
		mw = (9.395 * current) - 205.259;
	}
							
	else if ((adcval >= 204) && (adcval<=219)){//70-75 mA range
		mw = (30.8519 * current) - 1707.81;
	}
	else if ((adcval >= 220) && (adcval<=233)){//75-80 mA range
		mw = (70.8519 * current) - 4707.81;
	}
	else if(adcval>233){//Above 80 mA range
		mw = (29.0466 * current) - 1359.89;
	}
	else if(adcval<58){//Below 20 mA range
		mw = (8.0532 * current) + 73.4447;
	}
	return mw;
}