#define F_CPU 8000000UL 

#include "motor.h"
#include "lcd.h"
#include "memory.h"
#include "adc.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#define RED_BUTTON !(PINE & (1 << PE7))
#define BLACK_BUTTON !(PINE & (1 << PE6))

int main(void)
{
	int i=0;
	int j = 0;
	int points = 0;
	char *a[4];
	char buffer[20];
	a[0] = "100";
	a[1] = "200";
	a[2] = "500";
	a[3] = "900";
	PORTC = 0; //for motor control
	DDRC = 0xff;
	DDRE = 0x00;
	PORTE = (1<<PE6) | (1<<PE7);
	unsigned int currentazimuth = 0; //keeping track of azimuth
	unsigned int currentelevation = 0;//keeping track of elevation
	unsigned int baud = 9600;
	int wise = 2; //how many steps to take
	int adcval;
	char address = 0;
	float watts = 9000;
	float azimuth = 45;
	float elevation = 90;
	_delay_ms(500);
	USART_Init (baud);
	//ADC_Init();
	LCD_Init();
	_delay_ms(100);
	
	transmitstring("Data Points:",12);
	nextline();
	
	EEPROM_address(0,address);
	EEPROM_write_datapoint(watts,elevation,azimuth);
	
	transmitstring("complete!",9);
	
	
	
	
	//while(1){
		//if (RED_BUTTON){
			//transmitstring(a[points],3);
			//points++;
				//nextline();
				//if(points>3){
					 //points = 0;
				//} 
			//_delay_ms(1000);
			//} else {}
		//if(BLACK_BUTTON){
			//clearlcd();
			//_delay_ms(5);
			//transmitstring("Running!",8);
			//_delay_ms(2000);
			//break;
		//}
	//}
	/*ADC_ON();
	while(1){
		adcval = ADCH;
		itoa(ADCH,buffer,10);
		
		transmitstring(buffer,strlen(buffer));
		_delay_ms(5000);
		clearlcd();
	}*/
		
	
	//transmitstring("tits",4);

		//for(i=0; i<51; i++){
			//currentazimuth = rotate_relative_azimuth(currentazimuth, 2); //rotate 180 degrees 
			//_delay_ms(2);
		//
			//for(j=0;j<26;j++){
				//currentelevation = rotate_relative_elevation(currentelevation, wise);//rotate 90 degrees 
				//_delay_ms(2);
			//}
		//
			//wise = -wise; //invert wise so motor doesnt go beyond limits
		//}
	//PORTD = 0; //save power
		
	return 0;

}