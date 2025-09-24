#include "Encoder.h"

extern Encoder OriginalEncoder;//原始编码器
extern Motor MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;//电机速度控制相关参数
int Read_Encoder(uint8_t TIMX)//编码器原始数据
{
 int Encoder_TIM;    
 switch(TIMX)
 {
	case 2:  Encoder_TIM= (int16_t)TIM2 -> CNT;   TIM2 -> CNT=0;  break;
	case 3:  Encoder_TIM= (int16_t)TIM3 -> CNT;   TIM3 -> CNT=0;  break;
	case 4:  Encoder_TIM= (int16_t)TIM4 -> CNT;   TIM4 -> CNT=0;  break;	
	case 5:  Encoder_TIM= (int16_t)TIM5 -> CNT;   TIM5 -> CNT=0;  break;	
	default: Encoder_TIM=0;
 }
	return Encoder_TIM;
}
void EncoderPrc()//编码器处理
{
    //读取原始编码器数据并确定编码器数值极性
    OriginalEncoder.A = -Read_Encoder(2);	
    OriginalEncoder.B = Read_Encoder(4);	
    OriginalEncoder.C = Read_Encoder(3);	
    OriginalEncoder.D = Read_Encoder(5);
    //编码器原始数据转换为车轮速度，单位cm/s
    float velocity_a = OriginalEncoder.A * OriginalEncoder.frequency * OriginalEncoder.Wheel_perimeter / 10.0f / OriginalEncoder.Encoder_precision;
    float velocity_b = OriginalEncoder.B * OriginalEncoder.frequency * OriginalEncoder.Wheel_perimeter / 10.0f / OriginalEncoder.Encoder_precision;
    float velocity_c = OriginalEncoder.C * OriginalEncoder.frequency * OriginalEncoder.Wheel_perimeter / 10.0f / OriginalEncoder.Encoder_precision;
    float velocity_d = OriginalEncoder.D * OriginalEncoder.frequency * OriginalEncoder.Wheel_perimeter / 10.0f / OriginalEncoder.Encoder_precision;
    // 一阶低通滤波
    static float last_velocity_a = 0, last_velocity_b = 0, last_velocity_c = 0, last_velocity_d = 0;
    float alpha = 0.2f;
    MOTOR_A.Velocity = alpha * velocity_a + (1.0f - alpha) * last_velocity_a;
    MOTOR_B.Velocity = alpha * velocity_b + (1.0f - alpha) * last_velocity_b;
    MOTOR_C.Velocity = alpha * velocity_c + (1.0f - alpha) * last_velocity_c;
    MOTOR_D.Velocity = alpha * velocity_d + (1.0f - alpha) * last_velocity_d;
    last_velocity_a = MOTOR_A.Velocity;
    last_velocity_b = MOTOR_B.Velocity;
    last_velocity_c = MOTOR_C.Velocity;
    last_velocity_d = MOTOR_D.Velocity;
}
