#include "PID.h"
extern CarSpeedData BlueVel,AngleVel,PS2Vel,GreyVel;//小车速度相关结构体
extern Pid angle,cloudyaw,cloudpitch,cloudcamerax,cloudcameray,grey;//PID相关结构体
extern Steer steer0,steer1;//舵机结构体
extern Key Key1,Key2,Key3;//按键结构体
extern coordinate aim,laser;//相机坐标结构体
extern Euler CarAngle,SteerAngle;//欧拉角结构体
extern uint8_t GreyData[8];//循迹相关灰度变量
float fuzzy_err,last_fuzzy_err=0;
float kp_max=1.85;//p最大值1.65
float kd_max=1.55;//d最大值
float eff[7]={-0.5,-0.3,-0.1,0,0.1,0.3,0.5};

void PID_YAW()
{   
    angle.err=angle.target-CarAngle.Yaw;	
	if(angle.err>180)angle.err-=360;
	else if(angle.err<-180)angle.err+=360;
	LIMIT(angle.err, -30, 30);
    if (fabs(angle.err)<5)
        angle.err_plus+=angle.err;
    LIMIT(angle.err_plus, -angle.plus_limit, angle.plus_limit);
    if(angle.en)
        angle.out=angle.kp*angle.err+angle.kd*(angle.err-angle.err_last)+angle.ki*angle.err_plus;
	else 
        angle.out=angle.err_plus=0;
    angle.err_last=angle.err;
    AngleVel.z=angle.out;
}
void PID_Grey()
{
    static const float weight[8] = {-3, -2, -1, 0, 0, 1, 2, 3};
    float sum = 0, sum_weight = 0;
    for(int i = 0; i < 8; i++) {
        sum += GreyData[i];
        sum_weight += GreyData[i] * weight[i];
    }
    if(sum == 0) 
        grey.err = 0;
    else 
        grey.err = sum_weight / sum;
    grey.err_plus += grey.err;
    LIMIT(grey.err_plus, -grey.plus_limit, grey.plus_limit);
    if(grey.en)
        grey.out = grey.kp * grey.err
                 + grey.ki * grey.err_plus
                 + grey.kd * (grey.err - grey.err_last);
    else
        grey.out = 0;

    grey.err_last = grey.err;
    GreyVel.z = grey.out;
}
void PID_SteerCamerX()
{
    cloudcamerax.err = cloudcamerax.target - laser.y;
    if(fabs(cloudcamerax.err)>30)
        cloudcamerax.err_plus += cloudcamerax.err;
    else
        cloudcamerax.err_plus += cloudcamerax.err / 2;
    LIMIT(cloudcamerax.err_plus, -cloudcamerax.plus_limit, cloudcamerax.plus_limit);
    if(cloudcamerax.en)
        cloudcamerax.out = cloudcamerax.kp * cloudcamerax.err
                         + cloudcamerax.ki * cloudcamerax.err_plus
                         + cloudcamerax.kd * (cloudcamerax.err - cloudcamerax.err_last);
    else
        cloudcamerax.out = 0;
    cloudcamerax.err_last = cloudcamerax.err;
    LIMIT(cloudcamerax.out,-1000,1000);
    steer0.Pwm = steer0mid + cloudcamerax.out;
    LIMIT(steer0.Pwm, steer0min, steer0max);
    return;
}
void PID_SteerCamerY()
{
    cloudcameray.err = cloudcameray.target - laser.x;
    if(fabs(cloudcameray.err)>30)
        cloudcameray.err_plus += cloudcameray.err;
    else
        cloudcameray.err_plus += cloudcameray.err / 2;
    LIMIT(cloudcameray.err_plus, -cloudcameray.plus_limit, cloudcameray.plus_limit);
    if(cloudcameray.en)
        cloudcameray.out = cloudcameray.kp * cloudcameray.err
                         + cloudcameray.ki * cloudcameray.err_plus
                         + cloudcameray.kd * (cloudcameray.err - cloudcameray.err_last);
    else
        cloudcameray.out = 0;
    cloudcameray.err_last = cloudcameray.err;
    LIMIT(cloudcameray.out,-250,750);
    steer1.Pwm = steer1mid + cloudcameray.out;
    LIMIT(steer1.Pwm, steer1min, steer1max);
    return;
}
void PID_SteerYaw()
{
    cloudyaw.target = 0.0f;
    cloudyaw.get = SteerAngle.Yaw;
    cloudyaw.err = cloudyaw.target - cloudyaw.get;
    LIMIT(cloudyaw.err, -45, 45); 
    float E = cloudyaw.err;
    float EC = cloudyaw.err - cloudyaw.err_last;
    if(fabs(cloudyaw.err)>3)
        cloudyaw.err_plus += cloudyaw.err;
    else
        cloudyaw.err_plus += cloudyaw.err/5;
    LIMIT(cloudyaw.err_plus, -cloudyaw.plus_limit, cloudyaw.plus_limit);
    if(cloudyaw.en)
        cloudyaw.out = cloudyaw.kp * E
                     + cloudyaw.kd * EC
                     + cloudyaw.ki * cloudyaw.err_plus;
    else
        cloudyaw.out = 0;
    cloudyaw.err_last = cloudyaw.err;
    steer0.Pwm = 1500 + cloudyaw.out;
    LIMIT(steer0.Pwm, steer0min, steer0max);
}
void PID_SteerPitch()
{
    cloudpitch.target = -3.0f;
    cloudpitch.get = SteerAngle.Pitch;
    cloudpitch.err = cloudpitch.target - cloudpitch.get;
    LIMIT(cloudpitch.err, -30, 30);
    static float last_err = 0;
    if(cloudpitch.err>3|cloudpitch.err<-3)
        cloudpitch.out += cloudpitch.kp * cloudpitch.err+cloudpitch.kd * (cloudpitch.err - last_err);
    else 
        cloudpitch.out += cloudpitch.ki * cloudpitch.err;
    last_err = cloudpitch.err;
    LIMIT(cloudpitch.out,-cloudpitch.plus_limit, cloudpitch.plus_limit);
    steer1.Pwm = steer1min + cloudpitch.out;
    LIMIT(steer1.Pwm, steer1min, steer1max);
}
void PidPrc()
{
    // FuzzyPrc();//模糊PID处理
    PID_YAW();//小车偏航环
    // PID_Grey();//灰度循迹
    // PID_SteerYaw();//云台偏航环
    // PID_SteerPitch();//云台俯仰环
    // if(aim.x!=0)
    // {
    // PID_SteerCamerX();//云台相机X环
    // PID_SteerCamerY();//云台相机Y环
    // }

}
void FuzzyPrc()//模糊PID控制器
{
    float E=0,EC=0;
    fuzzy_err=angle.target-CarAngle.Yaw;
    E=fuzzy_err;
    EC=fuzzy_err-last_fuzzy_err;
    angle.kp=Kp_Fuzzy(E,EC); 
    angle.kd=Kd_Fuzzy(EC);
    last_fuzzy_err=fuzzy_err;
}
float Kp_Fuzzy(float E ,float EC){ 
    int rule_p[7][7]=
    {
        { 6 , 5 , 4 , 4 , 3 , 0 , 0},//-36
        { 6 , 4 , 3 , 3 , 2 , 0 , 0},//-24
        { 4 , 3 , 2 , 1 , 0 , 1 , 2},//-12
        { 2 , 1 , 1 , 0 , 1 , 1 , 2},//0
        { 2 , 1 , 0 , 1 , 2 , 3 , 4},//12
        { 0 , 0 , 2 , 3 , 3 , 4 , 6},//24
        { 0 , 1 , 3 , 4 , 4 , 5 , 6},//36
    };//模糊规则表 P
 
    int i2;
    /*输入量P语言值特征点*/
    float EFF[7];
    for(int i=0;i<=6;i++)
    {
        EFF[i]=eff[i];
    }
    /*输入量D语言值特征点*/
    float DFF[7];
    for(int i=0;i<=6;i++)
        DFF[i]=eff[i]/2;
    /*输出量U语言值特征点*/
    float UFF[7]; 
    for(i2=0;i2<=6;i2++)
    {
        UFF[i2]=kp_max/6*i2;
    }
    float U=0;  /*偏差,偏差微分以及输出值的精确量*/
    float PF[2]={0},DF[2]={0},UF[4]={0};
    /*偏差,偏差微分以及输出值的隶属度*/
    int Pn=0,Dn=0,Un[4]={0};
    float t1=0,t2=0,t3=0,t4=0,temp1=0,temp2=0;
    /*隶属度的确定*/
    if(E>EFF[0] && E<EFF[6])
    {
        if(E<=EFF[1])
        {
            Pn=-2;
            PF[0]=(EFF[1]-E)/(EFF[1]-EFF[0]);
        }
        else if(E<=EFF[2])
        {
            Pn=-1;
            PF[0]=(EFF[2]-E)/(EFF[2]-EFF[1]);
        }
        else if(E<=EFF[3])
        {
            Pn=0;
            PF[0]=(EFF[3]-E)/(EFF[3]-EFF[2]);
        }
        else if(E<=EFF[4])
        {
            Pn=1;
            PF[0]=(EFF[4]-E)/(EFF[4]-EFF[3]);
        }
        else if(E<=EFF[5])
        {
            Pn=2;
            PF[0]=(EFF[5]-E)/(EFF[5]-EFF[4]);
        }
        else if(E<=EFF[6])
        {
            Pn=3;
            PF[0]=(EFF[6]-E)/(EFF[6]-EFF[5]);
        }
    }
    else if(E<=EFF[0])
    {
        Pn=-2;
        PF[0]=1;
    }
    else if(E>=EFF[6])
    {
        Pn=3;
        PF[0]=0;
    }
    PF[1]=1-PF[0]; 
    //判断D的隶属度
    if(EC>DFF[0]&&EC<DFF[6])
    {
        if(EC<=DFF[1])
        {
            Dn=-2;
            DF[0]=(DFF[1]-EC)/(DFF[1]-DFF[0]);
        }
        else if(EC<=DFF[2])
        {
            Dn=-1;
            DF[0]=(DFF[2]-EC)/(DFF[2]-DFF[1]);
        }
        else if(EC<=DFF[3])
        {
            Dn=0;
            DF[0]=(DFF[3]-EC)/(DFF[3]-DFF[2]);
        }
        else if(EC<=DFF[4])
        {
            Dn=1;
            DF[0]=(DFF[4]-EC)/(DFF[4]-DFF[3]);
        }
        else if(EC<=DFF[5])
        {
            Dn=2;
            DF[0]=(DFF[5]-EC)/(DFF[5]-DFF[4]);
        }
        else if(EC<=DFF[6])
        {
            Dn=3;
            DF[0]=(DFF[6]-EC)/(DFF[6]-DFF[5]);
        }
    }
    //不在给定的区间内
    else if (EC<=DFF[0])
    {
        Dn=-2;
        DF[0]=1;
    }
    else if(EC>=DFF[6])
    {
        Dn=3;
        DF[0]=0;
    }
 
    DF[1]=1-DF[0];
    /*使用误差范围优化后的规则表rule[7][7]*/
    /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
    /*一般都是四个规则有效*/
    Un[0]=rule_p[Pn+2][Dn+2];
    Un[1]=rule_p[Pn+3][Dn+2];
    Un[2]=rule_p[Pn+2][Dn+3];
    Un[3]=rule_p[Pn+3][Dn+3];
 
    if(PF[0]<=DF[0])    //求小
        UF[0]=PF[0];
    else
        UF[0]=DF[0];
    if(PF[1]<=DF[0])
        UF[1]=PF[1];
    else
        UF[1]=DF[0];
    if(PF[0]<=DF[1])
        UF[2]=PF[0];
    else
        UF[2]=DF[1];
    if(PF[1]<=DF[1])
        UF[3]=PF[1];
    else
        UF[3]=DF[1];
    /*同隶属函数输出语言值求大*/
    if(Un[0]==Un[1])
    {
        if(UF[0]>UF[1])
            UF[1]=0;
        else
            UF[0]=0;
    }
    if(Un[0]==Un[2])
    {
        if(UF[0]>UF[2])
            UF[2]=0;
        else
            UF[0]=0;
    }
    if(Un[0]==Un[3])
    {
        if(UF[0]>UF[3])
            UF[3]=0;
        else
            UF[0]=0;
    }
    if(Un[1]==Un[2])
    {
        if(UF[1]>UF[2])
            UF[2]=0;
        else
            UF[1]=0;
    }
    if(Un[1]==Un[3])
    {
        if(UF[1]>UF[3])
            UF[3]=0;
        else
            UF[1]=0;
    }
    if(Un[2]==Un[3])
    {
        if(UF[2]>UF[3])
            UF[3]=0;
        else
            UF[2]=0;
    }
    t1=UF[0]*UFF[Un[0]];
    t2=UF[1]*UFF[Un[1]];
    t3=UF[2]*UFF[Un[2]];
    t4=UF[3]*UFF[Un[3]];
    temp1=t1+t2+t3+t4;
    temp2=UF[0]+UF[1]+UF[2]+UF[3];//模糊量输出
    if(temp2!=0)
        U=temp1/temp2;
    else {
        U=0;
    }

    return U;
}
float Kd_Fuzzy(float EC)
{
    int rule_d[7] = { 6 , 5 , 3 , 2 , 3 , 5 , 6};//模糊规则表 D
    float out=0;
    int i=0;
    float degree_left = 0,degree_right = 0;
    int degree_left_index = 0,degree_right_index = 0;
    float DFF[7];
    for(int i=0;i<=6;i++)
        DFF[i]=eff[i]/2;
    float UFF[7];
 
    for(i=0;i<7;i++)
            UFF[i]=kd_max/6*i;
 
    if(EC<DFF[0])
    {
        degree_left = 1;
        degree_right = 0;
        degree_left_index = 0;
    }
    else if (EC>DFF[6]) {
        degree_left = 1;
        degree_right = 0;
        degree_left_index = 6;
    }
    else {
        for(i=0;i<6;i++)
        {
            if(EC>=DFF[i]&&EC<DFF[i+1])
            {
                degree_left = (float)(DFF[i+1] - EC)/(DFF[i+1] - DFF[i]);
                degree_right = 1 - degree_left;
                degree_left_index = i;
                degree_right_index = i+1;
                break;
            }
        }
    }
    out = UFF[rule_d[degree_left_index]]*degree_left+UFF[rule_d[degree_right_index]]*degree_right; 
    return out;
}