#define F_CPU 8000000UL 
#include "motor.h"
#include "lcd.h"
#include <avr/interrupt.h>
#include <util/delay.h>
int main(void)
{
	//int i=0;
	int j=0;
	PORTD = 0; //for motor control
	DDRD = 0xff;
	PORTE = 0;
	DDRE = 0xff;
	unsigned int currentazimuth = 0; //keeping track of azimuth
	unsigned int currentelevation = 0;//keeping track of elevation
	unsigned int baud = 9600;
	int wise = 2; //how many steps to take
	_delay_ms(500);
	USART_Init (baud);
	_delay_ms(300);
	LCD_Init();
	_delay_ms(100);
	while(1){
	transmitstring("YOUR MOM", 8);
	_delay_ms(500);
	}

		/*for(i=0; i<51; i++){
			currentazimuth = rotate_relative_azimuth(currentazimuth, 2); //rotate 180 degrees 
			_delay_ms(5);
		
			for(j=0;j<26;j++){
				currentelevation = rotate_relative_elevation(currentelevation, wise);//rotate 90 degrees 
				_delay_ms(5);
			}
		
			wise = -wise; //invert wise so motor doesnt go beyond limits
		}
	PORTD = 0;*/ //save power
		
	return 0;

}