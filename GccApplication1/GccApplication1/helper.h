//Helper Functions if I ever need them
#ifndef __HELPER_H
#define __HELPER_H
void clearmemory(void);
void init_everything(void);
void demo_memory(void);
unsigned int Home_Azimuth(void);
unsigned int Home_Elevation(void);
float get_mw(int adcval, float current);
#endif