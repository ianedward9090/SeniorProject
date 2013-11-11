typedef union{
	float f;
	char  s[4];
} datapoint;

#define F_CPU 8000000UL 
#define RED_BUTTON !(PINE & (1 << PE7))
#define BLACK_BUTTON !(PINE & (1 << PE6))

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


int main(void)
{
	char lowaddress = 0;
	char highaddress = 0;
	int azimuth_motor = 0;
	int elevation_motor = 0;
	unsigned int i,j;
	int points = 0;
	int program = 0;
	char *a[2];
	a[0] = "650 ";
	a[1] = "1275";
	char *programs[3];
	programs[0] = "MemoryClear     ";
	programs[1] = "MemoryDump      ";
	programs[2] = "Run SolarTracker";
	char bufferadc[20];
	PORTC = 0; //for motor control
	DDRC = 0xff;
	DDRE = 0x00;
	PORTE = (1<<PE6) | (1<<PE7);
	
	unsigned int currentazimuth = 0; //keeping track of azimuth
	unsigned int currentelevation = 0;//keeping track of elevation
	unsigned int baud = 9600;
	
	float voltage,current,mw;
	
	int wise = 0; //how many steps to take
	float wiser = 3.6;
	float azimuthadd = 0;
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
	int azimuthrotate = 0;
	
	transmitstring("Program?",8);
	nextline();
	while(1){
		if (RED_BUTTON){
			program++;
			if(program>2){
				program = 0;
			}
			nextline();
			transmitstring(programs[program],strlen(programs[program]));
			_delay_ms(700);
		}
		if(BLACK_BUTTON){
			switch(program){
				case 0:
					// Clear memory
					clearlcd();
					transmitstring("Erasing!",8);
					EEPROM_erase();
					clearlcd();
					transmitstring("ERASED!",7);
					break;
				case 1:
					// Dump memory
					clearlcd();
					transmitstring("Dumping Memory",14);
					demo_memory();
					clearlcd();
					transmitstring("DUMPED!",6);
					break;
				case 2:{
					// Record
					currentazimuth = rotate_relative_azimuth(currentazimuth, 1);
					currentelevation = rotate_relative_elevation(currentelevation, -1);
					_delay_ms(700);
					clearlcd();
					transmitstring("Points?",7);
					nextline();
					int selecting_point = 1;
					while(selecting_point){
						if(RED_BUTTON){
							points++;
							if(points>1){
								points = 0;
							}
							nextline();
							transmitstring(a[points],strlen(a[points]));
							// debouncing
							_delay_ms(700);
						}
						if(BLACK_BUTTON){
							switch(points){
								case 0:
									clearlcd();
									azimuth_motor = 26;
									elevation_motor = 25;
									azimuthadd = 7.2;
									wise = 2;
									azimuthrotate = 4;
									selecting_point = 0;
									break;
								case 1:
									clearlcd();
									azimuth_motor = 51;
									elevation_motor = 25;
									azimuthadd = 3.6;
									wise = 2;
									azimuthrotate = 2;
									selecting_point = 0;
									break;							
							}
						}
					}
					azimuth = 0;
					elevation = 0;
					ADC_ON();
			
					for(i = 0; i < azimuth_motor; i++){
						if(i != 0 ){
							wiser = -wiser;
							currentazimuth = rotate_relative_azimuth(currentazimuth, azimuthrotate); //rotate 180 degrees
							azimuth = azimuth + azimuthadd;
						}
					
					for(j = 0;j < elevation_motor; j++){
						currentelevation = rotate_relative_elevation(currentelevation, wise);//rotate 90 degrees
						
						if(elevation > 0 && elevation <1){
							elevation = 0;
						}
						elevation += wiser;
						adcval = 0;
						adcval = ADC_READ();
						_delay_ms(250);
						
						voltage = .01921875 * adcval;
						current = (voltage / 56.0)*1000.0;
						if((adcval >= 58) && (adcval <= 117)){ //20-64 mA range
							mw = (3.5585 * current) + 184.825;
						}
						else if ((adcval >= 118) && (adcval<=204)){
							mw = (10.395 * current) - 115.259;
						}
						else if ((adcval >= 205) && (adcval<=233)){
							mw = (30.8519 * current) - 1507.81;
						}
						else if(adcval>233){
							mw = (26.0466 * current) - 1119.89;
						}
						else if(adcval<58){
							mw = (12.0532 * current) + 33.4447;
						}
						//itoa(mw,bufferadc,10);
						address = EEPROM_write_datapoint(mw,azimuth,elevation,address);
						int l = sprintf(buffer2,"%.0f,%.1f,%.1f",mw,azimuth,elevation);
						clearlcd();
						transmitstring(buffer2,l);
						_delay_ms(400);
						if(BLACK_BUTTON){
							_delay_ms(20000);
							
						}
					}
					
					wise = -wise; //invert wise so motor doesnt go beyond limits
					}
					break;
				}
			}
		}
	}

	return 0;

}
