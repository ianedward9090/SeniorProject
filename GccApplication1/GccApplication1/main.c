typedef union{
	float f;
	char  s[4];
} datapoint;

#define F_CPU 8000000UL 
#define EEPRO 0xA0

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
	//char buffer[20];
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
	unsigned char address = 0;
	unsigned int bobby = 10;
	char aaaa = 'a';
	float watts = 9000;
	float azimuth = 45;
	float elevation = 90;
	unsigned char ret;
	char* buffer;
	char buffer2[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	datapoint DP;
	_delay_ms(500);
	USART_Init (baud);
	//ADC_Init();
	LCD_Init();
	TWI_init();
	_delay_ms(100);
	transmitstring("Data Points:",12);
	nextline();
	

	//EEPROM_erase();
	EEPROM_address(45,0);
	EEPROM_write_datapoint(1,1,1);
	_delay_ms(50);
	buffer = EEPROM_read(45,0,12);
	DP.s[0] = buffer[0];
	DP.s[1] = buffer[1];
	DP.s[2] = buffer[2];
	DP.s[3] = buffer[3];
	
	int l = sprintf(buffer2,"%.2f, ",DP.f);
	
	transmitstring(buffer2,l);
	DP.s[0] = buffer[4];
	DP.s[1] = buffer[5];
	DP.s[2] = buffer[6];
	DP.s[3] = buffer[7];
	l = sprintf(buffer2,"%.0f, ",DP.f);
	transmitstring(buffer2,l);
	DP.s[0] = buffer[8];
	DP.s[1] = buffer[9];
	DP.s[2] = buffer[10];
	DP.s[3] = buffer[11];
	l = sprintf(buffer2,"%.0f",DP.f);
	transmitstring(buffer2,l);	

	
	
	stopi2c();
	
	
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