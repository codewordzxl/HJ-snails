#include "system.h"
#include "include.h"

void system_run(void)
{
    system_init();//初始化
    while(1){

    }
}

void Steer(void)
{
    ADC_deal();
    Straight();
    Roundabout();
    //直道与弯道
     if(Controlvar.Straight_flag)//直道
    {
        Controlvar.Duoji_P=0.8f;
        Controlvar.Duoji_RD=9.f;
        Controlvar.Deviation=Level;
        GPIO_Pin_Write(PTA17,0);
    }
    else//弯道
    {
        GPIO_Pin_Write(PTA17,1);
        Controlvar.Duoji_P=1.5f;
        Controlvar.Duoji_RD=15.f;
        Controlvar.Deviation=Level;
        
        if(g_ValueOfAD[4]<500)
        {
            GPIO_Pin_Write(PTC0,0);
            if(g_ValueOfAD[0]+g_ValueOfAD[2]>g_ValueOfAD[1]+g_ValueOfAD[3])
            {
                Controlvar.Lostline_right=1;
            }
            else
            {
                Controlvar.Lostline_left=1;
            }
        }
        
    }
    if(Controlvar.Deviation>70)Controlvar.Deviation=70;
    if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
    Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);
    
    if(g_ValueOfAD[4]>600)
    {
        GPIO_Pin_Write(PTC0,1);
        Controlvar.Lostline_right=0;
        Controlvar.Lostline_left=0;
    }
    
    if(Controlvar.Lostline_right)
    {
        Controlvar.Steer_variety=-80;
    }
    if(Controlvar.Lostline_left)
    {
        Controlvar.Steer_variety=80;
    }
    
    //环岛
    if(Controlvar.RoundEnter_left||Controlvar.RoundEnter_right)
    {
        Controlvar.Duoji_P=1.1f;
        Controlvar.Duoji_RD=20.f;
        Controlvar.Deviation=Deviation[1];
        if(Controlvar.RoundEnter_left)
        {
            if(Controlvar.Deviation<0)
            {
                Controlvar.Deviation=-(Controlvar.Deviation-20);
            }
        }
        if(Controlvar.RoundEnter_right)
        {
            if(Controlvar.Deviation>0)
            {
                Controlvar.Deviation=-(Controlvar.Deviation+20);
            }
        }
        if(Controlvar.Deviation>70)Controlvar.Deviation=70;
        if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
        Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);
    }
    
    //舵机输出
    Controlvar.Steer_out=Controlvar.Steer_center+Controlvar.Steer_variety;
    if(Controlvar.Steer_out>750)Controlvar.Steer_out=750;//限幅
    if(Controlvar.Steer_out<590)Controlvar.Steer_out=590;
    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_out);
}

void Speed_set_up(void)
{
    if(Controlvar.Steer_variety>20)
    {
        Controlvar.speed1_set=Controlvar.speed-Controlvar.Steer_variety*0.05;
        Controlvar.speed2_set=Controlvar.speed;
    }
    else if(Controlvar.Steer_variety<-20)
    {
        Controlvar.speed1_set=Controlvar.speed;
        Controlvar.speed2_set=Controlvar.speed+Controlvar.Steer_variety*0.05;
    }
    else 
    {
        Controlvar.speed1_set=Controlvar.speed;
        Controlvar.speed2_set=Controlvar.speed;
    }
   
    
    if(Controlvar.Lostline_right)
    {
        Controlvar.speed1_set=Controlvar.speed;
        Controlvar.speed2_set=Controlvar.speed-4;
    }
    if(Controlvar.Lostline_left)
    {
        Controlvar.speed1_set=Controlvar.speed-4;
        Controlvar.speed2_set=Controlvar.speed;
    }
    
    if(g_ValueOfAD[0]+g_ValueOfAD[1]+g_ValueOfAD[2]+g_ValueOfAD[3]+g_ValueOfAD[4]<100)
    {
        Controlvar.speed1_set=0;
        Controlvar.speed2_set=0;
    }
    
    if(GPIO_Pin_Read(PTB6)==0)
    {
        Controlvar.stop_flag=1;
    }
    
    if(Controlvar.stop_flag)
    {
        Controlvar.speed1_set=0;
        Controlvar.speed2_set=0;
    }
}

void OLED(void)
{
    if(GPIO_Pin_Read(PTB0)==0)//ADC值显示
    {
        OLED_CLS();
        int i;
        for(i=0;i<5;i++)
        {
            sprintf(txt,"adc%d:%d",i,g_ValueOfAD[i]);
            OLED_P6x8Str(0,i,(uint8_t *)txt);
        }
        delayms(1000);
    }
    
    if(GPIO_Pin_Read(PTB1)==0)//偏差显示
    {
        OLED_CLS();
        sprintf(txt,"DSP:%d",Deviation[0]);
        OLED_P6x8Str(0,0,(uint8_t *)txt);
        sprintf(txt,"DX:%d",Deviation[1]);
        OLED_P6x8Str(0,1,(uint8_t *)txt);
        sprintf(txt,"NSP:%d",Bias[0]);
        OLED_P6x8Str(0,2,(uint8_t *)txt);
        sprintf(txt,"NX:%d",Bias[1]);
        OLED_P6x8Str(0,3,(uint8_t *)txt);
        sprintf(txt,"L:%d",Level);
        OLED_P6x8Str(0,4,(uint8_t *)txt);
        sprintf(txt,"L2:%d",Level_2);
        OLED_P6x8Str(0,5,(uint8_t *)txt);
        sprintf(txt,"L3:%d",Level_3);
        OLED_P6x8Str(0,6,(uint8_t *)txt);
        sprintf(txt,"ST:%d",Controlvar.Steer_out);
        OLED_P6x8Str(0,7,(uint8_t *)txt);
        
        
        delayms(1000);
    }
}
