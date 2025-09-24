#ifndef __MYSYS_H
#define __MYSYS_H
#include "stm32f4xx.h"                  // Device header
#include "cmsis_os2.h"
#include <stdlib.h>
#include "stdio.h"
#include <math.h>
#include <string.h>
#include "Imu.h"
#include "Camera.h"
#include "OLED.h"
#include "Encoder.h"
#include "Motor.h"
#include "Key.h"
#include "Display.h"
#include "BlueTooth.h"
#include "Steering.h"
#include "Grey.h"
#include "PID.h"
#include "Scan.h"
#include "FOC.h"
#include <stdbool.h>
#define CARMODE 2  //小车模式 1:MEC_CAR 2:FOURWHEEL_CAR
#define UART2MODE 1 //串口2模式 0:N10P雷达 1:STP激光测距仪
#define UART5MODE 1 //串口5模式 0:K230 1:FOC驱动板
#define UART4MODE 1//串口4模式 0:舵机IMU 1:DL20
#define PI                 3.14159265358979f
#define LIMIT(x, min, max)  do { \
    if ((x) < (min)) (x) = (min); \
    else if ((x) > (max)) (x) = (max); \
} while(0)
//舵机属性
typedef enum
{   //舵机0：270度闭环mode
    steer0max = 2500,//逆时针MAX
    steer0mid = 1500,//停转
    steer0min = 500,//顺时针MAX
    steer0time = 300,
    //舵机1：270度闭环mode
    steer1max = 2500,
    steer1mid = 1750,
    steer1min = 1500,
    steer1time = 400
} SteerParam;
//小车属性
typedef enum
{
    EncoderMultiples = 4, //编码器倍数
    Accuracy = 13, //编码器
    gearratio = 30, //电机减速比
    tyre_diameter = 75//轮胎直径，单位：mm
} CarParam;
//按键结构体
typedef struct 
{
    uint8_t Key_Val;
    uint8_t Key_Down;
    uint8_t Key_Up;
    uint8_t Key_Old;
} Key;
//串口组结构体
typedef struct
{
    uint8_t len;
	uint16_t buff;
    uint8_t data;
    uint8_t *Receive_data;
    uint8_t *prcData;
} Uart;
typedef struct
{
    Uart imu;
    Uart camera;
    Uart blue;
    Uart steerimu;
    Uart DL20;
    Uart scan;
    Uart FOC;
    Uart STP;
} MyUartGroup;
//编码器结构体
typedef struct
{
    int A;      
    int B; 
    int C; 
    int D;
    float Encoder_precision;//电机(车轮)转1圈对应的编码器数值
    float Wheel_perimeter;//主动轮周长，单位：mm
    float frequency;//编码器捕获频率
} Encoder;
//电机底层速度控制相关参数结构体
typedef struct  
{
	float Velocity;     //编码器数值，读取电机实时速度
	float Pwm;          //电机PWM数值，控制电机实时速度
	float Target;      //电机目标速度值，控制电机目标速度
    float bais;       //电机速度偏差
    float Last_bais; //电机速度偏差上次值
	float Velocity_KP; //速度控制PID参数
	float Velocity_KI; //速度控制PID参数
    bool en;
} Motor;
//舵机相关结构体
typedef struct
{
    int ID; //舵机ID
    int Pwm; //舵机PWM数值500-2500
    int time; //舵机时间ms   
} Steer;
//小车应用层速度相关结构体
typedef struct
{
    float x; //小车x轴速度
    float y; //小车y轴速度
    float z; //小车z轴速度
} CarSpeedData;
//PID相关结构体
typedef struct
{
    float get;
    float target;
    float kp;
    float kd;
    float ki;
    float err;
    float err_last;
    float err_plus;
    float err_limit;
    float plus_limit;
    float out;
    bool en;
} Pid;
//手柄相关结构体
typedef struct
{
    bool ch1;
    bool ch2;
    bool ch3;
    bool ch4;
    bool ch5;
    bool ch6;
    bool ch7;
    bool ch8;
    uint8_t ch9;
    uint8_t ch10;
    uint8_t ch11;
    bool en;
} PS2CH;
//欧拉角相关结构体
typedef struct
{
    float Yaw;
    float Pitch;
    float Roll;
    float Gyrox;
    float Gyroy;
    float Gyroz;
} Euler;

void MyUartGroup_Init(MyUartGroup *group);
void MyEncoder_Init(Encoder *encoder);
void MyMotor_Init(Motor *motor);
void MySteer_Init();
void MyPid_Init();
#endif // __MYSYS_H