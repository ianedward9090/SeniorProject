#include "motor.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	int i=0;
	
	PORTD = 0;
	DDRD = 0xff;


	while(1){
		for(i=0; i<10;i++){
			clockwise();
			

		}
		_delay_ms(300);

		for(i=0; i<10;i++){
			counterclockwise();

		}


	}

	return 0;

}