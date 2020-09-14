#include "include.h"

void Straight(void)//识别直道和弯道
{
    if(g_ValueOfAD[4]>1000)
    {
        Controlvar.Straight_keep++;
    }
    else
    {
        Controlvar.Straight_keep=0;
    }
    
    if(Controlvar.Straight_keep>=1000)Controlvar.Straight_keep=1000;
    
    if(Controlvar.Straight_keep>Controlvar.Straight_delay)//时间5*Controlvar.Straight_delay   ms
    {
        Controlvar.Straight_flag=1;//直道
    }
    else
    {
        Controlvar.Straight_flag=0;//弯道
        
    }
    
    
}

void Cross(void)
{
    if(g_ValueOfAD[4]>1000&&g_ValueOfAD[4]<2000)
    {
        if(Controlvar.Cross_flag==0)//入十字环
        {
            if(Deviation[0]>0&&Deviation[0]<20&&Deviation[1]<-50)
            {
                Controlvar.Cross_flag=1;//左十字
            }
            
            if(Deviation[0]<0&&Deviation[0]>-20&&Deviation[1]>50)
            {
                Controlvar.Cross_flag=2;//右十字
            }
        }
        else if(Controlvar.Cross_flag==1)//出十字左环
        {
            if(Deviation[0]<0&&Deviation[0]>-20&&Deviation[1]>50)
            {
                Controlvar.Cross_flag=0;
            }
        }
        else                             //出十字右环
        {
            if(Deviation[0]>0&&Deviation[0]<20&&Deviation[1]<-50)
            {
                Controlvar.Cross_flag=0;
            }
        }
    }
}

