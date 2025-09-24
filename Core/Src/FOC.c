#include "FOC.h"
extern MyUartGroup uartGroup;
extern coordinate aim;
int L1scan;
int Lost;
void FocPrc()
{
    aim.x = (uartGroup.FOC.prcData[2] << 8) | uartGroup.FOC.prcData[3];
    aim.y = (uartGroup.FOC.prcData[4] << 8) | uartGroup.FOC.prcData[5];
    L1scan = (uartGroup.FOC.prcData[6] << 8) | uartGroup.FOC.prcData[7]; 
	Lost = uartGroup.FOC.prcData[8];
}