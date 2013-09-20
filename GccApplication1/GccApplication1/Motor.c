#define F_CPU 8000000UL 

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "motor.h"

//#define RED1   PORTB0
//#define BLUE1  PORTB1
//#define BLACK1 PORTB2
//#define WHITE1 PORTB3

//#define RED2   _BV(PB4)
//#define BLUE2  _BV(PB5)
//#define BLACK2 _BV(PB6)
//#define WHITE2 _BV(PB7)

void clockwise(void)
{
	PORTD = 0b00001010;
	_delay_ms(5);
	PORTD = 0b00001001;
	_delay_ms(5);
	PORTD = 0b00000101;
	_delay_ms(5);
	PORTD = 0b00000110;
	_delay_ms(5);
}

void counterclockwise(void)
{
	PORTD = 0b00000110;
	_delay_ms(5);
	PORTD = 0b00000101;
	_delay_ms(5);
	PORTD = 0b00001001;
	_delay_ms(5);
	PORTD = 0b00001010;
	_delay_ms(5);
}

int rotate_relative(int current_state, int steps) {
	int states[] = {0b00000110, 0b00000101, 0b00001001, 0b00001010};
	int next_state = current_state;
	int current_upper_PORTD = 0;
	
	int i;
	for (i = 0; i < abs(steps); i++) {
		if (steps < 0) {
			next_state = current_state - 1;
		} else if (steps > 0) {
			next_state = current_state + 1;
		}
		
		// Preserve the upper four bits
		current_upper_PORTD = PORTD&0xF0;
		
		
		// Set the lower four bits according to state
		PORTD = states[next_state%4] | current_upper_PORTD;
		current_state = next_state%4;
		
		// Delay 10ms between steps
		_delay_ms(10);
	}
	
	return current_state;
}

