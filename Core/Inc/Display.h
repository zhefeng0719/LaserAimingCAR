#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "MySys.h"
typedef enum
{
    X1 = 0,
    X2 = 12,
    X3 = 24,
    X4 = 36,
    X5 = 48,
    X6 = 60,
    X7 = 72,
    X8 = 84,
    X9 = 96,
    X10 = 108,
    X11 = 120,
    Y1 = 0,
    Y2 = 12,
    Y3 = 24,
    Y4 = 36,
    Y5 = 48,
}oledlocation;
void Led(int num,int state);
void Buffer(int state);
void OledPrc();
void LedPrc();
#endif // __DISPLAY_H