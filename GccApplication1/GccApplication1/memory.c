typedef union{
	float f;
	char  s[4];
} datapoint;

#include <avr/io.h>
#include "memory.h"

unsigned int i2c_start_proticol(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	while(!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) == START)			//Check value of TWI Status Register
		return 0;
	else
		return 1;
	
}

void stopi2c(void){
	TWCR =  (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //stop i2c communication
}

unsigned int i2c_send_address(unsigned char address){
	TWDR = address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return 0;
	 // Add some error checking here possible
	 
	
}


unsigned char i2c_send_data(unsigned char data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return 0;
}
void EEPROM_write_datapoint(float watts, float elevation, float azimuth){
	int i = 0;
	datapoint watts1;
	datapoint elevation1; 
	datapoint azimuth1;
	watts1.f = watts;
	elevation1.f = elevation;
	azimuth1.f = azimuth;
	
	for(i = 0; i < 4; i++){
		i2c_send_data(watts1.s[i]);
	}
	for(i = 0; i < 4; i++){
		i2c_send_data(elevation1.s[i]);
	}
	for(i = 0; i < 4; i++){
		i2c_send_data(azimuth1.s[i]);
	}
	
	stopi2c();
	
}
unsigned char EEPROM_address(unsigned char highAddress, unsigned char lowAddress){
	int error = 0;
	
	error = i2c_start_proticol();
	if(error == 1){
		stopi2c();
		return(1);
	}
	
	error = i2c_send_address(EEPROM_W);
	
	error = i2c_send_address(highAddress);
	
	error = i2c_send_address(lowAddress);
	
	
	
	return 0;
}


unsigned char EEPROM_read(unsigned char highAddress, unsigned char lowAddress, unsigned char totalChar){
	unsigned char error, i, data;
	i2c_start_proticol;
	i2c_send_address(EEPROM_W);
	i2c_send_data(highAddress);
	i2c_send_data(lowAddress);
	i2c_send_address(EEPROM_R);
	
	for(i = 0; i<totalChar;i++){
		if(i == (totalChar-1))
			data = i2c_receivedata_a();
		}
	stopi2c();
	return 0;
}


unsigned char i2c_receivedata_a(void){

	unsigned char data;
	TWCR = (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
	
	while (!(TWCR & (1<<TWINT)));
	
	data = TWDR;
	return(data);
}
//Here is where memory Lives. OK