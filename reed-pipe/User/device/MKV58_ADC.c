/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】模数转换
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#include "include.h"

/*------------------------------------------------------------------------------------------------------
【函    数】ADC_16Init
【功    能】初始化ADC16模式
【参    数】无
【返 回 值】无
【实    例】ADC_Init(); //初始化ADC16模块
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void ADC_Init(void)
{      
  SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //开启ADC0时钟
  SIM_SOPT7 &= ~(SIM_SOPT7_HSADC0AALTTRGEN_MASK  | SIM_SOPT7_HSADC0AALTTRGEN_MASK);
  SIM_SOPT7 |= SIM_SOPT7_HSADC0ATRGSEL(0);
 }

/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Start
【功    能】启动ADC转换
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【返 回 值】无
【实    例】ADC_Init(ADC0_SE8,ADC_12bit,1); //通道编号为 ADC0_SE8的引脚
【注意事项】使用前应初始化对应ADC模块
--------------------------------------------------------------------------------------------------------*/
void ADC_Start(ADCn_Ch_e adc_ch, ADC0_nbit bit)
{

    ADC0_CFG1 = (0  | ADC_CFG1_ADIV(2)              //时钟分频选择,分频系数为 2^n,2bit
                     | ADC_CFG1_ADLSMP_MASK          //采样时间配置，0为短采样时间，1 为长采样时间
                     | ADC_CFG1_MODE(bit)            //精确度选择
                     | ADC_CFG1_ADICLK(0)            //0为总线时钟,1为总线时钟/2,2为交替时钟（ALTCLK），3为 异步时钟（ADACK）。
                 );


    ADC0_CFG2  = (0  | ADC_CFG2_ADHSC_MASK          //高速配置,0为正常转换序列，1为高速转换序列
                     | ADC_CFG2_ADLSTS(0)           //长采样时间选择，ADCK为4+n个额外循环，额外循环，0为20，1为12，2为6，3为2
                     &~ADC_CFG2_MUXSEL_MASK         //选择通道A
                 );

    

    //写入 SC1A 启动转换
    ADC0_SC1(0) = (0 | ADC_SC1_AIEN_MASK            // 转换完成中断,0为禁止，1为使能
                     //| ADC_SC1_ADCH( 0x0c )       //ADC0 通道12
                     | ADC_SC1_ADCH( adc_ch )       //ADC0 通道13
                  );
   
}


/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Once
【功    能】启动ADC转换 获取一次ADC转换值
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【返 回 值】无
【实    例】ADC_Once(ADC0_SE8,ADC_12bit,1) 通道编号为 ADC0_SE8的引脚，进行一次ADC转换。
【注意事项】
--------------------------------------------------------------------------------------------------------*/
uint16_t ADC_Once(ADCn_Ch_e adc_ch, ADC0_nbit bit) 
{
    
    int result = 0;
    
    ADC_Start(adc_ch, bit);      //启动ADC转换

    while ((ADC0_SC1(0) & ADC_SC1_COCO_MASK )!= ADC_SC1_COCO_MASK);  
    result = ADC0_R(0);
    ADC0_SC1(0) &= ~ADC_SC1_COCO_MASK;
    return result;
    
}


/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Mid
【功    能】进行ADC3次转换取中间值
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【返 回 值】中值
【实    例】ADC_Mid(2,ADC0_SE8,ADC_12bit) 通道编号为 ADC0_SE8的引脚，进行3次ADC转换，选取中间值返回。
【注意事项】
--------------------------------------------------------------------------------------------------------*/
uint16_t ADC_Mid(ADCn_Ch_e adc_ch,ADC0_nbit bit) //中值滤波
{
    uint16_t i,j,k,tmp;
    //1.取3次A/D转换结果
    i = ADC_Once(adc_ch,bit); 
    j = ADC_Once(adc_ch,bit); 
    k = ADC_Once(adc_ch,bit); 
    //2.取中值
    if (i > j)
    {
        tmp = i; i = j; j = tmp;
    }
    if (k > j) 
        tmp = j;
    else if(k > i) 
        tmp = k; 
    else 
        tmp = i;
    
    return tmp;
}

/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Ave
【功    能】进行ADC3次转换取中间值 
【参    数】adc_ch：  ADC通道编号 
【参    数】bit   ：  精度选择ADC_8bit，ADC_12bit，ADC_10bit，ADC_16bit
【参    数】N     :   均值滤波次数（1~65535）
【返 回 值】平均值
【实    例】ADC_Ave(1,ADC0_SE8,ADC_12bit,100) 通道编号为 ADC0_SE8的引脚，进行100次ADC转换，计算出平均值返回。   
【注意事项】
--------------------------------------------------------------------------------------------------------*/
uint16_t ADC_Ave(ADCn_Ch_e adc_ch,ADC0_nbit bit,uint16_t N) //均值滤波
{
    uint32_t tmp = 0;
    uint8_t  i;
    
    for(i = 0; i < N; i++)
    {
        tmp += ADC_Mid(adc_ch,bit); 
    }
    tmp = tmp / N; 
    
    return (uint16_t)tmp;
}
