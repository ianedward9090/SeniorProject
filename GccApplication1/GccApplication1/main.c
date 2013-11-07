typedef union{
	float f;
	char  s[4];
} datapoint;

#define F_CPU 8000000UL 

#include "motor.h"
#include "lcd.h"
#include "memory.h"
#include "adc.h"
#include "helper.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>


#define RED_BUTTON !(PINE & (1 << PE7))
#define BLACK_BUTTON !(PINE & (1 << PE6))

int main(void)
{
	char lowaddress = 0;
	char highaddress = 0;
	
	unsigned int i,j;
	int points = 0;
	int program = 0;
	char *a[4];
	char *programs[3];
	programs[0] = "MemoryClear     ";
	programs[1] = "MemoryDump      ";
	programs[2] = "Run SolarTracker";
	char bufferadc[20];
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
	
	float voltage,current,mw;
	
	int wise = 2; //how many steps to take
	float wiser = 3.6;
	int adcval;
	//unsigned char address = 0;
	float watts, azimuth, elevation;

	unsigned char ret;
	char* buffer;
	_delay_ms(500);
	
	USART_Init (baud);
	USARTPC_Init();
	ADC_Init();
	LCD_Init();
	TWI_init();
	_delay_ms(100);

	
	char buffer2[50];
	//EEPROM_erase();
	//EEPROM_address(50,0);
	//EEPROM_write_datapoint(66357,55,57);
	//demo_memory();

	//buffer = EEPROM_read(50,0,12);
	//EEPROM_display(buffer);
	int address = 0;

		//address = EEPROM_write_datapoint(i,i*2,i*4,address);
		//_delay_ms(10);

	//demo_memory();
	
	
	//stopi2c();
	
	/*********MAIN CODE WILL START HERE***********/
	//char buffer2[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	transmitstring("Program?",8);
	nextline();
	while(1){
		if (RED_BUTTON){
			transmitstring(programs[program],strlen(programs[program]));
			program++;
			nextline();
			if(program>2){
				program = 0;
			}
			_delay_ms(1000);
			} else {}
			if(BLACK_BUTTON){
				switch(program){
					case 1:
						clearlcd();
						transmitstring("Erasing!",8);
						EEPROM_erase();
						clearlcd();
						transmitstring("ERASED!",7);
						break;
					case 2: 
						clearlcd();
						transmitstring("Dumping Memory",14);
						demo_memory();
						clearlcd();
						transmitstring("DUMPED!",6);
						break;
					case 0:{
						azimuth = 0;
						elevation = 0;
						ADC_ON();
						for(i = 0; i < 51; i++){
							if(i != 0 ){
								wiser = -wiser;
								currentazimuth = rotate_relative_azimuth(currentazimuth, 2); //rotate 180 degrees
								azimuth = azimuth + 3.6;
							}
							//currentazimuth = rotate_relative_azimuth(currentazimuth, 2); //rotate 180 degrees
							//azimuth = azimuth + 3.8;

						
						for(j=0;j<25;j++){
							currentelevation = rotate_relative_elevation(currentelevation, wise);//rotate 90 degrees
							if(elevation > 0 && elevation <1){
								elevation = 0;
							}
							elevation += wiser;
							adcval = ADC_READ();
							_delay_ms(500);
							voltage = .019531 * adcval;
							current = (voltage / 56.0)*1000.0;
							mw = (18.616 * current) + 15.672;
							itoa(mw,bufferadc,10);
							address = EEPROM_write_datapoint(mw,azimuth,elevation,address);
							int l = sprintf(buffer2,"%.2f,%.1f,%.1f",mw,azimuth,elevation);
							clearlcd();
							transmitstring(buffer2,l);
							_delay_ms(1000);
						}
						
						wise = -wise; //invert wise so motor doesnt go beyond limits
						}
						break;
					}
				}
			}
		}

	//PORTD = 0; //save power
			//while(1){
			//voltage = 0;
			//current = 0;
			//mw = 0;
			//adcval = ADC_READ();
			//voltage = .019531 * adcval;
			//current = (voltage / 56)*1000;
			//mw = (18.616 * current) + 15.672;
			//
			//itoa(mw,buffer,10);
			//
			//transmitstring(buffer,strlen(buffer));
			//_delay_ms(2500);
			//clearlcd();
			//
			//}
	return 0;

}