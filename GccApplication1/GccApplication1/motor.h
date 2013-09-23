#ifndef __MOTOR_H
#define __MOTOR_H

void clockwise(void); //Spin Motor Clockwise

void counterclockwise(void); //Spin Motor CounterClockwise

unsigned int rotate_relative_azimuth(int current_state, int steps);
unsigned int rotate_relative_elevation(int current_state, int steps);

#endif