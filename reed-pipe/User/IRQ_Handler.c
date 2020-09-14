#include "include.h"

int count=0;
/*---------------------------------------------------------------
【函    数】PIT0_Interrupt
【功    能】PIT0的中断服务函数
【参    数】无
【返 回 值】无
【注意事项】注意进入后要清除中断标志位
----------------------------------------------------------------*/
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);       //清中断标志位
    /*用户添加所需代码*/  
   
    Controlvar.start_time++;
    if(Controlvar.start_time>=1000)Controlvar.start_time=1000;
    
    if(Controlvar.start_time>Controlvar.start_delay)
    {
    
        //出库
        Controlvar.OUT_time++;
        if(Controlvar.OUT_time>=1000)Controlvar.OUT_time=1000;
        if(Controlvar.OUT_time<Controlvar.OUT_time1+Controlvar.OUT_time2)
        {
            if(Controlvar.OUT_time<Controlvar.OUT_time1)//前进
            {
                FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
            }
            else                                        //打角
            {
                if(GPIO_Pin_Read(PTB5)==0)
                {
                    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_left);
                }
                else
                {
                    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_right);
                }
            }
            Controlvar.speed1_set=Controlvar.OUT_speed;
            Controlvar.speed2_set=Controlvar.OUT_speed;
            speed_output();
        }
        else
        {
            if(Controlvar.Warehousing_flag==0)
            {
                Steer();
                Speed_set_up();
                OLED();
                
                count++;
                if(count>=4)//间隔5ms*4
                {
                    count=0;
                    speed_output();
                }
                
                if(GPIO_Pin_Read(PTB6)==0)
                {
                    Controlvar.Warehousing_flag=1;
                }
            }
            else
            {
                if(Controlvar.Warehousing_flag==1)//停车
                {
                    Controlvar.speed1_set=0;
                    Controlvar.speed2_set=0;
                    
                    count++;
                    if(count>=4)//间隔5ms*4
                    {
                        count=0;
                        speed_output();
                    }
                   
                    if(enc1==0&&enc2==0)
                    {
                        Controlvar.Warehousing_flag=2;
                    }
                }
                else if(Controlvar.Warehousing_flag==2)//倒车
                {
                    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
                    
                    Controlvar.speed1_set=Controlvar.Reversing_speed;
                    Controlvar.speed2_set=Controlvar.Reversing_speed;
                    
                    count++;
                    if(count>=4)//间隔5ms*4
                    {
                        count=0;
                        speed_output();
                    }
                    
                    if(GPIO_Pin_Read(PTB6)==0)
                    {
                        Controlvar.Warehousing_flag=3;
                    }
                }
                else{
                    Controlvar.Warehousing_time++;
                    if(Controlvar.Warehousing_time>=1000)Controlvar.Warehousing_time=1000;
                    
                    if(Controlvar.Warehousing_time<Controlvar.Warehousing_time1)
                    {
                        if(GPIO_Pin_Read(PTB5)==0)
                        {
                            FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_left);
                            Controlvar.speed1_set=0;
                            Controlvar.speed2_set=Controlvar.Warehousing_speed;
                        }
                        else
                        {
                            FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_right);
                            Controlvar.speed1_set=Controlvar.Warehousing_speed;
                            Controlvar.speed2_set=0;
                        }
                        
                        count++;
                        if(count>=4)//间隔5ms*4
                        {
                            count=0;
                            speed_output();
                        }
                    }
                    else if(Controlvar.Warehousing_time<Controlvar.Warehousing_time1+Controlvar.Warehousing_time2)
                    {
                        FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
                        Controlvar.speed1_set=0;
                        Controlvar.speed2_set=0;
                        speed_output();
                    }
                    else
                    {
                        FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
                        Controlvar.speed1_set=0;
                        Controlvar.speed2_set=0;
                        speed_output();
                    }
                }
            }
                
          
        }
    }
    
}

///*---------------------------------------------------------------
//【函    数】PIT1_Interrupt
//【功    能】PIT1的中断服务函数
//【参    数】无
//【返 回 值】无
//【注意事项】注意进入后要清除中断标志位
//----------------------------------------------------------------*/
//void PIT1_IRQHandler()
//{
//    PIT_Flag_Clear(PIT1);       //清中断标志位
//    /*用户添加所需代码*/
//    
//    
//}

