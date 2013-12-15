//Helper Functions if I ever need them
#ifndef __HELPER_H
#define __HELPER_H
void clearmemory(void);//Erase Memory
void init_everything(void);//Initialize the modules
void demo_memory(void);//Dump the current memory

unsigned int Home_Azimuth(void);//Home azimuth
unsigned int Home_Elevation(void);//Home elevation

float get_mw(int adcval, float current); //Get MW 
#endif