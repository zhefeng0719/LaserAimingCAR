#include "Motor.h"

extern Motor MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;//电机速度控制相关参数
extern CarSpeedData BlueVel,AngleVel,PS2Vel,GreyVel;//小车速度相关结构体
extern Pid angle,cloudyaw,cloudpitch,cloudcamerax,cloudcameray;//PID相关结构体
extern PS2CH PS2;//手柄相关结构体
extern Euler CarAngle,SteerAngle;//欧拉角相关结构体
int oil;
extern int smoothflag;
//增量式速度环
void Incremental_PI_A (Motor *motor)
{
    motor->bais=motor->Target-motor->Velocity;
    motor->Pwm+=motor->Velocity_KP*(motor->bais-motor->Last_bais)+motor->Velocity_KI*motor->bais;
    motor->Last_bais=motor->bais;
    LIMIT(motor->Pwm, -16700, 16700);
    if(motor->Target==0)motor->Pwm=0;
}
void Incremental_PI_B (Motor *motor)
{
    motor->bais=motor->Target-motor->Velocity;
    motor->Pwm+=motor->Velocity_KP*(motor->bais-motor->Last_bais)+motor->Velocity_KI*motor->bais;
    motor->Last_bais=motor->bais;
    LIMIT(motor->Pwm, -16700, 16700);
    if(motor->Target==0)motor->Pwm=0;
}
void Incremental_PI_C (Motor *motor)
{
    motor->bais=motor->Target-motor->Velocity;
    motor->Pwm+=motor->Velocity_KP*(motor->bais-motor->Last_bais)+motor->Velocity_KI*motor->bais;
    motor->Last_bais=motor->bais;
    LIMIT(motor->Pwm, -16700, 16700);
    if(motor->Target==0)motor->Pwm=0;
}
void Incremental_PI_D (Motor *motor)
{
    motor->bais=motor->Target-motor->Velocity;
    motor->Pwm+=motor->Velocity_KP*(motor->bais-motor->Last_bais)+motor->Velocity_KI*motor->bais;
    motor->Last_bais=motor->bais;
    LIMIT(motor->Pwm, -16700, 16700);
    if(motor->Target==0)motor->Pwm=0;
}
void TF()//坐标树:若要以自身为基准坐标，注释掉出现在坐标树的对应内容即可
{
    // float theta = (Yaw - SteerYaw) * PI / 180.0f;
    //  BlueVel.x = BlueVel.x * cosf(theta) - BlueVel.y * sinf(theta);
    //  BlueVel.y = BlueVel.x * sinf(theta) + BlueVel.y * cosf(theta);
}
void MotorCtrl()
{
    // 坐标变换，将小车自身坐标系映射到世界坐标系(麦轮用)
    if(CARMODE == 1)TF(); 
    //整理控制器   
	if(smoothflag==0){
    MOTOR_A.Target = PS2Vel.x-PS2Vel.y+PS2Vel.z+BlueVel.x+BlueVel.y+AngleVel.z+oil;
    MOTOR_B.Target = PS2Vel.x+PS2Vel.y+PS2Vel.z+BlueVel.x-BlueVel.y+AngleVel.z+oil;
    MOTOR_C.Target = PS2Vel.x+PS2Vel.y-PS2Vel.z+BlueVel.x-BlueVel.y-AngleVel.z+oil;
    MOTOR_D.Target = PS2Vel.x-PS2Vel.y-PS2Vel.z+BlueVel.x+BlueVel.y-AngleVel.z+oil;
	}
	if(smoothflag==1){
	MOTOR_A.Target = PS2Vel.x-PS2Vel.y+PS2Vel.z+BlueVel.x+BlueVel.y+AngleVel.z+Smooth(oil);
    MOTOR_B.Target = PS2Vel.x+PS2Vel.y+PS2Vel.z+BlueVel.x-BlueVel.y+AngleVel.z+Smooth(oil);
    MOTOR_C.Target = PS2Vel.x+PS2Vel.y-PS2Vel.z+BlueVel.x-BlueVel.y-AngleVel.z+Smooth(oil);
    MOTOR_D.Target = PS2Vel.x-PS2Vel.y-PS2Vel.z+BlueVel.x+BlueVel.y-AngleVel.z+Smooth(oil);
	}
    if(MOTOR_A.en)//闭环控制
    {    
        //速度环计算
        Incremental_PI_A(&MOTOR_A);
        Incremental_PI_B(&MOTOR_B);
        Incremental_PI_C(&MOTOR_C);
        Incremental_PI_D(&MOTOR_D);
    }
    else//开环控制 
    {
        const static float a=25;//增益系数
        MOTOR_A.Pwm = PS2Vel.x-PS2Vel.y+PS2Vel.z+BlueVel.x+BlueVel.y+BlueVel.z+GreyVel.z;
        MOTOR_B.Pwm = PS2Vel.x+PS2Vel.y+PS2Vel.z+BlueVel.x-BlueVel.y+BlueVel.z+GreyVel.z;
        MOTOR_C.Pwm = PS2Vel.x+PS2Vel.y-PS2Vel.z+BlueVel.x-BlueVel.y-BlueVel.z-GreyVel.z;
        MOTOR_D.Pwm = PS2Vel.x-PS2Vel.y-PS2Vel.z+BlueVel.x+BlueVel.y-BlueVel.z-GreyVel.z;

        MOTOR_A.Pwm*=a;MOTOR_B.Pwm*=a;MOTOR_C.Pwm*=a;MOTOR_D.Pwm*=a;
        angle.target = CarAngle.Yaw;//开环旋转时目标角度随变
    }
    //电机控制
    MOTOR_DIR_CTRL_AND_PWM(MOTOR_A.Pwm, GPIOE, GPIO_PIN_12, GPIOE, GPIO_PIN_15, TIM1, CCR4);
    MOTOR_DIR_CTRL_AND_PWM(MOTOR_B.Pwm, GPIOE, GPIO_PIN_7 , GPIOE, GPIO_PIN_8 , TIM1, CCR3);
    MOTOR_DIR_CTRL_AND_PWM(MOTOR_C.Pwm, GPIOB, GPIO_PIN_0 , GPIOB, GPIO_PIN_1 , TIM1, CCR2);
    MOTOR_DIR_CTRL_AND_PWM(MOTOR_D.Pwm, GPIOC, GPIO_PIN_5 , GPIOC, GPIO_PIN_4 , TIM1, CCR1);
}

float Smooth(float target)//平滑闭环，避免电机释放时的微动
{
    static float value=500.0;
    if(target>value)value+=3.5f;
    if(target<value)value-=3.5f;
    return value;
}