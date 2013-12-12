#include <avr/io.h>
#include "adc.h"
#include <avr/interrupt.h> 


void ADC_Init_C(void){
	//ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //prescaler of 32
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Prescaler of 128   8M/128 = 62.5k
	ADMUX |= (1 << REFS0); //Sets Reference to AVCC
	ADMUX |= (1 << ADLAR); //Turns the ADC basically into a 8 bit ADC from 10 BIT !!!!READ ADCH!!!!
	ADMUX &= (~(1 <<MUX0));
	ADMUX &= (~(1 <<MUX1));
	//ADCSRA |= (1 << ADFR); //free running mode 
}

void ADC_Init_B_Azimuth(void){
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Prescaler of 128   8M/128 = 62.5k
	ADMUX |= (1 << REFS0); //Sets Reference to AVCC
	ADMUX |= (1 << ADLAR); //Turns the ADC basically into a 8 bit ADC from 10 BIT !!!!READ ADCH!!!!
	ADMUX |= (1 << MUX0);
}
void ADC_Init_B_Elevation(void){
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Prescaler of 128   8M/128 = 62.5k
	ADMUX |= (1 << REFS0); //Sets Reference to AVCC
	ADMUX |= (1 << ADLAR); //Turns the ADC basically into a 8 bit ADC from 10 BIT !!!!READ ADCH!!!!
	ADMUX |= (1 << MUX1);
	ADMUX &= (~(1 <<MUX0));
}

void ADC_ON(void){
	ADCSRA |= (1 << ADEN); //Enable
	ADCSRA |= (1 << ADSC); 
}

int ADC_READ(void){
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADCH;
	
}

void ADC_OFF(void){
	ADCSRA &= (~(1 <<ADEN)); //Disable
}