#ifndef __MOTOR_H
#define __MOTOR_H

void clockwise(void); //Spin Motor Clockwise

void counterclockwise(void); //Spin Motor CounterClockwise

int rotate_relative(int current_state, int steps);
#endif