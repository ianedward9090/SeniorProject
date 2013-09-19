#ifndef __MOTOR_H
#define __MOTOR_H

int clockwise(void); //Spin Motor Clockwise

int counterclockwise(void); //Spin Motor CounterClockwise

int rotate_relative(int current_state, int steps);
#endif