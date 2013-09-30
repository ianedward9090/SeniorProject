#define F_CPU 8000000UL 
#include "motor.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	int i=0;
	int j=0;
	PORTD = 0;
	DDRD = 0xff;
	unsigned int currentazimuth = 0;
	unsigned int currentelevation = 0;
	int wise = 2;

		for(i=0; i<51; i++){
			currentazimuth = rotate_relative_azimuth(currentazimuth, 2);
			_delay_ms(5);
		
			for(j=0;j<26;j++){
				currentelevation = rotate_relative_elevation(currentelevation, wise);
				_delay_ms(5);
			}
		
			wise = -wise;
		}
	PORTD = 0;
	return 0;

}