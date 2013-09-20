#define F_CPU 8000000UL 
#include "motor.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	int i=0;
	
	PORTD = 0;
	DDRD = 0xff;
	int current = 0;

	while(1){
		//for(i=0; i<50;i++){
			
		//	clockwise();
			
		//}
		
		//for(i=0; i<50;i++){
		//	counterclockwise();

		//}

		current = rotate_relative(current, 200);
		_delay_ms(500);
		current = rotate_relative(current, 200);
		_delay_ms(500);
	}

	return 0;

}