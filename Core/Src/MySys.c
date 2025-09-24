#include <Mysys.h>
Key Key1,Key2,Key3;//按键结构体
MyUartGroup uartGroup;//定义串口组
Encoder OriginalEncoder;//原始编码器
Motor MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;//电机速度控制相关参数
Steer steer0,steer1;//舵机结构体
CarSpeedData BlueVel,AngleVel,PS2Vel,GreyVel;//小车速度相关结构体
Pid angle,cloudyaw,cloudpitch,cloudcamerax,cloudcameray,grey;//PID相关结构体
PS2CH PS2;//手柄相关结构体
Euler CarAngle,SteerAngle;//欧拉角相关结构体
// 静态分配scan缓冲区,雷达数据流太大了
static uint8_t scan_rx_buf[255];
static uint8_t scan_prc_buf[58];
static uint8_t stp_rx_buf[512];   // DMA接收缓冲区，留余量防止粘包
static uint8_t stp_prc_buf[195];  // 用于存放一帧完整数据包（数据区+校验和）
//串口参数初始化
void MyUartGroup_Init(MyUartGroup *group)
{
    group->imu.len = 11;//有效长度
	group->imu.buff = 64;//缓冲区大小
    group->imu.Receive_data = (uint8_t *)malloc(group->imu.buff);
    group->imu.prcData = (uint8_t *)malloc(group->imu.len);

    switch (UART2MODE)
    {
    case 0:
    group->scan.len = 58;
    group->scan.buff = 255;
    group->scan.Receive_data = scan_rx_buf;
    group->scan.prcData = scan_prc_buf;
    break;
    case 1:
    group->STP.len = 195;      // 一帧完整包长度
    group->STP.buff = 512;     // 缓冲区大小
    group->STP.Receive_data = stp_rx_buf;
    group->STP.prcData = stp_prc_buf;
    break;
    default:
        break;
    }

    switch(UART5MODE)
    {
    case 0:
    group->camera.len = 10;
    group->camera.buff = 64;
    group->camera.Receive_data = (uint8_t *)malloc(group->camera.buff);
    group->camera.prcData = (uint8_t *)malloc(group->camera.len);
    break;
    case 1:
    group->FOC.len = 9;      
    group->FOC.buff = 64; 
    group->FOC.Receive_data = (uint8_t *)malloc(group->FOC.buff);
    group->FOC.prcData = (uint8_t *)malloc(group->FOC.len);
    break;
    }

    switch (UART4MODE)
    {
    case 0:
        group->steerimu.len = 11;  
        group->steerimu.buff = 64;
        group->steerimu.Receive_data = (uint8_t *)malloc(group->steerimu.buff);
        group->steerimu.prcData = (uint8_t *)malloc(group->steerimu.len);
        break;
    case 1:
        group->DL20.len = 13;
        group->DL20.buff = 64;
        group->DL20.Receive_data = (uint8_t *)malloc(group->DL20.buff);
        group->DL20.prcData = (uint8_t *)malloc(group->DL20.len);
        break;
    default:
        break;
    }

    group->blue.data = 0;
}
//编码器初始化
void MyEncoder_Init(Encoder *encoder)
{
    encoder->A = 0;
    encoder->B = 0;
    encoder->C = 0;
    encoder->D = 0;
    encoder->Encoder_precision = EncoderMultiples*Accuracy*gearratio; //编码器精度
    encoder->Wheel_perimeter = tyre_diameter * PI; //轮子周长mm
    encoder->frequency = 100.0f; //频率HZ
}
//电机初始化
void MyMotor_Init(Motor *motor)
{
    motor->Velocity = 0.0f;
    motor->Pwm = 0.0f;
    motor->Target = 0.0f;
    motor->bais = 0.0f;
    motor->Last_bais = 0.0f;
    motor->Velocity_KP = 70.0f; //速度控制PID参数
    motor->Velocity_KI = 15.0f; //速度控制PID参数
    MOTOR_A.en=1;//初始状态开启闭环
}
//总线舵机初始化
void MySteer_Init()
{
    steer0.ID = 0;
    steer0.Pwm = steer0mid;
    steer0.time = steer0time;

    steer1.ID = 1;
    steer1.Pwm = steer1mid;
    steer1.time = steer1time;
}
//PID初始化
void MyPid_Init()
{
    angle.kp = -2.0f;
    angle.kd = -0.0f;
    angle.ki = -0.04f;
    angle.en = 1;
	angle.plus_limit=10000;

    cloudyaw.kp = 3.5f;
    cloudyaw.kd = 2.5f;
    cloudyaw.ki = 1.2f;
    cloudyaw.en = 1;
    cloudyaw.plus_limit=500;

    cloudpitch.kp = 7.0f;
    cloudpitch.kd = 0.0f;
    cloudpitch.ki = 0.0f;
    cloudpitch.en = 0;
    cloudpitch.plus_limit=500;

    //第一套 -3.45 -0.4 -0.48 1米内优秀
    cloudcamerax.kp = -3.45f;
    cloudcamerax.kd = -0.4f;
    cloudcamerax.ki = -0.48f;
    cloudcamerax.en = 1;
    cloudcamerax.plus_limit=10000;
    cloudcamerax.target = 120.0f;

   //第一套 -2.4 -0.35 -0.34 //1米内优秀
    cloudcameray.kp = -2.4f;
    cloudcameray.kd = -0.35f;
    cloudcameray.ki = -0.34f;
    cloudcameray.en = 1;
    cloudcameray.plus_limit=10000;
    cloudcameray.target = 160.0f;

    grey.kp=-100.0f;
    grey.kd=0.0f;
    grey.ki=0.0f;
    grey.en=0;
    grey.err_plus=100;
}