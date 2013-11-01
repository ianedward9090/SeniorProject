typedef union{
	float f;
	char  s[4];
} datapoint;

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"


unsigned int i2c_start_protocol(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) == START)	{		//Check value of TWI Status Register
		return 0;
	}
	else{
		return 1;
	}
}

unsigned int i2c_start_protocolrepeat(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) == 0x10)	{		//Check value of TWI Status Register
		return 0;
	}
	else{
		return 1;
	}
}


void stopi2c(void){
	TWCR =  (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //stop i2c communication
	return;
}

unsigned int i2c_send_address(unsigned char address){
	unsigned char STATUS;
	if((address & 0x01) == 0)
		STATUS = 0x18;
	else
		STATUS = 0x40;
		
	TWDR = address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if((TWSR & 0xF8) == STATUS)
		return 0;
	else
		return 1;
	 // Add some error checking here possible
}


unsigned char i2c_send_data(unsigned char data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x28)   //Check value of TWI Status Register
	   return 1;
   else
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
	//_delay_ms(50);
	stopi2c();
	//_delay_ms(50);
}
unsigned char EEPROM_address(unsigned char highAddress, unsigned char lowAddress){
	int error = 0;
	
	error = i2c_start_protocol();
	if(error == 1){
		transmitstring("error",5);
		stopi2c();
		return(1);
	}
	
	error = i2c_send_address(EEPROM_W);
	
	error = i2c_send_address(highAddress);
	
	error = i2c_send_address(lowAddress);
	
	
	
	return 0;
}


unsigned char* EEPROM_read(unsigned char highAddress, unsigned char lowAddress, unsigned int totalChar){
	unsigned char error, i;
	unsigned char* data= calloc(totalChar + 1, sizeof(unsigned char));
	//data[totalChar]='/0';
	i2c_start_protocol();
	i2c_send_address(EEPROM_W);
	i2c_send_data(highAddress);
	i2c_send_data(lowAddress);
	//EEPROM_address(0x00,0x00);
	i2c_start_protocolrepeat();
	i2c_send_address(EEPROM_R);
for(i=0;i<totalChar;i++)
{
	if(i == (totalChar-1))  	 //no Acknowledge after receiving the last byte
	data[i] = i2c_receiveData_NACK();
	else
	data[i] = i2c_receivedata_a();
}
	stopi2c();
	return data;
}


unsigned char i2c_receivedata_a(void){

	unsigned char data;
	TWCR = (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
	
	while (!(TWCR & (1<<TWINT)));
	if((TWSR & 0xF8) != 0x50)
		transmitstring("balls",5);
	data = TWDR;
	return(data);
}

unsigned char i2c_receiveData_NACK(void)
{
	unsigned char data;
	
	TWCR = (1<<TWINT)|(1<<TWEN);
	
	while (!(TWCR & (1<<TWINT)));	   	   //Wait for TWINT flag set. This indicates that the
	//data has been received
	if ((TWSR & 0xF8) != 0x58)    //Check value of TWI Status Register
		transmitstring("balls2",6);
	
	data = TWDR;
	return(data);
}

unsigned char EEPROM_erase(void){
	unsigned int i;
	unsigned char error;
	
	error = i2c_start_protocol();
	if(error == 1)
	{
		transmitstring("error",5);
		stopi2c();
		return(1);
	}
	error = i2c_send_address(EEPROM_W);
		if(error == 1)
		{
			transmitstring("error1",6);
			stopi2c();
			return(1);
		}
	error = i2c_send_data(0x00);
		if(error == 1)
		{
			transmitstring("error2",6);
			stopi2c();
			return(1);
		}
	error = i2c_send_data(0x00);
		if(error == 1)
		{
			transmitstring("error3",6);
			stopi2c();
			return(1);
		}
	
	for(i = 0; i< 0x8000; i++){
		error = i2c_send_data(0x48);
			if(error == 1)
			{
				transmitstring("errorsend",9);
				stopi2c();
				return(1);
			}
		
		
	}
	stopi2c();
	return 0;
}

void TWI_init(void){
	TWCR = 0x00;
	TWBR = 0x08;
	TWSR = (0<<TWPS0)|(0<<TWPS1);
	TWCR = 0x44; //01000100
}
//Here is where memory Lives. OK