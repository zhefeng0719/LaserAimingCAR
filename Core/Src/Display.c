#include "Display.h"
extern uint8_t GreyData[8];
extern Key Key1,Key2,Key3;//按键结构体
extern Motor MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;//电机速度控制相关参数
extern MyUartGroup uartGroup;//定义串口组
extern coordinate aim,laser;//摄像头结构体
extern Euler CarAngle,SteerAngle;//欧拉角结构体
extern PS2CH PS2;//手柄结构体
extern Distance Scan;//雷达数据结构体
extern DisStp STP;
extern int L1scan;//L1激光测距仪
extern int Lost;
void Led(int num,int state)
{
    switch(num)
    {
        case 1:
            if(state)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
            else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
            break;
        case 2:
            if(state)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
            else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
            break;
        case 3:
            if(state)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
            else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}
void Buffer(int state)
{
    if(state)HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
}
void OledPrc()
{
    static float time;
    time+=0.01f;
    static int Page=6,PageOld=99;
    if(Page!=PageOld)//检测到页面切换，清屏
    {
        OLED_Clear();
        PageOld=Page;
    }
    if(Key1.Key_Up)Page++;//切换页面
    if(Page>6)Page=0;//页面限幅
    switch(Page)
    {
        case 0:
           OLED_ShowString(X1,Y2,"Vel:",OLED_6X8);
           OLED_ShowString(X1,Y4,"Tar:",OLED_6X8);
           OLED_ShowSignedNum(X4,Y2,MOTOR_A.Velocity,5,OLED_6X8);
           OLED_ShowSignedNum(X8,Y2,MOTOR_C.Velocity,5,OLED_6X8);
           OLED_ShowSignedNum(X4,Y3,MOTOR_B.Velocity,5,OLED_6X8);
           OLED_ShowSignedNum(X8,Y3,MOTOR_D.Velocity,5,OLED_6X8);
           OLED_ShowSignedNum(X4,Y4,MOTOR_A.Target,5,OLED_6X8);
           OLED_ShowSignedNum(X8,Y4,MOTOR_C.Target,5,OLED_6X8);
           OLED_ShowSignedNum(X4,Y5,MOTOR_B.Target,5,OLED_6X8);
           OLED_ShowSignedNum(X8,Y5,MOTOR_D.Target,5,OLED_6X8);           
            break;
        case 1:
            OLED_ShowString(X1,Y2,"Yaw",OLED_6X8);
            OLED_ShowString(X7,Y2,"Gz",OLED_6X8);
            OLED_ShowString(X1,Y3,"Grey:",OLED_6X8);
            OLED_ShowString(X1,Y4,"Aim:",OLED_6X8);
            OLED_ShowString(X4,Y4,"x",OLED_6X8);
            OLED_ShowString(X8,Y4,"y",OLED_6X8);
            OLED_ShowString(X1,Y5,"Laser:",OLED_6X8);
            OLED_ShowString(X4,Y4,"x",OLED_6X8);
            OLED_ShowString(X8,Y4,"y",OLED_6X8);
            OLED_ShowString(X4,Y5,"x",OLED_6X8);
            OLED_ShowString(X8,Y5,"y",OLED_6X8);
            OLED_ShowFloatNum(X3,Y2,CarAngle.Yaw,3,1,OLED_6X8);    
            OLED_ShowFloatNum(X8,Y2,CarAngle.Gyroz,3,1,OLED_6X8);        
            OLED_ShowNum(X4,Y3,GreyData[0],1,OLED_6X8);
            OLED_ShowNum(X5,Y3,GreyData[1],1,OLED_6X8);
            OLED_ShowNum(X6,Y3,GreyData[2],1,OLED_6X8);
            OLED_ShowNum(X7,Y3,GreyData[3],1,OLED_6X8);
            OLED_ShowNum(X8,Y3,GreyData[4],1,OLED_6X8);
            OLED_ShowNum(X9,Y3,GreyData[5],1,OLED_6X8);
            OLED_ShowNum(X10,Y3,GreyData[6],1,OLED_6X8);
            OLED_ShowNum(X11,Y3,GreyData[7],1,OLED_6X8);   
            OLED_ShowNum(X5,Y4,aim.x,3,OLED_6X8);
            OLED_ShowNum(X9,Y4,aim.y,3,OLED_6X8);
            OLED_ShowNum(X5,Y5,laser.x,3,OLED_6X8);
            OLED_ShowNum(X9,Y5,laser.y,3,OLED_6X8);                 
            break;
        case 2:
            OLED_ShowString(X1,Y2,"SteerYaw:",OLED_6X8);
            OLED_ShowString(X1,Y3,"SteerPitch:",OLED_6X8);
            OLED_ShowString(X1,Y4,"Key:",OLED_6X8);
            OLED_ShowString(X1,Y5,"Blue:",OLED_6X8);
            // OLED_ShowNum(X4,Y2,0,1,OLED_6X8);
            OLED_ShowFloatNum(X8,Y2,SteerAngle.Yaw,3,1,OLED_6X8);
            OLED_ShowNum(X4,Y3,0,1,OLED_6X8);
            OLED_ShowFloatNum(X8,Y3,SteerAngle.Pitch,3,1,OLED_6X8);
            OLED_ShowNum(X4,Y4,Key1.Key_Val,1,OLED_6X8);
            OLED_ShowNum(X6,Y4,Key2.Key_Val,1,OLED_6X8);
            OLED_ShowNum(X8,Y4,Key3.Key_Val,1,OLED_6X8);
            OLED_ShowChar(X4,Y5,(char)uartGroup.blue.data,OLED_6X8);

            break;
        case 3:
            OLED_ShowString(X1,Y2,"PS2:",OLED_6X8);
            OLED_ShowNum(X4,Y2,PS2.ch1,1,OLED_6X8);
            OLED_ShowNum(X6,Y2,PS2.ch2,1,OLED_6X8);
            OLED_ShowNum(X8,Y2,PS2.ch3,1,OLED_6X8);
            OLED_ShowNum(X10,Y2,PS2.ch4,1,OLED_6X8);
            OLED_ShowNum(X4,Y3,PS2.ch5,1,OLED_6X8);
            OLED_ShowNum(X6,Y3,PS2.ch6,1,OLED_6X8);
            OLED_ShowNum(X8,Y3,PS2.ch7,1,OLED_6X8);
            OLED_ShowNum(X10,Y3,PS2.ch8,1,OLED_6X8);
            OLED_ShowNum(X4,Y4,PS2.ch9,3,OLED_6X8);
            OLED_ShowNum(X8,Y4,PS2.ch10,3,OLED_6X8);
            OLED_ShowNum(X10,Y4,PS2.ch11,3,OLED_6X8);
            OLED_ShowNum(X1,Y5,uartGroup.DL20.prcData[11],3,OLED_6X8);
            OLED_ShowNum(X10,Y5,uartGroup.DL20.prcData[12],3,OLED_6X8);
            break;
        case 4:
            OLED_ShowString(X1,Y2,"SCAN:",OLED_6X8);
            OLED_ShowFloatNum(X2,Y3,Scan.x1,3,1,OLED_6X8);
            OLED_ShowFloatNum(X5,Y3,Scan.x2,3,1,OLED_6X8);
            OLED_ShowFloatNum(X8,Y3,Scan.x3,3,1,OLED_6X8);
            OLED_ShowFloatNum(X2,Y4,Scan.x4,3,1,OLED_6X8);
            OLED_ShowFloatNum(X5,Y4,Scan.x5,3,1,OLED_6X8);
            OLED_ShowFloatNum(X8,Y4,Scan.x6,3,1,OLED_6X8);
            OLED_ShowFloatNum(X2,Y5,Scan.x7,3,1,OLED_6X8);
            OLED_ShowFloatNum(X5,Y5,Scan.x8,3,1,OLED_6X8);
            OLED_ShowFloatNum(X8,Y5,Scan.x9,3,1,OLED_6X8);
            break;
        case 5 :
            OLED_ShowString(X1,Y2,"L1scan:",OLED_6X8);
            OLED_ShowNum(X5,Y2,L1scan,4,OLED_6X8);
            OLED_ShowString(X1,Y3,"STPscan:",OLED_6X8);  
            OLED_ShowNum(X5,Y3,STP.Value,4,OLED_6X8);
            break;
        case 6:
            OLED_ShowString(X1,Y2,"Yaw",OLED_6X8);
            OLED_ShowFloatNum(X3,Y2,CarAngle.Yaw,3,1,OLED_6X8); 
            OLED_ShowString(X1,Y3,"STPscan:",OLED_6X8);  
            OLED_ShowNum(X5,Y3,STP.Value,4,OLED_6X8);
            OLED_ShowString(X1,Y4,"AIM:",OLED_6X8);
            OLED_ShowSignedNum(X4,Y4,aim.x,5,OLED_6X8);
            OLED_ShowSignedNum(X8,Y4,aim.y,5,OLED_6X8);
			OLED_ShowString(X1,Y5,"Lost:",OLED_6X8);
            OLED_ShowSignedNum(X4,Y5,Lost,1,OLED_6X8);
			OLED_ShowString(X6,Y5,"L1:",OLED_6X8);
			OLED_ShowSignedNum(X8,Y5,L1scan,4,OLED_6X8);
            break;
        default:
            Page=0;
            break;
    }
    OLED_ShowString(X1,Y1,"Page:",OLED_6X8);
    OLED_ShowNum(X4,Y1,Page,1,OLED_6X8);
    OLED_ShowString(X5,Y1,"time:",OLED_6X8);
    OLED_ShowFloatNum(X8,Y1,time,3,2,OLED_6X8);
    OLED_Update();
}
void LedPrc()
{
    
}