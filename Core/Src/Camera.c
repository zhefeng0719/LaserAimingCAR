#include "Camera.h"

extern MyUartGroup uartGroup;
extern osMessageQueueId_t Queue_CameraHandle;
coordinate aim, laser;

// 滑动窗口大小
#define FILTER_WIN 10
// 滑动窗口缓存
static uint16_t aim_x_buf[FILTER_WIN] = {0};
static uint16_t aim_y_buf[FILTER_WIN] = {0};
static uint16_t laser_x_buf[FILTER_WIN] = {0};
static uint16_t laser_y_buf[FILTER_WIN] = {0};
static int filter_idx = 0;

uint16_t moving_average(uint16_t *buf, uint16_t new_val) {
    buf[filter_idx] = new_val;
    uint32_t sum = 0;
    for (int i = 0; i < FILTER_WIN; i++) {
        sum += buf[i];
    }
    return (uint16_t)(sum / FILTER_WIN);
}

void CameraPrc()
{
    // 原始数据
    uint16_t raw_aim_x   = (uartGroup.camera.prcData[2] << 8 ) | uartGroup.camera.prcData[3];
    uint16_t raw_aim_y   = (uartGroup.camera.prcData[4] << 8 ) | uartGroup.camera.prcData[5];
    uint16_t raw_laser_x = (uartGroup.camera.prcData[6] << 8 ) | uartGroup.camera.prcData[7];
    uint16_t raw_laser_y = (uartGroup.camera.prcData[8] << 8 ) | uartGroup.camera.prcData[9];

    // 滑动滤波
    aim.x   = moving_average(aim_x_buf, raw_aim_x);
    aim.y   = moving_average(aim_y_buf, raw_aim_y);
    laser.x = moving_average(laser_x_buf, raw_laser_x);
    laser.y = moving_average(laser_y_buf, raw_laser_y);

    // 更新滑动窗口索引
    filter_idx++;
    if (filter_idx >= FILTER_WIN) filter_idx = 0;
}