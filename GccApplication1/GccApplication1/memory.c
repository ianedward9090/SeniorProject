//Code adapted from ATMEGA128 datasheet and EEPROM examples from Chennai Dharmani

typedef union{
	float f;
	char  s[4];
} datapoint;
//Unions allow one potion of memory to be accesses as different data types.

#define F_CPU 8000000UL 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "lcd.h"
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
unsigned int EEPROM_write_datapoint(float watts, float elevation, float azimuth,unsigned int address){
	int i = 0; //This function writes the entire datapoint to memory
	datapoint watts1;
	datapoint elevation1; 
	datapoint azimuth1;
	watts1.f = watts;
	elevation1.f = elevation;
	azimuth1.f = azimuth;
	char lowaddress = 0;
	char highaddress = 0;
	
	lowaddress = address & 0xff;
	highaddress = (address &0xff00)>>8;
	
	EEPROM_address(highaddress,lowaddress);
	
	

	for(i = 0; i < 4; i++){
		
		i2c_send_data(watts1.s[i]);//Send one char at a time
		stopi2c();
		_delay_ms(5);
		address++;//Next memory address
		lowaddress = address & 0xff;
		highaddress = (address &0xff00)>>8;
		EEPROM_address(highaddress,lowaddress);
	}
	for(i = 0; i < 4; i++){
		i2c_send_data(elevation1.s[i]);
		stopi2c();
		_delay_ms(5);
		address++;
		lowaddress = address & 0xff;
		highaddress = (address &0xff00)>>8;
		EEPROM_address(highaddress,lowaddress);
	}

	for(i = 0; i < 4; i++){
		i2c_send_data(azimuth1.s[i]);
		stopi2c();
		_delay_ms(5);
		address++;
		lowaddress = address & 0xff;
		highaddress = (address &0xff00)>>8;
		EEPROM_address(highaddress,lowaddress);
	}

	stopi2c();
	_delay_ms(5);
	return address;
}
unsigned char EEPROM_address(unsigned char highAddress, unsigned char lowAddress){
	int error = 0;
	//Proticol for starting the EEPROM addressing
	error = i2c_start_protocol();
	if(error == 1){
		transmitstring("error",5);
		stopi2c();
		return(1);
	}
	
	error = i2c_send_address(EEPROM_W);
	if(error == 1){
		transmitstring("error",5);
		stopi2c();
		return(1);
	}
	error = i2c_send_address(highAddress);
	/*if(error == 1){
		transmitstring("error",5);
		stopi2c();
		return(1);
	}*/
	
	error = i2c_send_address(lowAddress);
	
	/*if(error == 1){
		transmitstring("error",5);
		stopi2c();
		return(1);
	}*/
	
	return 0;
}


char* EEPROM_read(unsigned char highAddress, unsigned char lowAddress, unsigned int totalChar){
	unsigned char error, i;
	char* data = calloc(totalChar + 1, sizeof(char));
	EEPROM_address(highAddress,lowAddress);
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
		transmitstring("error",5);
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
		transmitstring("errors",6);
	
	data = TWDR;
	return(data);
}

unsigned char EEPROM_erase(void){
	_delay_ms(100);
	int i;
	int address = 0;
	for(i = 0; i<2700; i++){// Fill everything with 0's
		address = EEPROM_write_datapoint(0,0,0,address);
		_delay_ms(5);
	}
	transmitstring("Complete",8);//Show complete on the LCD
	stopi2c();
	_delay_ms(100);
	return 0;
}

void TWI_init(void){
	TWCR = 0x00;
	TWBR = 0x08;
	TWSR = (0<<TWPS0)|(0<<TWPS1);
	TWCR = 0x44; //01000100
}

void EEPROM_DUMP_POINT(char * buffer){
	datapoint DP; //This function puts the entire point out to serial
	char buffer2[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	DP.s[0] = buffer[0];	
	DP.s[1] = buffer[1];
	DP.s[2] = buffer[2];
	DP.s[3] = buffer[3];
	int l = sprintf(buffer2,"%.2f, ",DP.f);
	USART_putstring(buffer2,l);
	
	DP.s[0] = buffer[4];
	DP.s[1] = buffer[5];
	DP.s[2] = buffer[6];
	DP.s[3] = buffer[7];
	l = sprintf(buffer2,"%.1f, ",DP.f);
	USART_putstring(buffer2,l);
	
	DP.s[0] = buffer[8];
	DP.s[1] = buffer[9];
	DP.s[2] = buffer[10];
	DP.s[3] = buffer[11];
	l = sprintf(buffer2,"%.1f",DP.f);
	USART_putstring(buffer2,l);
}

void USARTPC_Init(void){
		unsigned int baudrate;
		baudrate = 51;
		UBRR1H = (unsigned char) (baudrate >> 8);
		UBRR1L = (unsigned char) baudrate;
		UCSR1B = (1 << RXEN1) | (1 << TXEN1);//|(1<<RXCIE0);
		UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
	
}

char USART_putchar(char tx){//Put out one char
	char good = 0;
	while (!(UCSR1A & (1<<UDRE1)));
	
	UDR1 = tx;
	
	good = 1;
	
	return good;
}

char USART_putstring(char* tx, unsigned int totalChar){
	char good = 0; //Calls put char until the string is done
	int i = 0;
	
	for(i=0;i<totalChar;i++){
		USART_putchar(tx[i]);
	}
	good = 1;
	return good;
}
