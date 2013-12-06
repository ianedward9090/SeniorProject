#include <avr/io.h>
#include "adc.h"
#include <avr/interrupt.h> 


void ADC_Init(void){
	//ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //prescaler of 32
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Prescaler of 128   8M/128 = 62.5k
	ADMUX |= (1 << REFS0); //Sets Reference to AVCC
	ADMUX |= (1 << ADLAR); //Turns the ADC basically into a 8 bit ADC from 10 BIT !!!!READ ADCH!!!!
	//ADCSRA |= (1 << ADFR); //free running mode 
}

void ADC_ON(void){
	ADCSRA |= (1 << ADEN); 
	ADCSRA |= (1 << ADSC); 
}

ISR(ADC_vect){

}

int ADC_READ(void){
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADC;
	
	}