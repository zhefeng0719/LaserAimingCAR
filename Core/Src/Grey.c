#include "Grey.h"
uint8_t GreyData[8];

void GreyPrc()//感为灰度数据处理
{
    unsigned char ret = 0;//储存数据
    unsigned char sensor_data;
     for (register uint8_t grey = 0; grey < 8; ++grey)
    {
        GreySCL_DOWN;	
        for(register int j=0; j<100; j++); // 延时，确保SCL下降沿稳定
        ret |= ((unsigned char)GreySDA << grey);
        GreySCL_UP; 
    }
    sensor_data = ret;
	GreyData[0] = ((sensor_data >> 0) & 0x1);
    GreyData[1] = ((sensor_data >> 1) & 0x1);
    GreyData[2] = ((sensor_data >> 2) & 0x1);
    GreyData[3] = ((sensor_data >> 3) & 0x1);
    GreyData[4] = ((sensor_data >> 4) & 0x1);
    GreyData[5] = ((sensor_data >> 5) & 0x1);
    GreyData[6] = ((sensor_data >> 6) & 0x1);
    GreyData[7] = ((sensor_data >> 7) & 0x1);
}