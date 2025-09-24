#include "BlueTooth.h"

extern Motor MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;//电机速度控制相关参数
extern MyUartGroup uartGroup;//定义串口组
extern CarSpeedData BlueVel,PS2Vel;//小车速度相关结构体
extern Pid angle,cloudyaw,cloudpitch,cloudcamerax,cloudcameray,grey;//PID相关结构体
extern PS2CH PS2;//手柄相关结构体
extern Euler CarAngle,SteerAngle;//欧拉角相关结构体
void BluePrc()
{
    static float speed = 100.0f;
    switch(uartGroup.blue.data)
    {
        case 'A':
            // 执行↑对应的操作
            BlueVel.x = speed;
            BlueVel.y = 0.0f;
            break;
        case 'B':
            // 执行↗对应的操作
            if(CARMODE!=1)break;
            BlueVel.x = speed;
            BlueVel.y = speed;
            break;
        case 'C':
            // 执行→对应的操作
            if(CARMODE==1)
            { 
                BlueVel.x = 0.0f;
                BlueVel.y = speed;
                break;
            }
            if(CARMODE==2)
            {
                MOTOR_A.en = 0;
                BlueVel.z = 150;
                break;
            }
        case 'D':
            if(CARMODE!=1)break;
            // 执行↘对应的操作
            BlueVel.x = -speed;
            BlueVel.y = speed;
            break;
        case 'E':
            // 执行↓对应的操作
            BlueVel.x = -speed;
            BlueVel.y = 0.0f;
            break;
        case 'F':
            if(CARMODE!=1)break;
            // 执行↙对应的操作
            BlueVel.x = -speed;
            BlueVel.y = -speed;
            break;
        case 'G':
            // 执行←对应的操作
            if(CARMODE==1)
            {
            BlueVel.x = 0.0f;
            BlueVel.y = -speed;
            break;
            }
            if(CARMODE==2)
             {
                MOTOR_A.en = 0;
                BlueVel.z = -150;
                break;
             }
        case 'H':
            if(CARMODE!=1)break;
            // 执行↖对应的操作
            BlueVel.x = speed;
            BlueVel.y = -speed;
            break;
        case 'X':
            speed += 5.0f;
            break;
        case 'Y':
            speed -= 5.0f;
            break;
        case 'Z':
            // 执行松开按键对应的操作
            //清除XY轴速度
            BlueVel.x = 0.0f;
            BlueVel.y = 0.0f;
            //重新使能闭环(串级闭环旋转很抖动，所以用开环旋转,转完再闭)
            MOTOR_A.en = 1;
            break;
        
        default:
            // 其他数据
            break;
    }  
}
void PS2Prc()
{
    if(UART4MODE!=1)return;
    PS2DataPrc(); 
    MOTOR_A.en=PS2.ch8;//开闭环模式选择
    angle.en = PS2.ch7;//角度环开启
    grey.en  = PS2.ch6;//循迹开启
    if(MOTOR_A.en)//闭环模式
    {
        if(PS2.ch1)
            angle.target+=3.0f;
        else if(PS2.ch3)
            angle.target-=3.0f;  

    }
    else//开环模式转向
    {
        if(PS2.ch1)
            PS2Vel.z=-100.0f;
        else if(PS2.ch3)
            PS2Vel.z=100.0f;  
        else
            PS2Vel.z=0.0f;
        
    }
    if(PS2.ch2)
        {
        Led(1,1);
        Led(2,1);
        Led(3,1);
        Buffer(1);
        }
    else
        {
        Led(1,0);
        Led(2,0);
        Led(3,0);
        Buffer(0);   
        }
    switch (PS2.ch11)
    {
    case 1:
        PS2Vel.x=200.0f;
        break;
    case 2:
        PS2Vel.x=100.0f;
        break;
    case 0:
        PS2Vel.x=0.0f;
        break;
    case 4:
        PS2Vel.x=-100.0f;
        break;
    case 5:
        PS2Vel.x=-200.0f;
        break;
    }
    switch (PS2.ch10)
    {
    case 1:
        PS2Vel.y=200.0f;
        break;
    case 2:
        PS2Vel.y=100.0f;
        break;
    case 0:
        PS2Vel.y=0.0f;
        break;
    case 4:
        PS2Vel.y=-100.0f;
        break;
    case 5:
        PS2Vel.y=-200.0f;
        break;
    }
    

}
void PS2DataPrc()
{
    if(PS2.en){
    //处理简单布尔值和线性编码器值
    PS2.ch1 = uartGroup.DL20.prcData[2];
    PS2.ch2 = uartGroup.DL20.prcData[3];
    PS2.ch3 = uartGroup.DL20.prcData[4]; 
    PS2.ch4 = uartGroup.DL20.prcData[5];  
    PS2.ch5 = uartGroup.DL20.prcData[6];
    PS2.ch6 = uartGroup.DL20.prcData[7];
    PS2.ch7 = uartGroup.DL20.prcData[8];
    PS2.ch8 = uartGroup.DL20.prcData[9];
    PS2.ch9 = uartGroup.DL20.prcData[10];
    //通道值判断
    if(uartGroup.DL20.prcData[11]<3|uartGroup.DL20.prcData[11]>200)
        PS2.ch10 = 1;
    if(uartGroup.DL20.prcData[11]>3&uartGroup.DL20.prcData[11]<40)
        PS2.ch10 = 2;
    if(uartGroup.DL20.prcData[11]>40&uartGroup.DL20.prcData[11]<60)
        PS2.ch10 = 0;
    if(uartGroup.DL20.prcData[11]>60&uartGroup.DL20.prcData[11]<95)
        PS2.ch10 = 4;
    if(uartGroup.DL20.prcData[11]>95&uartGroup.DL20.prcData[11]<105)
        PS2.ch10 = 5;
    if(uartGroup.DL20.prcData[12]<3|uartGroup.DL20.prcData[12]>200)
        PS2.ch11 = 1;
    if(uartGroup.DL20.prcData[12]>3&uartGroup.DL20.prcData[12]<40)
        PS2.ch11 = 2;
    if(uartGroup.DL20.prcData[12]>40&uartGroup.DL20.prcData[12]<60)
        PS2.ch11 = 0;
    if(uartGroup.DL20.prcData[12]>60&uartGroup.DL20.prcData[12]<95)
        PS2.ch11 = 4;
    if(uartGroup.DL20.prcData[12]>95&&uartGroup.DL20.prcData[12]<105)
        PS2.ch11 = 5;
    }
    else PS2.en+=uartGroup.DL20.prcData[5];
}