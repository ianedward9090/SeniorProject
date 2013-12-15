/* Header file for all memory functions*/
#ifndef __MEMORY_H
#define __MEMORY_H
#define	START 0x08
#define EEPROM_W 0xa0
#define EEPROM_R 0xa1

void stopi2c(void);
unsigned int i2c_start_protocol(void); //Start the I2C module
unsigned int i2c_send_address(unsigned char address);//Send address of i2c
unsigned int i2c_start_protocolrepeat(void);//Repeat the start protocol
unsigned char i2c_receiveData_NACK(void);//Receive data with no acknowledge
unsigned char i2c_receivedata_a(void);//Receive data with acknowledge

unsigned char EEPROM_erase(void);//Erase the EEPROM
unsigned char EEPROM_address(unsigned char highAddress, unsigned char lowAddress); //Send High and Low address
unsigned int EEPROM_write_datapoint(float watts, float elevation, float azimuth,unsigned int address); //Write a datapoint
char* EEPROM_read(unsigned char highAddress, unsigned char lowAddress, unsigned int totalChar); //Read from memory address
void EEPROM_DUMP_POINT(char * buffer);//Dump out a point


void TWI_init(void); //Initialize Two Wire Interface

void USARTPC_Init(void); //Initialize Usart
char USART_putstring(char* tx, unsigned int totalChar);//Output a string
char USART_putchar(char tx);//Output a char

#endif