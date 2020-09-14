#include "include.h"
#include "ZXL_ADC.h"

uint16_t g_ValueOfAD[5];//电感值

char txt[16];//OLED显示储存数据


int Deviation[3];
int last_Deviation[3][5];
int Deviation_d[3];

int Limit[5];//限幅
int Normalized[5];//归一化
int Bias[3];//偏差

int Level;
int Level_2;
int Level_3;

void ADC_get(void)
{
     int16_t  i,j,k,temp;
     int16_t  ad_valu[5][5],ad_valu1[5],ad_sum[5];

     for(i=0;i<5;i++)
     {
         ad_valu[0][i]=(uint16_t)(ADC_Ave(ADC0_SE5a, ADC_12bit, 5));//左水平
         ad_valu[1][i]=(uint16_t)(ADC_Ave(ADC0_SE4a, ADC_12bit, 5));//右水平
         ad_valu[2][i]=(uint16_t)(ADC_Ave(ADC0_DP3, ADC_12bit, 5));//左斜
         ad_valu[3][i]=(uint16_t)(ADC_Ave(ADC0_SE11, ADC_12bit, 5));//右斜
		 ad_valu[4][i]=(uint16_t)(ADC_Ave(ADC0_DP2, ADC_12bit, 5));//中
     }
/*=========================冒泡排序升序==========================*///舍弃最大值和最小值
     for(i=0;i<5;i++)
     {
        for(j=0;j<4;j++)
        {
           for(k=0;k<4-j;k++)
           {
              if(ad_valu[i][k] > ad_valu[i][k+1])
              {
                 temp = ad_valu[i][k+1];
                 ad_valu[i][k+1] = ad_valu[i][k];
                 ad_valu[i][k] = temp;
              }
           }
        }
     }
/*===========================中值滤波=================================*/
     for(i=0;i<5;i++)    //求中间三项的和
     {
        ad_sum[i] = ad_valu[i][1] + ad_valu[i][2] + ad_valu[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
        g_ValueOfAD[i] = (int16)(ad_valu1[i]/10*10);//将数值中个位数除掉
     }

}


void ADC_deal(void)
{
    OLED_CLS();
    ADC_get();
    
    //偏差
    Deviation[0]=100*(g_ValueOfAD[1]-g_ValueOfAD[0])/(g_ValueOfAD[1]+g_ValueOfAD[0]);//水平
    Deviation[1]=100*(g_ValueOfAD[3]-g_ValueOfAD[2])/(g_ValueOfAD[3]+g_ValueOfAD[2]);//八字
    Deviation[2]=100*(g_ValueOfAD[3]-g_ValueOfAD[2])/(g_ValueOfAD[0]+g_ValueOfAD[1]);
    
    //偏差微分
    Deviation_d[0]=Deviation[0]-last_Deviation[0][0];
    Deviation_d[1]=Deviation[1]-last_Deviation[1][0];
    Deviation_d[2]=Deviation[2]-last_Deviation[2][0];
    
    int i;
    for(i=0;i<4;i++)
    {
        last_Deviation[0][i]=last_Deviation[0][i+1];
        last_Deviation[1][i]=last_Deviation[1][i+1];
        last_Deviation[2][i]=last_Deviation[2][i+1];
    }
    last_Deviation[0][4]=Deviation[0];
    last_Deviation[1][4]=Deviation[1];
    last_Deviation[2][4]=Deviation[2];
    
    //限幅
    if(g_ValueOfAD[0]>Controlvar.SPDG_max)
    {
        Limit[0]=Controlvar.SPDG_max;
    }
    else
    {
        Limit[0]=g_ValueOfAD[0];
    }
    
    if(g_ValueOfAD[1]>Controlvar.SPDG_max)
    {
        Limit[1]=Controlvar.SPDG_max;
    }
    else
    {
        Limit[1]=g_ValueOfAD[1];
    }
    
    if(g_ValueOfAD[2]>Controlvar.XDG_max)
    {
        Limit[2]=Controlvar.XDG_max;
    }
    else
    {
        Limit[2]=g_ValueOfAD[2];
    }
    
    if(g_ValueOfAD[3]>Controlvar.XDG_max)
    {
        Limit[3]=Controlvar.XDG_max;
    }
    else
    {
        Limit[3]=g_ValueOfAD[3];
    }
    
    if(g_ValueOfAD[4]>Controlvar.ZZ_max)
    {
        Limit[4]=Controlvar.ZZ_max;
    }
    else
    {
        Limit[4]=g_ValueOfAD[4];
    }
    //归一化
    Normalized[0]=99*(Limit[0]-0)/(Controlvar.SPDG_max-0)+1;
    Normalized[1]=99*(Limit[1]-0)/(Controlvar.SPDG_max-0)+1;
    Normalized[2]=99*(Limit[2]-0)/(Controlvar.XDG_max-0)+1;
    Normalized[3]=99*(Limit[3]-0)/(Controlvar.XDG_max-0)+1;
    Normalized[4]=99*(Limit[4]-0)/(Controlvar.ZZ_max-0)+1;
    
    //偏差
    Bias[0]=Normalized[1]-Normalized[0];
    Bias[1]=Normalized[3]-Normalized[2];
    Bias[2]=100-Normalized[4];
    
    //
    Level=100*(g_ValueOfAD[1]-g_ValueOfAD[0])/(g_ValueOfAD[1]+g_ValueOfAD[0]+g_ValueOfAD[4]);
    
    
    Level_2=100*(Bias[0]/(Normalized[0]+Normalized[1]+Normalized[4]));
     
    Level_3=100*(g_ValueOfAD[3]-g_ValueOfAD[2])/(g_ValueOfAD[2]+g_ValueOfAD[3]+g_ValueOfAD[4]);
}


