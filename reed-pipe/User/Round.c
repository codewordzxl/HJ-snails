#include "include.h"

void Roundabout(void)
{
    //疑似环岛判断
    if(Controlvar.Straight_flag)//直道上可能存在 十字 环岛 坡道
    {
        if(g_ValueOfAD[0]>g_ValueOfAD[1]&&g_ValueOfAD[3]>g_ValueOfAD[2])//疑似右环岛保持时间
        {
            Controlvar.Roundright_keep++;
            Controlvar.Roundleft_keep=0;
        }
        else
        {
            Controlvar.Roundright_keep=0;
        }
        
        if(g_ValueOfAD[1]>g_ValueOfAD[0]&&g_ValueOfAD[2]>g_ValueOfAD[3])//疑似左环岛保持时间
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
        
        if(Controlvar.Roundright_flag==0&&Controlvar.Roundleft_flag==0)//未识别到环岛时进行疑似判断
        {
            if(Controlvar.Roundright_keep>Controlvar.Round_delay)//疑似右环岛
            {
                Controlvar.Round_right=1;
                Controlvar.Round_left=0;
            }
            
            if(Controlvar.Roundleft_keep>Controlvar.Round_delay)//疑似左环岛
            {
                Controlvar.Round_left=1;
                Controlvar.Round_right=0;
            }
        }
    }
    else//弯道或坡顶
    {
        Controlvar.Roundright_keep=0;//弯道清空疑似环岛标志
        Controlvar.Roundleft_keep=0;
        Controlvar.Round_left=0;
        Controlvar.Round_right=0;
        
        Controlvar.Ramp_down=0;//进入弯道则下坡结束
    }
    
    //环岛确认
    if(Controlvar.Roundleft_flag==0&&Controlvar.Roundright_flag==0)
    {
        if(g_ValueOfAD[4]>2000&&g_ValueOfAD[0]+g_ValueOfAD[1]>3500)//环岛
        {
            if(Controlvar.Round_left)//疑似转为确立环岛存在标志
            {
                Controlvar.Roundleft_flag=1;//左环岛标志
                GPIO_Pin_Write(PTE29,0);
                Controlvar.Round_left=0;
            }
            
            if(Controlvar.Round_right)
            {
                Controlvar.Roundright_flag=1;//右环岛标志
                GPIO_Pin_Write(PTD15,0);
                Controlvar.Round_right=0;
            }
        }
    }
    
    if((Controlvar.Roundright_flag||Controlvar.Roundleft_flag)&&g_ValueOfAD[4]<2400)//进入环岛
    {
        Controlvar.RoundEnter=1;
    }
    
    //进入环岛
    if(Controlvar.RoundEnter&&Controlvar.RoundEnter_left==0&&Controlvar.RoundEnter_right==0)
    {
        if(Controlvar.Roundleft_flag)//进入左环岛
        {
            if(g_ValueOfAD[1]>g_ValueOfAD[0]&&g_ValueOfAD[3]>g_ValueOfAD[2])
            {
                Controlvar.RoundEnter_left=1;
            }
        }
        if(Controlvar.Roundright_flag)//进入右环岛
        {
            if(g_ValueOfAD[0]>g_ValueOfAD[1]&&g_ValueOfAD[2]>g_ValueOfAD[3])
            {
                Controlvar.RoundEnter_right=1;
            }
        }
        
    }
    
    if(g_ValueOfAD[4]<1500&&(Controlvar.RoundEnter_left||Controlvar.RoundEnter_right))//进环完成或识别错误
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
                
                Controlvar.Lostline_left=0;//进环过程可能出现反向丢线
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
                
                Controlvar.Lostline_left=0;//进环过程可能出现反向丢线
                Controlvar.Lostline_right=0;
                
                GPIO_Pin_Write(PTE29,1);
                GPIO_Pin_Write(PTD15,1);
            }
        }
    }
    
    
    
}
