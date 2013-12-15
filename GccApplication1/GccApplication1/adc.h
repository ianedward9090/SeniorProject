#ifndef __ADC_H
#define __ADC_H
void ADC_Init_C(void);// Initialize C grade ADC
void ADC_ON(void);//Turn on ADC
int ADC_READ(void);//Read one value from the ADC
void ADC_Init_B_Azimuth(void);//Initialize B grade azimuth ADC
void ADC_Init_B_Elevation(void);//Initialize B grade elevation ADC
void ADC_OFF(void);//Turn off ADC
#endif