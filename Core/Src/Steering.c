#include "Steering.h"

extern Steer steer0,steer1;//舵机结构体

void SteerCtrl()//舵机控制函数 
{
   printf("{#%03dP%04dT%04d!#%03dP%04dT%04d!}",//不足位数需补位0，不然舵机解包不了
    steer0.ID,steer0.Pwm,steer0.time,
    steer1.ID,steer1.Pwm,steer1.time);
}