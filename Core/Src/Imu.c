#include "Imu.h"
#include "usart.h"

extern MyUartGroup uartGroup;
extern Euler CarAngle,SteerAngle;
void ImuPrc()//IMU数据获取
{      
    if(uartGroup.imu.prcData[1] == 0x52)
        CarAngle.Gyroz = ((float)((int16_t)((uartGroup.imu.prcData[7] << 8) | uartGroup.imu.prcData[6])) / 32768) * 2000;
        // CarAngle.Gyrox = ((float)((int16_t)((uartGroup.imu.prcData[5] << 8) | uartGroup.imu.prcData[4])) / 32768) * 180;
        // CarAngle.Gyroy = ((float)((int16_t)((uartGroup.imu.prcData[3] << 8) | uartGroup.imu.prcData[2])) / 32768) * 180;
    if(uartGroup.imu.prcData[1] == 0x53)
        CarAngle.Yaw = ((float)((int16_t)((uartGroup.imu.prcData[7] << 8) | uartGroup.imu.prcData[6])) / 32768) * 180;
        // CarAngle.Pitch = ((float)((int16_t)((uartGroup.imu.prcData[5] << 8) | uartGroup.imu.prcData[4])) / 32768) * 180;
        // CarAngle.Roll = ((float)((int16_t)((uartGroup.imu.prcData[3] << 8) | uartGroup.imu.prcData[2])) / 32768) * 180;
    
    if(UART4MODE==0){       
        if(uartGroup.steerimu.prcData[0] == 0x55 && uartGroup.steerimu.prcData[1] == 0x53)
            SteerAngle.Yaw = ((float)((int16_t)((uartGroup.steerimu.prcData[7] << 8) | uartGroup.steerimu.prcData[6])) / 32768) * 180;
            SteerAngle.Pitch = ((float)((int16_t)((uartGroup.steerimu.prcData[5] << 8) | uartGroup.steerimu.prcData[4])) / 32768) * 180;
            SteerAngle.Roll = ((float)((int16_t)((uartGroup.steerimu.prcData[3] << 8) | uartGroup.steerimu.prcData[2])) / 32768) * 180;
    }
}