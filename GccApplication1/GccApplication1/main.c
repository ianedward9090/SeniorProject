//This really needs some sprucing up

typedef union{
	float f;
	char  s[4];
} datapoint;

#define F_CPU 8000000UL 
#define RED_BUTTON !(PINE & (1 << PE7))
#define BLACK_BUTTON !(PINE & (1 << PE6))
#define LIMIT2 !(PINE & (1 << PE2))
#define LIMIT3 !(PINE & (1 << PE3))

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
	int azimuth_motor = 0;
	int elevation_motor = 0;
	int azimuthlimit = 0;
	int elevationlimit = 0;
	int points = 0;
	int program = 0;
	int adcval;
	int address = 0;
	int wise = 0; //how many steps to take

	char buffer2[50];
	
	char *a[2]; //Points
	a[0] = "650 ";
	a[1] = "1275";
	
	char *programs[4]; //Programs
	programs[0] = "MemoryClear     ";
	programs[1] = "MemoryDump      ";
	programs[2] = "SunTracker      ";
	programs[3] = "Run SolarTracker";
	
	PORTC = 0; //for motor control
	DDRC = 0xff;
	DDRE = 0x00;
	PORTE = (1<<PE2) | (1<<PE3) | (1<<PE6) | (1<<PE7);//Pullup resistors
	
	unsigned int currentazimuth = 0; //keeping track of azimuth
	unsigned int currentelevation = 0;//keeping track of elevation
	unsigned int baud = 9600;
	unsigned int findazimuth = 0;
	unsigned int findelevation = 0;
	unsigned int i, j;
	
	float voltage,current,mw;
	float wiser = 3.6;
	float azimuthadd = 0;
	float azimuth = 0;
	float elevation = 0;

	//unsigned char ret;
	//char* buffer;
	_delay_ms(500);
	
	USART_Init (baud);
	USARTPC_Init();
	LCD_Init();
	TWI_init();
	_delay_ms(100);
	
	
	/*********MAIN CODE WILL START HERE***********/
	int azimuthrotate = 0;
	
	transmitstring("Program?",8);
	nextline();
	while(1){
		if (RED_BUTTON){
			program++;
			if(program>3){
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
					int case2points = 1;
					while(case2points){
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
							case2points = 0;
						}
					}
					int counter = a[points];
					while(counter >0 ){
						clearlcd();
						currentazimuth = rotate_relative_azimuth(currentazimuth, 1);
						currentelevation = rotate_relative_elevation(currentelevation, -1);
						transmitstring("Finding Sun",11);
					
						ADC_Init_B_Azimuth();
						ADC_ON();
						_delay_ms(100);
						adcval = ADC_READ();
					
						int sprint = sprintf(buffer2,"%i",adcval);
						transmitstring(buffer2,sprint);
						_delay_ms(200);
						while((adcval < 125 || adcval > 131) && azimuthlimit <= 100){
							clearlcd();
							findazimuth = rotate_relative_azimuth(findazimuth, 1);
							azimuthlimit += 1;
							adcval = ADC_READ();
							int sprint = sprintf(buffer2,"%i",adcval);
							transmitstring(buffer2,sprint);
							azimuth = azimuth + 1.8;
							_delay_ms(250);
						}
					
						_delay_ms(500);
						ADC_OFF();
						ADC_Init_B_Elevation();//Switch ADC
						ADC_ON();
						_delay_ms(100);
						adcval = ADC_READ();
					
						while((adcval < 125 || adcval > 131) && elevationlimit <= 50){
							clearlcd();
							findelevation = rotate_relative_elevation(findelevation, -1);
							elevationlimit += 1;
							adcval = ADC_READ();
							int sprint2 = sprintf(buffer2,"%i",adcval);
							transmitstring(buffer2,sprint2);
							elevation = elevation + 1.8;
							_delay_ms(250);
						}
					
						ADC_OFF();
						ADC_Init_C(); //Switch ADC
						ADC_ON();
						_delay_ms(100);
					
						adcval = ADC_READ(); //Do the datapoint
						voltage = .019297 * adcval;
						current = (voltage / 56.0) * 1000.0;
						mw = get_mw(adcval,current);
						address = EEPROM_write_datapoint(mw,azimuth,elevation,address);//write the datapoint
						int l = sprintf(buffer2,"%.0f,%.1f,%.1f",mw,azimuth,elevation);
						clearlcd();//Refresh LCD
						transmitstring(buffer2,l); //Show the point
					
						rotate_relative_elevation(findelevation, elevationlimit); //Move back
						rotate_relative_azimuth(findazimuth, -azimuthlimit); //Move back
					}
					
					break;
				}
				case 3:{
					// Record
					ADC_Init_C();
					clearlcd();
					transmitstring("Homing",6);
					//currentazimuth = Home_Azimuth();
					//currentelevation = Home_Elevation();
					//currentazimuth = rotate_relative_azimuth(currentazimuth, 1);
					//currentelevation = rotate_relative_elevation(currentelevation, -1);
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
							elevation = 0; //Clear Elevation
						}
						elevation += wiser;
						adcval = 0;
						current = 0;
						voltage = 0;
						mw = 0;
						adcval = ADC_READ();
						_delay_ms(350);
						
						voltage = .019297 * adcval;
						current = (voltage / 56.0) * 1000.0;
						if((adcval >= 58) && (adcval <= 117)){ //20-40 mA range
							mw = (3.5585 * current) + 164.825;
						}
						else if ((adcval >= 118) && (adcval<=173)){//40-60 mA range
							mw = (2.5585 * current) +204.825;
						}
						else if ((adcval >= 174) && (adcval<=203)){//60-70 mA range
							mw = (9.395 * current) - 205.259;
						}
						
						else if ((adcval >= 204) && (adcval<=219)){//70-75 mA range
							mw = (30.8519 * current) - 1707.81;
						}
						else if ((adcval >= 220) && (adcval<=233)){//75-80 mA range
							mw = (70.8519 * current) - 4707.81;
						}
						else if(adcval>233){//Above 80 mA range
							mw = (29.0466 * current) - 1359.89;
						}
						else if(adcval<58){//Below 20 mA range
							mw = (8.0532 * current) + 73.4447;
						}
						address = EEPROM_write_datapoint(mw,azimuth,elevation,address);//write the datapoint
						int l = sprintf(buffer2,"%.0f,%.1f,%.1f",mw,azimuth,elevation);//Place the points into buffer2
						clearlcd();//Refresh LCD
						transmitstring(buffer2,l);//Show Datapoint on LCD
						_delay_ms(400);
						if(BLACK_BUTTON){//To show our spec
							_delay_ms(20000);
							
						}
					}
					wise = -wise; //invert wise so motor doesn't go beyond limits
					}
					break;
				}
			}
		}
	}

	return 0;

}
