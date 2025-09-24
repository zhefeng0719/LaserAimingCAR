#include "Scan.h"
#include <string.h>
extern MyUartGroup uartGroup;//定义串口组
Distance Scan;
DisStp STP;

#define STP_FILTER_WIN 5
static float stp_filter_buf[STP_FILTER_WIN] = {0};
static uint8_t stp_filter_idx = 0;
static uint8_t stp_filter_cnt = 0;

void StpPrc()
{
    uint8_t *data = uartGroup.STP.prcData;
    uint32_t sum = 0;
    for(int i = 0; i < 12; i++)
    {
        uint16_t dist = data[10 + i*15] | (data[10 + i*15 + 1] << 8); // 低位在前
        STP.StpScan[i] = dist;
        sum += dist;
    }
    // 求平均值
    float avg = sum / 12.0f;
    STP.Value = avg;

    // 滑动滤波
    stp_filter_buf[stp_filter_idx] = avg;
    stp_filter_idx = (stp_filter_idx + 1) % STP_FILTER_WIN;
    if(stp_filter_cnt < STP_FILTER_WIN) stp_filter_cnt++;

    float filter_sum = 0;
    for(uint8_t i = 0; i < stp_filter_cnt; i++)
        filter_sum += stp_filter_buf[i];

    STP.Value = filter_sum / stp_filter_cnt;
}
void ScanPrc()
{
    uint8_t *data = uartGroup.scan.prcData;
    float start_deg = (uint16_t)((data[5] << 8) | data[6]) / 100.0f;
    float stop_deg  = (uint16_t)((data[55] << 8) | data[56]) / 100.0f;
    float area_angle = start_deg + (stop_deg - start_deg) / 2.0f;
    if(stop_deg < start_deg) stop_deg += 360.0f;
    if(area_angle >= 360.0f) area_angle -= 360.0f;

    if(area_angle > 345.0f || area_angle < 15.0f)
        Scan.x1 = ScanDataPrc(data); // 平均距离，单位mm
    if(area_angle > 15.0f && area_angle < 45.0f)
        Scan.x2 = ScanDataPrc(data); // 平均距离，单位mm

}
float ScanDataPrc(uint8_t *data)
{
    uint32_t distance_sum = 0;
    uint8_t valid_cnt = 0;
    for(register int i = 0; i < 16; i++)
    {
        uint16_t dist = (data[7 + i*3] << 8) | data[8 + i*3];
        if(dist > mindata && dist < maxdata)
        {
            distance_sum += dist;
            valid_cnt++;
        }
    }
    return (valid_cnt > 0) ? (distance_sum / (float)valid_cnt) : 0;
}