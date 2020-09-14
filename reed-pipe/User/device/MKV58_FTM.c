#include "include.h"
#include "MKV58_FTM.h"
#include "fsl_ftm.h"
/* 定义四个指针数组保存 FTMn_e 的地址 */
FTM_MemMapPtr FTMN[4] = {FTM0, FTM1, FTM2, FTM3}; 
/* FTM输出PWM时的周期计数值 */
static uint32 ftm_period[4];



/* FTM引脚初始化 */
static void FTM_PinInit(FTM_Type * ftmn, FTM_CHn_e ch, uint8_t mode);


/*------------------------------------------------------------------------------------------------------
【函    数】FTM_PwmInit
【功    能】初始化FTM模式PWM通道
【参    数】ftmn  :  模块名FTM0,FTM1，FTM2
【参    数】ch    :  通道号 对应引脚参考ftm.h文件
【参    数】freq  ： 设置PWM的频率
【参    数】duty  ： 设置PWM的占空比
【返 回 值】无
【实    例】FTM_PwmInit(FTM0, FTM_CH0, 10000, 500); //频率10KHZ，占空比为百分之（500/FTM_PRECISON *100）;
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void FTM_PwmInit(FTM_Type * ftmn, FTM_CHn_e ch, uint16_t freq, uint16_t duty)
{
  	uint8_t ftm_num;

	ftm_num = FTM_GetNum(ftmn);
	
    /* 开启FTM时钟配置端口复用 */
    FTM_PinInit(ftmn, ch, 1);
    
    /* 配置FTM模块的最佳时钟 */
    uint32_t clk_hz = (CLOCK_GetFastPeriphClkFreq()) ;     
    uint16_t mod = (clk_hz >> 16 ) / freq ;      
    uint8_t ps = 0;
    while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
    {
        ps++;
    }

    if(ps>0x07) return ;               // 断言， PS 最大为 0x07 ，超过此值，则 PWM频率设置过低，或 Bus 频率过高
    mod = (clk_hz >> ps) / freq;        // 求 MOD 的值
    ftm_period[ftm_num]=mod;
    uint16_t cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;
    
    /******************** 选择输出模式为 边沿对齐PWM *******************/
    //通道状态控制，根据模式来选择 边沿或电平
    FTM_CnSC_REG(ftmn, ch) &= ~FTM_CnSC_ELSA_MASK;
    FTM_CnSC_REG(ftmn, ch)  = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    FTM_SC_REG(ftmn)   =  FTM_SC_PS(ps) | FTM_SC_CLKS(1);

    FTM_MOD_REG(ftmn)   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
    FTM_CNTIN_REG(ftmn) = 0;                          //计数器初始化值。设置脉冲宽度：(CnV - CNTIN).
    FTM_CnV_REG(ftmn, ch) = cv;
    FTM_CNT_REG(ftmn)   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
}

/*------------------------------------------------------------------------------------------------------
【函    数】FTM_PwmDuty
【功    能】初始化FTM模式PWM通道
【参    数】ftmn  :  模块名FTM0,FTM1，FTM2
【参    数】ch    :  通道号 对应引脚参考ftm.h文件
【参    数】duty  ： 设置PWM的占空比
【返 回 值】无
【实    例】FTM_PwmDuty(FTM0, FTM_CH0, 500); //占空比为百分之（500/FTM_PRECISON *100）;
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void FTM_PwmDuty(FTM_Type * ftmn, FTM_CHn_e ch, u16 duty)
{ 
	uint8_t ftm_num;

	ftm_num = FTM_GetNum(ftmn);
	
    uint32_t mod = ftm_period[ftm_num];
    uint16_t cv = (duty * (mod - 0 + 1)) / FTM_PRECISON;
    // 配置FTM通道值
    /* Configure the fault mode, enable FTM mode and disable write protection */
    FTM_CnV_REG(ftmn, ch) = cv;

}


//////////////////////////////// 以下为正交解码 //////////////////////////////////////////



/*------------------------------------------------------------------------------------------------------
【函    数】FTM_ABInit
【功    能】初始化FTM模式正交解码模式
【参    数】ftmn  :  模块名FTM1，FTM2
【返 回 值】无
【实    例】FTM_ABInit(FTM1); //FTM1为正交解码模式  
【注意事项】使用管脚在ftm.h文件查看
--------------------------------------------------------------------------------------------------------*/
void FTM_ABInit(FTM_Type * ftmn)
{
   /* 开启FTM时钟配置端口复用 */
    FTM_PinInit(ftmn, FTM_CH0, 0);

	
    FTM_MODE_REG(ftmn)  |=    (0
                                     | FTM_MODE_WPDIS_MASK  //写保护禁止
                                     //| FTM_MODE_FTMEN_MASK   //使能 FTM
                                    );
    FTM_QDCTRL_REG(ftmn) |=   (0
                                    | FTM_QDCTRL_QUADMODE_MASK
                                     );
    FTM_CNTIN_REG(ftmn)   = 0;
    FTM_MOD_REG(ftmn)     = FTM_MOD_MOD_MASK;
    FTM_QDCTRL_REG(ftmn) |=   (0
                                    | FTM_QDCTRL_QUADEN_MASK
                                     );
    FTM_MODE_REG(ftmn)  |= FTM_QDCTRL_QUADEN_MASK;;
    FTM_CNT_REG(ftmn)     = 0;                    //计数器。只有低16位可用（写任何值到此寄存器，都会加载 CNTIN 的值）
    
}


/*------------------------------------------------------------------------------------------------------
【函    数】FTM_ABGet
【功    能】获取正交解码的计数数值
【参    数】ftmn  :  模块名FTM1，FTM2
【返 回 值】计数值
【实    例】FTM_ABGet(FTM1); //FTM1正交解码值  
【注意事项】使用管脚在ftm.h文件查看
--------------------------------------------------------------------------------------------------------*/
short FTM_ABGet(FTM_Type * ftmn)
{
    short val;
   
    val = FTM_CNT_REG(ftmn);
    
    FTM_CNT_REG(ftmn) = 0;             

    return val;
}

uint8_t FTM_GetNum(FTM_Type * ftmn)
{
	if(ftmn == FTM0)
	{
		return 0;
	}
	if(ftmn == FTM1)
	{
		return 1;
	}
	if(ftmn == FTM2)
	{
		return 2;
	}
	if(ftmn == FTM3)
	{
		return 3;
	}
	return 4;//原来为return -1;-1与返回类型不符
}

/*------------------------------------------------------------------------------------------------------
【函    数】FTM_PinInit
【功    能】初始化FTM管脚通道  内部调用
【参    数】ftmn  :  模块名FTM0,FTM1，FTM2
【参    数】ch    :  通道号 对应引脚参考ftm.h文件
【参    数】mode  ： 1：PWM管脚   0：正交解码管脚
【返 回 值】无
【实    例】FTM_PwmInit(FTM0, FTM_CH0, 1); //初始化FTM0的FTM_CH0通道为PWM模式
【注意事项】
--------------------------------------------------------------------------------------------------------*/
static void FTM_PinInit(FTM_Type * ftmn, FTM_CHn_e ch, uint8_t mode)
{
	uint8_t ftm_num;
    
	ftm_num = FTM_GetNum(ftmn);
    if(mode)
    {
        switch(ftm_num)
        {
          case 0:
            SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;       //使能FTM0时钟
            
            switch(ch)
            {
              case FTM_CH0:
                if(FTM0_CH0 == PTC1)
                {
                    PORT_PCR_REG(PORTC,1)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH0 == PTA3)
                {
                    PORT_PCR_REG(PORTA,3)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH0 == PTE24)
                {
                    PORT_PCR_REG(PORTE,24)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH0 == PTD0)
                {
                    PORT_PCR_REG(PORTD,0)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH1:
                if(FTM0_CH1 == PTC2)
                {
                    PORT_PCR_REG(PORTC,2)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH1 == PTA4)
                {
                    PORT_PCR_REG(PORTA,4)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH1 == PTE25)
                {
                    PORT_PCR_REG(PORTE,25)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH1 == PTD1)
                {
                    PORT_PCR_REG(PORTD,1)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH2:
                if(FTM0_CH2 == PTC3)
                {
                    PORT_PCR_REG(PORTC,3)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH2 == PTA5)
                {
                    PORT_PCR_REG(PORTA,5)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH2 == PTE29)
                {
                    PORT_PCR_REG(PORTE,29)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH2 == PTD2)
                {
                    PORT_PCR_REG(PORTD,2)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH3:
                if(FTM0_CH3 == PTC4)
                {
                    PORT_PCR_REG(PORTC,4)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH3 == PTA6)
                {
                    PORT_PCR_REG(PORTA,6)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH3 == PTE30)
                {
                    PORT_PCR_REG(PORTE,30)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH3 == PTD3)
                {
                    PORT_PCR_REG(PORTD,3)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH4:
                if(FTM0_CH4 == PTD4)
                {
                    PORT_PCR_REG(PORTD,4)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH4 == PTA7)
                {
                    PORT_PCR_REG(PORTA,7)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH4 == PTE26)
                {
                    PORT_PCR_REG(PORTE,26)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH4 == PTD4)
                {
                    PORT_PCR_REG(PORTD,4)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH5:
                if(FTM0_CH5 == PTD5)
                {
                    PORT_PCR_REG(PORTD,5)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH5 == PTA0)
                {
                    PORT_PCR_REG(PORTA,0)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH5 == PTD5)
                {
                    PORT_PCR_REG(PORTD,5)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH6:
                if(FTM0_CH6 == PTD6)
                {
                    PORT_PCR_REG(PORTD,6)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH6 == PTA1)
                {
                    PORT_PCR_REG(PORTA,1)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH6 == PTD6)
                {
                    PORT_PCR_REG(PORTD,6)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH7:
                if(FTM0_CH7 == PTD7)
                {
                    PORT_PCR_REG(PORTD,7)=PORT_PCR_MUX(4);
                }
                else if(FTM0_CH7 == PTA2)
                {
                    PORT_PCR_REG(PORTA,2)=PORT_PCR_MUX(3);
                }
                else if(FTM0_CH7 == PTD7)
                {
                    PORT_PCR_REG(PORTD,7)=PORT_PCR_MUX(4);
                }
                break;
              default:
                return;
            }
            break;
            
          case 1:
            SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
            switch(ch)
            {
              case FTM_CH0:
                if(FTM1_CH0 == PTA8)
                {
                    PORT_PCR_REG(PORTA,8)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH0 == PTA12)
                {
                    PORT_PCR_REG(PORTA,12)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH0 == PTB0)
                {
                    PORT_PCR_REG(PORTB,0)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH0 == PTE20)
                {
                    PORT_PCR_REG(PORTE,20)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH0 == PTA2)
                {
                    PORT_PCR_REG(PORTA,2)=PORT_PCR_MUX(6);
                }
                else if(FTM1_CH0 == PTD6)
                {
                    PORT_PCR_REG(PORTD,6)=PORT_PCR_MUX(5);
                }
                break;
                
                
              case FTM_CH1:
                if(FTM1_CH1 == PTA9)
                {
                    PORT_PCR_REG(PORTA,9)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH1 == PTA13)
                {
                    PORT_PCR_REG(PORTA,13)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH1 == PTB1)
                {
                    PORT_PCR_REG(PORTB,1)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH1 == PTE21)
                {
                    PORT_PCR_REG(PORTE,21)=PORT_PCR_MUX(3);
                }
                else if(FTM1_CH1 == PTA1)
                {
                    PORT_PCR_REG(PORTA,1)=PORT_PCR_MUX(6);
                }
                else if(FTM1_CH1 == PTD7)
                {
                    PORT_PCR_REG(PORTD,7)=PORT_PCR_MUX(5);
                }
                break;
                
              default:
                return;
            }
            break;
            
          case 2:
            SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;                           //使能FTM2时钟
            switch(ch)
            {
              case FTM_CH0:
                if(FTM2_CH0 == PTA10)
                {
                    PORT_PCR_REG(PORTA,10)=PORT_PCR_MUX(3);
                }
                else if(FTM2_CH0 == PTB18)
                {
                    PORT_PCR_REG(PORTB,18)=PORT_PCR_MUX(3);
                }
                else if(FTM2_CH0 == PTE22)
                {
                    PORT_PCR_REG(PORTE,22)=PORT_PCR_MUX(3);
                }
                break;
                
              case FTM_CH1:
                if(FTM2_CH1 == PTA11)
                {
                    PORT_PCR_REG(PORTA,11)=PORT_PCR_MUX(3);
                }
                else if(FTM2_CH1 == PTB19)
                {
                    PORT_PCR_REG(PORTB,19)=PORT_PCR_MUX(3);
                }
                else if(FTM2_CH1 == PTE23)
                {
                    PORT_PCR_REG(PORTE,23)=PORT_PCR_MUX(3);
                }
                break;
                
              default:
                return;
            }
            break;
            
            
          case 3:
            SIM_SCGC6 |= SIM_SCGC6_FTM3_MASK;
            switch(ch)
            {
              case FTM_CH0:
                if(FTM3_CH0 == PTE5)
                {
                    PORT_PCR_REG(PORTE,5)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH0 == PTD0)
                {
                    PORT_PCR_REG(PORTD,0)=PORT_PCR_MUX(4);
                }
                break;
                
              case FTM_CH1:
                if(FTM3_CH1 == PTE6)
                {
                    PORT_PCR_REG(PORTE,6)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH1 == PTD1)
                {
                    PORT_PCR_REG(PORTD,1)=PORT_PCR_MUX(4);
                }
                break;
              case FTM_CH2:
                if(FTM3_CH2 == PTE7)
                {
                    PORT_PCR_REG(PORTE,7)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH2 == PTD2)
                {
                    PORT_PCR_REG(PORTD,2)=PORT_PCR_MUX(4);
                }
                break;
              case FTM_CH3:
                if(FTM3_CH3 == PTE8)
                {
                    PORT_PCR_REG(PORTE,8)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH3 == PTD3)
                {
                    PORT_PCR_REG(PORTD,3)=PORT_PCR_MUX(4);
                }
                break;
              case FTM_CH4:
                if(FTM3_CH4 == PTE9)
                {
                    PORT_PCR_REG(PORTE,9)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH4 == PTD1)
                {
                    PORT_PCR_REG(PORTC,8)=PORT_PCR_MUX(4);
                }
                else if(FTM3_CH5 == PTC8)
                {
                    PORT_PCR_REG(PORTC,8)=PORT_PCR_MUX(3);
                }
                break;
              case FTM_CH5:
                if(FTM3_CH5 == PTE10)
                {
                    PORT_PCR_REG(PORTE,10)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH5 == PTC9)
                {
                    PORT_PCR_REG(PORTC,9)=PORT_PCR_MUX(3);
                }
                break;
              case FTM_CH6:
                if(FTM3_CH6 == PTE11)
                {
                    PORT_PCR_REG(PORTE,11)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH6 == PTC10)
                {
                    PORT_PCR_REG(PORTC,10)=PORT_PCR_MUX(3);
                }
                break;
              case FTM_CH7:
                if(FTM3_CH7 == PTE12)
                {
                    PORT_PCR_REG(PORTE,12)=PORT_PCR_MUX(6);
                }
                else if(FTM3_CH7 == PTC11)
                {
                    PORT_PCR_REG(PORTC,11)=PORT_PCR_MUX(3);
                }
                break;
              default:  return;
              
            }
            
        }
        
    }
    else
    {
        
        switch(ftm_num)
        {
            
          case 1:
            SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;       //使能FTM1时钟
            if(FTM1_QDPHA == PTA8)                  //管脚复用
            {
                
                PORTA_PCR8= PORT_PCR_MUX(6);
            }
            else if(FTM1_QDPHA == PTA12)
            {
                
                PORTA_PCR12= PORT_PCR_MUX(7);
            }
            else if(FTM1_QDPHA == PTB0)
            {
                
                PORTB_PCR0= PORT_PCR_MUX(6);
            }
            else if(FTM1_QDPHA == PTE20)
            {
                
                PORTE_PCR20= PORT_PCR_MUX(5);
            }
            
            if(FTM1_QDPHB == PTA9)
            {
                
                PORTA_PCR9= PORT_PCR_MUX(6);
            }
            else if(FTM1_QDPHB == PTA13)
            {
                
                PORTA_PCR13= PORT_PCR_MUX(7);
            }
            else if(FTM1_QDPHB == PTB1)
            {
                
                PORTB_PCR1= PORT_PCR_MUX(6);
            }
            else if(FTM1_QDPHB == PTE21)
            {
                
                PORTE_PCR21= PORT_PCR_MUX(5);
            }
            break;
            
            
          case 2:
            SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;                           //使能FTM2时钟
            if(FTM2_QDPHA == PTA10)                  //管脚复用
            {
                
                PORTA_PCR10= PORT_PCR_MUX(6);
            }
            else if(FTM2_QDPHA == PTB18)
            {
                
                PORTB_PCR18= PORT_PCR_MUX(6);
            }
            else if(FTM2_QDPHA == PTE22)
            {
                
                PORTE_PCR22= PORT_PCR_MUX(5);
            }
            else if(FTM2_QDPHA == PTA1)
            {
                
                PORTA_PCR1= PORT_PCR_MUX(5);
            }
            
            if(FTM2_QDPHB == PTA11)                  //管脚复用
            {
                
                PORTA_PCR11= PORT_PCR_MUX(6);
            }
            else if(FTM2_QDPHB == PTB19)
            {
                
                PORTB_PCR19= PORT_PCR_MUX(6);
            }
            else if(FTM2_QDPHB == PTE23)
            {
                
                PORTE_PCR23= PORT_PCR_MUX(5);
            }
            else if(FTM2_QDPHB == PTA2)
            {
                
                PORTA_PCR2= PORT_PCR_MUX(5);
            }
            break;
        }
        
    }
}

