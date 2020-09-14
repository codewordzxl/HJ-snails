#include "include.h"

uint16_t g_ValueOfAD[5];//���ֵ
int Deviation[3];//ƫ��
int Level;//ˮƽ��Ⱥ�


short enc1=0;//������1��ֵ
short enc2=0;//������2��ֵ
 
int count=0;

int Angle_Save[20];
/*---------------------------------------------------------------
����    ����PIT0_Interrupt
����    �ܡ�PIT0���жϷ�����
����    ������
���� �� ֵ����
��ע�����ע������Ҫ����жϱ�־λ
----------------------------------------------------------------*/
void PIT0_IRQHandler()
{
    PIT_Flag_Clear(PIT0);       //���жϱ�־λ
    /*�û�����������*/  

    /***********************************************��ʱ����*****************************************************/
    if(Controlvar.Start_count<1000){Controlvar.Start_count++;}//����ʱ�����
    if(Controlvar.Start_count>Controlvar.Start_time){Controlvar.Start_flag=1;}//����ʱ�����ֵ�ﵽ����ʱ��Ҫ��
    
    if(Controlvar.Start_flag)//�ﵽ����ʱ��
    {
      
        /***************************************�����߼���ʱ*************************************************/
      
        if(Controlvar.StartLine_count<1200){Controlvar.StartLine_count++;}
        if(Controlvar.StartLine_count>Controlvar.StartLine_time){Controlvar.StartLine_flag=1;}
        
        /********************************************����******************************************************/
        if(Controlvar.Out_count<1000){Controlvar.Out_count++;}//����ʱ�����
        if(Controlvar.Out_count<Controlvar.Out_time1)//����1��ǰ��
        {
            FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
            Controlvar.speed1_set=Controlvar.Out_speed;
            Controlvar.speed2_set=Controlvar.Out_speed;
        }
        else if(Controlvar.Out_count<Controlvar.Out_time1+Controlvar.Out_time2)//����2��ת��
        {
            if(Controlvar.Out_Direction==0)//���뿪�أ����һ����
            {
              FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Left);//���������

            }
            else
            {
              FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Right);//δ�����ҳ���
            }
            Controlvar.speed1_set=Controlvar.Out_speed;
            Controlvar.speed2_set=Controlvar.Out_speed;
        }
        else//��ɳ���
        {
            Controlvar.Out_flag=1;
        }
        
        /******************************************����Ѱ��*************************************************************/
        if(Controlvar.Out_flag&&Controlvar.Stop_flag==0)
        {
            ADC_Deal();
            Track();
            Steer();
            Spd_set();
        }
        
        /**********************************************���****************************************************************/
        if(Controlvar.Stop_flag)
        {
            if(Controlvar.Stop_count<1000){Controlvar.Stop_count++;}
            if(Controlvar.Stop_count<Controlvar.Stop_time1)
            {
                /************************************ת��****************************************/
                if(Controlvar.Out_Direction==0)//
                {
                  FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Left);//���������
                  Controlvar.speed1_set=Controlvar.Stop_speed;
                  Controlvar.speed2_set=Controlvar.Stop_speed+Controlvar.Stop_cs;
                }
                else
                {
                  FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_Right);//δ���������
                  Controlvar.speed1_set=Controlvar.Stop_speed+Controlvar.Stop_cs;
                  Controlvar.speed2_set=Controlvar.Stop_speed;
                }
            }
            else if(Controlvar.Stop_count<Controlvar.Stop_time1+Controlvar.Stop_time2)
            {
              
                FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_center);
                Controlvar.speed1_set=1;
                Controlvar.speed2_set=1;
            }
            else
            {
                /************************************ͣ��****************************************/
                Controlvar.speed1_set=0;
                Controlvar.speed2_set=0;
            }
            
        }
        count++;
        if(count>=4)
        {
          count=0;
          Spd_out();
        }
    }
    
    /**********************************************ͼ��ɼ��������߼��********************************************************/
    if(Field_Over_Flag)       //���һ��ͼ��ɼ�
    { 
      Get_Use_Image();      //�Ӳɼ�ͼ��������ȡ���Լ���Ҫʹ�õĴ�С
      Get_01_Value(1);      //��ֵ��ͼ������
      Draw_Road();          //����OLEDģ����ʾ��̬ͼ��
      
      if(Controlvar.StartLine_flag)
      {
          Start_Deect();//�����߼��
      }
      Field_Over_Flag= 0;  
    }
}

void ADC_Deal()//ADC��ȡ��ƫ�����
{
     int16_t  i,j,k,temp;
     int16_t  ad_valu[5][5],ad_valu1[5],ad_sum[5];

     for(i=0;i<5;i++)
     {
         ad_valu[0][i]=(uint16_t)(ADC_Ave(ADC0_SE5a, ADC_12bit, 5));//��ˮƽ
         ad_valu[1][i]=(uint16_t)(ADC_Ave(ADC0_SE4a, ADC_12bit, 5));//��ˮƽ
         ad_valu[2][i]=(uint16_t)(ADC_Ave(ADC0_DP3, ADC_12bit, 5));//��б
         ad_valu[3][i]=(uint16_t)(ADC_Ave(ADC0_SE11, ADC_12bit, 5));//��б
		 ad_valu[4][i]=(uint16_t)(ADC_Ave(ADC0_DP2, ADC_12bit, 5));//��
     }
/*=========================ð����������==========================*///�������ֵ����Сֵ
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
/*===========================��ֵ�˲�=================================*/
     for(i=0;i<5;i++)    //���м�����ĺ�
     {
        ad_sum[i] = ad_valu[i][1] + ad_valu[i][2] + ad_valu[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
        g_ValueOfAD[i] = (int16)(ad_valu1[i]/10*10);//����ֵ�и�λ������
     }
     
/***************************ƫ�����************************************/
     Deviation[0]=100*(g_ValueOfAD[1]-g_ValueOfAD[0])/(g_ValueOfAD[1]+g_ValueOfAD[0]+1);//ˮƽ
     Deviation[1]=100*(g_ValueOfAD[3]-g_ValueOfAD[2])/(g_ValueOfAD[3]+g_ValueOfAD[2]+1);//����
     Deviation[2]=100*(g_ValueOfAD[3]-g_ValueOfAD[2])/(g_ValueOfAD[0]+g_ValueOfAD[1]+1);
     
     Level=100*(g_ValueOfAD[1]-g_ValueOfAD[0])/(g_ValueOfAD[1]+g_ValueOfAD[0]+g_ValueOfAD[4]+1);
     
}

void Track()//����ʶ��
{
/*********************************************��������ж�************************************************************/
    if(Controlvar.Leave_count<1000&&(g_ValueOfAD[0]+g_ValueOfAD[1]+g_ValueOfAD[2]+g_ValueOfAD[3]+g_ValueOfAD[4]==0))
    {
        Controlvar.Leave_count++;
    }
    else
    {
        Controlvar.Leave_count=0;
    }
    if(Controlvar.Leave_count>Controlvar.Leave_time){Controlvar.Leave_flag=1;}
  
  
/***********************************************ֱ��������ж�********************************************************/
    if(g_ValueOfAD[4]>1000)//ֱ���ж�-----ֱ���ϴ���ʮ�֡��������µ�
    {
        Controlvar.Straight_keep++;//����ʱ��
    }
    else
    {
        Controlvar.Straight_keep=0;
    }
    
    if(Controlvar.Straight_keep>=1000)Controlvar.Straight_keep=1000;//��ֹ���
    
    if(Controlvar.Straight_keep>Controlvar.Straight_delay)//ʱ��5*Controlvar.Straight_delay   ms
    {
        Controlvar.Straight_flag=1;//ֱ��
    }
    else
    {
        Controlvar.Straight_flag=0;//���
    }
    
/*******************************************����ʶ��***************************************************************/
    
    //ʹ��Controlvar.Straight_flag���ж�ֱ��
    //���ܴ���ֱ�����ȣ�����δ��ֱ����Ӱ��ֱ��������Ԫ�ص��ж�
    if(Controlvar.Straight_flag)//ֱ������ʮ�֡��������µ�
    {
    /**************************************�µ��ж�***********************************************/      
        if(g_ValueOfAD[4]>3000)//�µ�
        {
          if(g_ValueOfAD[0]+g_ValueOfAD[1]<3500)//�������У��м���ֵ���ܴﵽ3000
          {
            if(Controlvar.Ramp_flag==0)//��ǰΪ���µ�
            {
              Controlvar.Ramp_flag=1;//�µ���־
            }
            else
            {
              Controlvar.Ramp_flag=0;
            }
          }
        }
    /*************************************���ƻ����ж�*********************************************/
        if(g_ValueOfAD[0]>g_ValueOfAD[1]&&g_ValueOfAD[3]>g_ValueOfAD[2])
        {
          Controlvar.Roundright_keep++;//�����һ�������ʱ��
          Controlvar.Roundleft_keep=0;
        }
        else
        {
          Controlvar.Roundright_keep=0;
        }
        
        if(g_ValueOfAD[1]>g_ValueOfAD[0]&&g_ValueOfAD[2]>g_ValueOfAD[3])
        {
          Controlvar.Roundleft_keep++;//�����󻷵�����ʱ��
          Controlvar.Roundright_keep=0;
        }
        else
        {
          Controlvar.Roundleft_keep=0;
        }
        
        if(Controlvar.Roundright_keep>=1000)Controlvar.Roundright_keep=1000;//��ֹ���
        if(Controlvar.Roundleft_keep>=1000)Controlvar.Roundleft_keep=1000;//��ֹ���
        
        
        if(Controlvar.Roundright_keep>Controlvar.Round_delay)//�����һ���
        {
          Controlvar.Round_right=1;
          Controlvar.Round_left=0;
        }
        
        if(Controlvar.Roundleft_keep>Controlvar.Round_delay)//�����󻷵�
        {
          Controlvar.Round_left=1;
          Controlvar.Round_right=0;
        }
        /*************************************����ȷ��*****************************************************/
        if(Controlvar.Roundleft_flag==0&&Controlvar.Roundright_flag==0&&(Controlvar.Round_left||Controlvar.Round_right))//��ǰδȷ�ϻ����Ҵ������ƻ���
        {
          if(g_ValueOfAD[4]>2000&&g_ValueOfAD[0]+g_ValueOfAD[1]>3500)//����
          {
            if(Controlvar.Round_left)//����תΪȷ���������ڱ�־
            {
              Controlvar.Roundleft_flag=1;//�󻷵���־
              GPIO_Pin_Write(PTE29,0);
              Controlvar.Round_left=0;
            }
            
            if(Controlvar.Round_right)
            {
              Controlvar.Roundright_flag=1;//�һ�����־
              GPIO_Pin_Write(PTD15,0);
              Controlvar.Round_right=0;
            }
          }
        }
        /******************************************���뻷��****************************************************************/
        if((Controlvar.Roundright_flag||Controlvar.Roundleft_flag)&&g_ValueOfAD[4]<2400)//���뻷��
        {
          Controlvar.RoundEnter=1;
        }
        
        //���뻷��
        if(Controlvar.RoundEnter&&Controlvar.RoundEnter_left==0&&Controlvar.RoundEnter_right==0)
        {
          if(Controlvar.Roundleft_flag)//�����󻷵�
          {
            if(g_ValueOfAD[1]>g_ValueOfAD[0]&&g_ValueOfAD[3]>g_ValueOfAD[2])
            {
              Controlvar.RoundEnter_left=1;
            }
          }
          if(Controlvar.Roundright_flag)//�����һ���
          {
            if(g_ValueOfAD[0]>g_ValueOfAD[1]&&g_ValueOfAD[2]>g_ValueOfAD[3])
            {
              Controlvar.RoundEnter_right=1;
            }
          }
        }
        
    }
    else//���
    {
        Controlvar.Round_left=0;
        Controlvar.Round_right=0;
        Controlvar.Roundleft_flag=0;//�󻷵���־
        Controlvar.Roundright_flag=0;//�һ�����־
    }
   
    /******************************************��������ж�**************************************************/
    if(g_ValueOfAD[4]<1500&&(Controlvar.RoundEnter_left||Controlvar.RoundEnter_right))//������ɻ�ʶ�����
    {
        if(Controlvar.RoundEnter_left)
        {
            if(Controlvar.speed==20)
            {
                if(g_ValueOfAD[3]<300||g_ValueOfAD[0]+g_ValueOfAD[1]<2000)//Ĭ���ٶ���������
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//�������̿��ܳ��ַ�����
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
            else
            {
                if(g_ValueOfAD[0]+g_ValueOfAD[1]<2200)
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//�������̿��ܳ��ַ�����
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
            
        }
        if(Controlvar.RoundEnter_right)
        {
            if(Controlvar.speed==20)
            {
                if(g_ValueOfAD[3]<300||g_ValueOfAD[0]+g_ValueOfAD[1]<2000)//Ĭ���ٶ��ҽ������
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//�������̿��ܳ��ַ�����
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
            else
            {
                if(g_ValueOfAD[0]+g_ValueOfAD[1]<2200)
              {
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundright_flag=0;
                Controlvar.Roundleft_flag=0;
                Controlvar.RoundEnter=0;
                Controlvar.RoundEnter_left=0;
                Controlvar.RoundEnter_right=0;
                
                Controlvar.Lostline_left=0;//�������̿��ܳ��ַ�����
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
                }
            }
        }
    }
/*����ʶ��***************************************************************************************************************************************************************************/
}

void Steer()
{
/******************************************************���뻷��******************************************************/
    if(Controlvar.RoundEnter_left||Controlvar.RoundEnter_right)
    {
          Controlvar.Duoji_P=Controlvar.Round_P;
          Controlvar.Duoji_RD=Controlvar.Round_D;
          Controlvar.Deviation=Deviation[1]*Controlvar.Round_coeff;//б��������뻷
          if(Controlvar.RoundEnter_left)
          {
              if(Controlvar.Deviation<10)
              {
                  Controlvar.Deviation=Controlvar.Round_FD;
              }
              if(Controlvar.Deviation<0)
              {
                  Controlvar.Deviation=-(Controlvar.Deviation-Controlvar.Round_QD);
              }
          }
          if(Controlvar.RoundEnter_right)
          {
              if(Controlvar.Deviation>-10)
              {
                  Controlvar.Deviation=-Controlvar.Round_FD;
              }
              if(Controlvar.Deviation>0)
              {
                  Controlvar.Deviation=-(Controlvar.Deviation+Controlvar.Round_QD);
              }
          }
          if(Controlvar.Deviation>70)Controlvar.Deviation=70;
          if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
          Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);//��PD����ת��
/*****���뻷��****************************************************************************************************************/
    }
    else
    {
/*******************************************************�ǻ���******************************************************************/
          if(Controlvar.Straight_flag)//ֱ��
          {
              Controlvar.Duoji_P=Controlvar.Straight_P;
              Controlvar.Duoji_RD=Controlvar.Straight_D;
              Controlvar.Deviation=Level;
              
              if(Controlvar.Deviation>70)Controlvar.Deviation=70;
              if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
              Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);
              
          }
          else
          {
              Controlvar.Duoji_P=Controlvar.Curve_P;
              Controlvar.Duoji_RD=Controlvar.Curve_D;
              Controlvar.Deviation=(int)(Level*1.5);
              
              if(Controlvar.Deviation>70)Controlvar.Deviation=70;
              if(Controlvar.Deviation<-70)Controlvar.Deviation=-70;
              Controlvar.Steer_variety=Steer_PD(Controlvar.Deviation);
          }
          
          /*****************************************����****************************************/
          if(g_ValueOfAD[4]<Controlvar.Lostline_Threshold)
          {
              if(Controlvar.Lostline_right==0&&Controlvar.Lostline_left==0)//�����ڶ���ʱ����
              {
                  int i,zuo=0,you=0;
                  for(i=0;i<19;i++)
                  {
                    if(Angle_Save[i]>0)zuo++;
                    if(Angle_Save[i]<0)you++;
                  }
                  if(zuo>you)
                  {
                    Controlvar.Lostline_left=1;//��ת����
                  }
                  if(zuo<you)
                  {
                    Controlvar.Lostline_right=1;//��ת����
                  }
                  GPIO_Pin_Write(PTC0,0);//����ָʾ�ƣ�����
              }
          }
          if(g_ValueOfAD[4]>Controlvar.Lostline_Restore)
          {
              GPIO_Pin_Write(PTC0,1);//����ָʾ�ƣ���
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
    }
/**********�ǻ���*************************************************************************************************/
    
    
    
    /******************************************ת�Ǽ�¼************************************************************/
    int i;
    for(i=0;i<19;i++)
    {
        Angle_Save[i]=Angle_Save[i+1];
    }
    Angle_Save[19]=Controlvar.Steer_variety;
    
    /*******************************************������***********************************************************/
    Controlvar.Steer_out=Controlvar.Steer_center+Controlvar.Steer_variety;//Steer_out���PWM���ֵ
    if(Controlvar.Steer_out>Controlvar.Steer_Left)Controlvar.Steer_out=Controlvar.Steer_Left;//��ת�޷�
    if(Controlvar.Steer_out<Controlvar.Steer_Right)Controlvar.Steer_out=Controlvar.Steer_Right;//��ת�޷�
    FTM_PwmDuty(FTM3, FTM_CH6,Controlvar.Steer_out);//���PWM���
}

void Spd_set()
{
/************************************************�����뻷***************************************************/
    if(Controlvar.Roundright_flag||Controlvar.Roundleft_flag)
    {
        if(Controlvar.speed<25)
        {
            Controlvar.speed1_set=Controlvar.speed;
            Controlvar.speed2_set=Controlvar.speed;
        }
        else
        {
            Controlvar.speed1_set=Controlvar.Round_speed;
            Controlvar.speed2_set=Controlvar.Round_speed;
        }
        
        if(Controlvar.RoundEnter_right)
        {
            Controlvar.speed1_set+=Controlvar.Round_Dif;
        }
        else
        {
            Controlvar.speed2_set+=Controlvar.Round_Dif;
        }
    }
    else
    {
/****************************************************ת�����*******************************************************/
        if(Controlvar.Lostline_right==0&&Controlvar.Lostline_left==0)
        {
            if(Controlvar.Steer_variety>20)
            {
              Controlvar.speed1_set=Controlvar.speed-(int)(Controlvar.Steer_variety*Controlvar.Cruve_Dif);
              Controlvar.speed2_set=Controlvar.speed;
            }
            else if(Controlvar.Steer_variety<-20)
            {
              Controlvar.speed1_set=Controlvar.speed;
              Controlvar.speed2_set=Controlvar.speed+(int)(Controlvar.Steer_variety*Controlvar.Cruve_Dif);
            }
            else 
            {
              Controlvar.speed1_set=Controlvar.speed+Controlvar.Straight_plus;
              Controlvar.speed2_set=Controlvar.speed+Controlvar.Straight_plus;
            }
        }
        else
        {
        /*********************************���߲���**************************************/
            if(Controlvar.Lostline_right)
            {
              Controlvar.speed1_set=Controlvar.speed;
              Controlvar.speed2_set=Controlvar.speed-Controlvar.Lostline_Dif;
            }
            if(Controlvar.Lostline_left)
            {
              Controlvar.speed1_set=Controlvar.speed-Controlvar.Lostline_Dif;
              Controlvar.speed2_set=Controlvar.speed;
            }
        }
    }
    
    /*******************************************�������ͣ��**********************************************/
    if(Controlvar.Leave_flag)//�������
    {
        Controlvar.speed1_set=0;
        Controlvar.speed2_set=0;
    }
    
}

void Spd_out()
{
    enc1 = FTM_ABGet(FTM1)/10;
    enc2 = -FTM_ABGet(FTM2)/10;
    
    Controlvar.speed1_get=enc1;//speed1_get�����ٶ�
    PID_left(Controlvar.speed1_set);
    
    Controlvar.speed2_get=enc2;
    PID_right(Controlvar.speed2_set);
    
    if(Controlvar.Pwm1_Out>=0)//����
   {
     FTM_PwmDuty(FTM0,FTM_CH0,0);   //�����ת   
     FTM_PwmDuty(FTM0,FTM_CH1,(u16)Controlvar.Pwm1_Out);  
   }
   else
   {
      FTM_PwmDuty(FTM0,FTM_CH1,0);   //�����ת  
      FTM_PwmDuty(FTM0,FTM_CH0,-(u16)Controlvar.Pwm1_Out);  
   }
   
   if(Controlvar.Pwm2_Out>=0)//�ҵ��
   {
     FTM_PwmDuty(FTM0,FTM_CH2,0);   //�����ת   
     FTM_PwmDuty(FTM0,FTM_CH3,(u16)Controlvar.Pwm2_Out);  
   }
   else
   {
      FTM_PwmDuty(FTM0,FTM_CH3,0);   //�����ת  
      FTM_PwmDuty(FTM0,FTM_CH2,-(u16)Controlvar.Pwm2_Out);  
   }
   
   if(Controlvar.speed1_set==0&&Controlvar.speed1_get<=2&&Controlvar.speed2_set==0&&Controlvar.speed2_get<=2)
   {
      FTM_PwmDuty(FTM0,FTM_CH0,0);
      FTM_PwmDuty(FTM0,FTM_CH1,0);
      FTM_PwmDuty(FTM0,FTM_CH2,0);
      FTM_PwmDuty(FTM0,FTM_CH3,0);
   }
}