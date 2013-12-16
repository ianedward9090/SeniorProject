#define F_CPU 8000000UL 

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "motor.h"

void clockwise(void){//Test function not used anymore

	PORTC = 0b00001010;
	_delay_ms(5);
	PORTC = 0b00001001;
	_delay_ms(5);
	PORTC = 0b00000101;
	_delay_ms(5);
	PORTC = 0b00000110;
	_delay_ms(5);
}

void counterclockwise(void){// Test functions not ever used anymore
	PORTC = 0b00000110;
	_delay_ms(5);
	PORTC = 0b00000101;
	_delay_ms(5);
	PORTC = 0b00001001;
	_delay_ms(5);
	PORTC = 0b00001010;
	_delay_ms(5);
}

unsigned int rotate_relative_azimuth(int current_state, int steps){
	unsigned int states[] = {0b00000110, 0b00000101, 0b00001001, 0b00001010};
	unsigned int next_state = current_state;
	unsigned int current_upper_PORTC = 0;
	
	int i;
	/*Move A set amount of steps in a direction*/
	for (i = 0; i < abs(steps); i++) { 
		if (steps < 0) {
			next_state = current_state - 1;
		} else if (steps > 0) {
			next_state = current_state + 1;
		}
		
		// Preserve the upper four bits
		current_upper_PORTC = PORTC&0xF0;
		
		
		// Set the lower four bits according to state
		PORTC = states[next_state%4] | current_upper_PORTC;
		current_state = next_state%4;
		
		// Delay 10ms between steps
		_delay_ms(10);//Can go smaller if needed
	}
	//PORTD = 0;
	return current_state; //So the next call wont skip a step
}


unsigned int rotate_relative_elevation(int current_state, int steps){
	unsigned int states[] = {0b01100000, 0b01010000, 0b10010000, 0b10100000}; //See motor documentation
	unsigned int next_state = current_state;
	unsigned int current_lower_PORTC = 0;
	
	int i;
	for (i = 0; i < abs(steps); i++) {
		if (steps < 0) {
			next_state = current_state - 1;
			} else if (steps > 0) {
			next_state = current_state + 1;
		}
		
		// Preserve the lower four bits
		current_lower_PORTC = PORTC&0x0F;
		
		
		// Set the upper four bits according to state
		PORTC = states[next_state%4] | current_lower_PORTC;
		current_state = next_state%4;
		
		// Delay 10ms between steps
		_delay_ms(10);
	}
	//PORTD = 0; Motor needs to be on to hold position
	return current_state;
}
