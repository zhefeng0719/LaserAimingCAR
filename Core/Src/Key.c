#include "Key.h"

extern Key Key1,Key2,Key3;//按键结构体

void KeyPrc()//按键处理
{
    Key1.Key_Val  = S1;//按键读取
    Key1.Key_Down = Key1.Key_Val & (Key1.Key_Val ^ Key1.Key_Old);//检测下降沿
    Key1.Key_Up   = ~Key1.Key_Val & (Key1.Key_Val ^ Key1.Key_Old);//检测上升沿
    Key1.Key_Old  = Key1.Key_Val;//保存按键

    Key2.Key_Val  = S2;//按键读取
    Key2.Key_Down = Key2.Key_Val & (Key2.Key_Val ^ Key2.Key_Old);//检测下降沿
    Key2.Key_Up   = ~Key2.Key_Val & (Key2.Key_Val ^ Key2.Key_Old);//检测上升沿
    Key2.Key_Old  = Key2.Key_Val;//保存按键

    Key3.Key_Val  = S3;//按键读取
    Key3.Key_Down = Key3.Key_Val & (Key3.Key_Val ^ Key3.Key_Old);//检测下降沿
    Key3.Key_Up   = ~Key3.Key_Val & (Key3.Key_Val ^ Key3.Key_Old);//检测上升沿
    Key3.Key_Old  = Key3.Key_Val;//保存按键
}