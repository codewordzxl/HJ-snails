#include "include.h"

void Straight(void)//ʶ��ֱ�������
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
    
    if(Controlvar.Straight_keep>Controlvar.Straight_delay)//ʱ��5*Controlvar.Straight_delay   ms
    {
        Controlvar.Straight_flag=1;//ֱ��
    }
    else
    {
        Controlvar.Straight_flag=0;//���
        
    }
    
    
}

void Cross(void)
{
    if(g_ValueOfAD[4]>1000&&g_ValueOfAD[4]<2000)
    {
        if(Controlvar.Cross_flag==0)//��ʮ�ֻ�
        {
            if(Deviation[0]>0&&Deviation[0]<20&&Deviation[1]<-50)
            {
                Controlvar.Cross_flag=1;//��ʮ��
            }
            
            if(Deviation[0]<0&&Deviation[0]>-20&&Deviation[1]>50)
            {
                Controlvar.Cross_flag=2;//��ʮ��
            }
        }
        else if(Controlvar.Cross_flag==1)//��ʮ����
        {
            if(Deviation[0]<0&&Deviation[0]>-20&&Deviation[1]>50)
            {
                Controlvar.Cross_flag=0;
            }
        }
        else                             //��ʮ���һ�
        {
            if(Deviation[0]>0&&Deviation[0]<20&&Deviation[1]<-50)
            {
                Controlvar.Cross_flag=0;
            }
        }
    }
}

