#include "include.h"

void Roundabout(void)
{
    //���ƻ����ж�
    if(Controlvar.Straight_flag)//ֱ���Ͽ��ܴ��� ʮ�� ���� �µ�
    {
        if(g_ValueOfAD[0]>g_ValueOfAD[1]&&g_ValueOfAD[3]>g_ValueOfAD[2])//�����һ�������ʱ��
        {
            Controlvar.Roundright_keep++;
            Controlvar.Roundleft_keep=0;
        }
        else
        {
            Controlvar.Roundright_keep=0;
        }
        
        if(g_ValueOfAD[1]>g_ValueOfAD[0]&&g_ValueOfAD[2]>g_ValueOfAD[3])//�����󻷵�����ʱ��
        {
            Controlvar.Roundleft_keep++;
            Controlvar.Roundright_keep=0;
        }
        else
        {
            Controlvar.Roundleft_keep=0;
        }
        
        if(Controlvar.Roundright_keep>=1000)Controlvar.Roundright_keep=1000;
        if(Controlvar.Roundleft_keep>=1000)Controlvar.Roundleft_keep=1000;
        
        if(Controlvar.Roundright_flag==0&&Controlvar.Roundleft_flag==0)//δʶ�𵽻���ʱ���������ж�
        {
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
        }
    }
    else//������¶�
    {
        Controlvar.Roundright_keep=0;//���������ƻ�����־
        Controlvar.Roundleft_keep=0;
        Controlvar.Round_left=0;
        Controlvar.Round_right=0;
        
        Controlvar.Ramp_down=0;//������������½���
    }
    
    //����ȷ��
    if(Controlvar.Roundleft_flag==0&&Controlvar.Roundright_flag==0)
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
    
    if(g_ValueOfAD[4]<1500&&(Controlvar.RoundEnter_left||Controlvar.RoundEnter_right))//������ɻ�ʶ�����
    {
        if(Controlvar.RoundEnter_left)
        {
            if(g_ValueOfAD[2]<300||(g_ValueOfAD[2]>1000&&g_ValueOfAD[3]<1000))
            {
                Controlvar.Round_inside=1;
                
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundlike_flag=0;
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
        if(Controlvar.RoundEnter_right)
        {
            if(g_ValueOfAD[3]<300||(g_ValueOfAD[3]>1000&&g_ValueOfAD[2]<1000))
            {
                Controlvar.Round_inside=1;
                
                Controlvar.Round_left=0;
                Controlvar.Round_right=0;
                Controlvar.Roundleft_keep=0;
                Controlvar.Roundright_keep=0;
                Controlvar.Roundlike_flag=0;
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
