#ifndef __GREY_H
#define __GREY_H
#include "Mysys.h"
#define GreySCL_UP HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET)
#define GreySCL_DOWN HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET)
#define GreySDA HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)
void GreyPrc();
#endif