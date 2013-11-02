#ifndef __MEMORY_H
#define __MEMORY_H
#define	START 0x08
#define EEPROM_W 0xa0
#define EEPROM_R 0xa1
void stopi2c(void);
unsigned int i2c_start_protocol(void);
unsigned int i2c_send_address(unsigned char address);
unsigned char EEPROM_address(unsigned char high, unsigned char lowAddress);
void EEPROM_write_datapoint(float watts, float elevation, float azimuth);
unsigned char* EEPROM_read(unsigned char highAddress, unsigned char lowAddress, unsigned int totalChar);
unsigned char i2c_receivedata_a(void);
unsigned char EEPROM_erase(void);
void TWI_init(void);
unsigned int i2c_start_protocolrepeat(void);
unsigned char i2c_receiveData_NACK(void);
void EEPROM_display(unsigned char * buffer);
#endif