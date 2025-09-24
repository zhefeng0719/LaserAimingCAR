#ifndef __SCAN_H_
#define __SCAN_H_
#include "MySys.h"
typedef enum
{
//有效数据
maxdata = 12000,
mindata = 100,
}ScanParam;
typedef struct 
{
float x1;
float x2;
float x3;
float x4;
float x5;
float x6;
float x7;
float x8;
float x9;
float x10;
float x11;
float x12;
}Distance;
typedef struct
{
int Value;
int StpScan[12];

}DisStp;

void ScanPrc();
void StpPrc();
float ScanDataPrc(uint8_t *data);
#endif