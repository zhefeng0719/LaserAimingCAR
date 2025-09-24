#ifndef __PID_H
#define __PID_H
#include "MySys.h"
void PID_YAW();
void PID_SteerCamerX();
void PID_SteerCamerY();
void PID_SteerYaw();
void PID_SteerPitch();
void PidPrc();
void FuzzyPrc();
float Kp_Fuzzy(float E ,float EC);
float Kd_Fuzzy(float EC);
#endif // __PID_H
