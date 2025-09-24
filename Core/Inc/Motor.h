#include "MySys.h"
#define MOTOR_DIR_CTRL_AND_PWM(CCR, PIN_A_PORT, PIN_A, PIN_B_PORT, PIN_B, TIMx, TIM_CCR) \
    do { \
        if ((CCR) > 0) { \
            HAL_GPIO_WritePin((PIN_A_PORT), (PIN_A), GPIO_PIN_SET); \
            HAL_GPIO_WritePin((PIN_B_PORT), (PIN_B), GPIO_PIN_RESET); \
            (TIMx)->TIM_CCR = (CCR); \
        } else { \
            HAL_GPIO_WritePin((PIN_A_PORT), (PIN_A), GPIO_PIN_RESET); \
            HAL_GPIO_WritePin((PIN_B_PORT), (PIN_B), GPIO_PIN_SET); \
            (TIMx)->TIM_CCR = -(CCR); \
        } \
    } while(0)
void TF();
void MotorCtrl();
float Smooth(float target);

