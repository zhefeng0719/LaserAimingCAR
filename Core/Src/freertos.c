/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MySys.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define HITMODE 1 //1��ģʽ 0С��ģʽ
extern Key Key1,Key2,Key3,Key_GW;//按键结构�????????????????
extern Motor MOTOR_A,MOTOR_B,MOTOR_C,MOTOR_D;//电机速度控制相关变量
extern MyUartGroup uartGroup;//UART组结构体
extern Encoder OriginalEncoder;//编码器结构体
extern DisStp STP;
extern coordinate aim;
extern int L1scan;
extern int oil;
extern int Lost;
int smoothflag=0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task1 */
osThreadId_t Task1Handle;
const osThreadAttr_t Task1_attributes = {
  .name = "Task1",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for Task2 */
osThreadId_t Task2Handle;
const osThreadAttr_t Task2_attributes = {
  .name = "Task2",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for Task3 */
osThreadId_t Task3Handle;
const osThreadAttr_t Task3_attributes = {
  .name = "Task3",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task0 */
osThreadId_t Task0Handle;
const osThreadAttr_t Task0_attributes = {
  .name = "Task0",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task4 */
osThreadId_t Task4Handle;
const osThreadAttr_t Task4_attributes = {
  .name = "Task4",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task5 */
osThreadId_t Task5Handle;
const osThreadAttr_t Task5_attributes = {
  .name = "Task5",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Task6 */
osThreadId_t Task6Handle;
const osThreadAttr_t Task6_attributes = {
  .name = "Task6",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void InitTask(void *argument);
void MsgTaskHight(void *argument);
void MsgTaskLow(void *argument);
void DispTask(void *argument);
void CtrlTask(void *argument);
void ScanTask(void *argument);
void TestTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Task1 */
  Task1Handle = osThreadNew(InitTask, NULL, &Task1_attributes);

  /* creation of Task2 */
  Task2Handle = osThreadNew(MsgTaskHight, NULL, &Task2_attributes);

  /* creation of Task3 */
  Task3Handle = osThreadNew(MsgTaskLow, NULL, &Task3_attributes);

  /* creation of Task0 */
  Task0Handle = osThreadNew(DispTask, NULL, &Task0_attributes);

  /* creation of Task4 */
  Task4Handle = osThreadNew(CtrlTask, NULL, &Task4_attributes);

  /* creation of Task5 */
  Task5Handle = osThreadNew(ScanTask, NULL, &Task5_attributes);

  /* creation of Task6 */
  Task6Handle = osThreadNew(TestTask, NULL, &Task6_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_InitTask */
/**
* @brief Function implementing the Task0 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_InitTask */
void InitTask(void *argument)
{
  /* USER CODE BEGIN InitTask */
//----------------初始化任务，执行后删�???????----------------
  // 启动PWM输出
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  // 启动霍尔编码�???
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_2);
  // OLED初始�???
  OLED_Init();
  // 串口组初始化
  MyUartGroup_Init(&uartGroup);
  // 编码器结构体初始�???
  MyEncoder_Init(&OriginalEncoder);
  // 电机参数初始�???
  MyMotor_Init(&MOTOR_A);
  MyMotor_Init(&MOTOR_B);
  MyMotor_Init(&MOTOR_C);
  MyMotor_Init(&MOTOR_D);
  // 舵机参数初始�???
  // MySteer_Init();
  //PID参数初始�???
  MyPid_Init();
  // 使能串口中断
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
  HAL_NVIC_EnableIRQ(UART5_IRQn);
  // 启动串口3中断接收
  HAL_UART_Receive_IT(&huart3, &uartGroup.blue.data, 1);
  // 启动DMA接收
  HAL_UART_Receive_DMA(&huart1, uartGroup.imu.Receive_data, uartGroup.imu.len);
  if(UART2MODE==0)
  HAL_UART_Receive_DMA(&huart2, uartGroup.scan.Receive_data, uartGroup.scan.len);
  else if(UART2MODE==1)
  HAL_UART_Receive_DMA(&huart2, uartGroup.STP.Receive_data, uartGroup.STP.len);
  if(UART5MODE==0)
  HAL_UART_Receive_DMA(&huart5, uartGroup.camera.Receive_data, uartGroup.camera.len);
  else if(UART5MODE==1)
  HAL_UART_Receive_DMA(&huart5, uartGroup.FOC.Receive_data, uartGroup.FOC.len);
  if(UART4MODE==1)
    HAL_UART_Receive_DMA(&huart4, uartGroup.DL20.Receive_data, uartGroup.DL20.buff);
  else if(UART4MODE==0)
    HAL_UART_Receive_DMA(&huart4, uartGroup.steerimu.Receive_data, uartGroup.steerimu.len);
  // 使能串口1和串�???5的空闲中�???
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
  __HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  // �???有初始化完成，删除本任务
  Buffer(0);
  Led(2,0);
  for(register long slow=0;slow<21474836;slow++);//缓启动，确保电压稳定
  Buffer(0);
  Led(2,0);
  vTaskDelete(NULL);
  /* USER CODE END InitTask */
}

/* USER CODE BEGIN Header_MsgTaskHight */
/**
* @brief Function implementing the Task2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MsgTaskHight */
void MsgTaskHight(void *argument)
{
  /* USER CODE BEGIN MsgTaskHight */
  /* Infinite loop */
  for(;;)
  {
    ImuPrc();
    FocPrc();
    // CameraPrc(); 
    osDelay(1);
  }
  /* USER CODE END MsgTaskHight */
}

/* USER CODE BEGIN Header_MsgTaskLow */
/**
* @brief Function implementing the Task4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MsgTaskLow */
void MsgTaskLow(void *argument)
{
  /* USER CODE BEGIN MsgTaskLow */
  /* Infinite loop */
  for(;;)
  {
    KeyPrc();
    BluePrc();
    // GreyPrc();
    EncoderPrc();
    // PS2Prc(); 
    osDelay(30);
  }
  /* USER CODE END MsgTaskLow */
}

/* USER CODE BEGIN Header_DispTask */
/**
* @brief Function implementing the Task3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DispTask */
void DispTask(void *argument)
{
  /* USER CODE BEGIN DispTask */
  /* Infinite loop */
  for(;;)
  {
    LedPrc();
    OledPrc();
    osDelay(10);
  }
  /* USER CODE END DispTask */
}

/* USER CODE BEGIN Header_CtrlTask */
/**
* @brief Function implementing the Task2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CtrlTask */
void CtrlTask(void *argument)
{
  /* USER CODE BEGIN CtrlTask */
  /* Infinite loop */
  for(;;)        
  {
    PidPrc();
    MotorCtrl();
    // SteerCtrl();
    osDelay(20);
  }
  /* USER CODE END CtrlTask */
}

/* USER CODE BEGIN Header_ScanTask */
/**
* @brief Function implementing the Task5 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ScanTask */
void ScanTask(void *argument)
{
  /* USER CODE BEGIN ScanTask */
  /* Infinite loop */
  for(;;)
  {
    // ScanPrc();
    StpPrc();
    osDelay(1);
  }
  /* USER CODE END ScanTask */
}

/* USER CODE BEGIN Header_TestTask */
/**
* @brief Function implementing the Task6 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TestTask */
void TestTask(void *argument)
{
  /* USER CODE BEGIN TestTask */
  const int TIMERR = 200; 
  const int effhittime = 450;
  static int hiterr = 60;
  static int time_ms=0;
  static int time_ms2=0;
  static int Task=1;
  /* Infinite loop */
  for(;;)
  {
    if(Task==1)
    {
      time_ms+=10;
      if(time_ms<300)
      {
        oil = 500;
      }
      else
      {
		smoothflag=1;
        oil = 0;
        time_ms=0;
        Task++;
      }
    }
    if(Task==2)
    {
      if(STP.Value<2000&&STP.Value>100)
        time_ms+=10;
      else if(STP.Value>2000)
        {
        oil=200;
        time_ms=0;
        }
      if(time_ms>TIMERR){
        Task++;
        time_ms=0;
      }
    }
    if(Task==3)
    {
      oil=0;
      time_ms+=10;
      if(time_ms>500){
        Task++;
        time_ms=0;
      }
    }
    if(Task==4)
    {
      oil = -15;
      time_ms+=10;
      if(time_ms>500){
        Task++;
        time_ms=0;
      }
    }
    if(Task==5)
    {
      oil = 15;
      time_ms+=10;
      if(time_ms>500){
        Task--;
        time_ms=0;
      }
    }
    if(Task==6)
    {
      oil=0;
      Buffer(1);
      Led(3,1);
    }
	//�ж���Ч���
	if( (aim.x<320+hiterr)&
		(aim.x>320-hiterr)&
		(L1scan!=0)&
		(Lost!=1))
	{
	time_ms2+=10;
	}
	else time_ms2=0;
	//�����Ч
	if(time_ms2>effhittime){
		Buffer(1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
	}
	else {
		Buffer(0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	}
	//���ݾ�����������Ч��Χ
	if(HITMODE==1){
	if(L1scan<3000)hiterr=50;
	if(L1scan<2000)hiterr=70;
	if(L1scan<1000)hiterr=130;
	if(L1scan<500)hiterr=240;
	}
	if(HITMODE==0){
	if(L1scan<3000)hiterr=30;
	if(L1scan<2000)hiterr=50;
	if(L1scan<1000)hiterr=100;
	if(L1scan<500)hiterr=230;
	}
    osDelay(10);
  }
  /* USER CODE END TestTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

