/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_ADC.h"
/*------------------------------------------------------------------------------------------------------
【函    数】ADC_Get
【功    能】获取龙邱母板上的8路ADC值
【参    数】num ： 0 ~ 7
【返 回 值】ADC读取的值
【实    例】ADC_Get(0);           //获取AD0接口的值
【注意事项】ADC0_DP1  = 1,        // PTE16         
            ADC0_DP2  = 2,        // PTE4      
            ADC0_DP3  = 3,        // PTE11      
            ADC0_SE4a = 4,        // PTE6      
            ADC0_SE5a = 5,        // PTE18     
            ADC0_SE8  = 8,        // ADC0_SE8 
            ADC0_SE9  = 9,        // PTE17    
            ADC0_SE10 =10,        // PTE5
--------------------------------------------------------------------------------------------------------*/
uint16_t ADC_Get(uint8_t num)
{
    switch(num)
    {
      case 0:
        return ADC_Mid(ADC0_DP1, ADC_12bit);
        break;
        
      case 1:
        return ADC_Mid(ADC0_DP2, ADC_12bit);
        break;
        
      case 2:
        return ADC_Mid(ADC0_DP3, ADC_12bit);
        break;
        
      case 3:
        return ADC_Mid(ADC0_SE4a, ADC_12bit);
        break;
        
      case 4:
        return ADC_Mid(ADC0_SE5a, ADC_12bit);
        break;
     
      case 5:
        return ADC_Mid(ADC0_SE8, ADC_12bit);
        break;
        
      case 6:
        return ADC_Mid(ADC0_SE9, ADC_12bit);
        break;
        
      case 7:
        return ADC_Mid(ADC0_SE10, ADC_12bit);
        break;
        

        
      default:
        return 0;   
    }
}



/*------------------------------------------------------------------------------------------------------
【函    数】Test_ADC
【功    能】测试母板上的10路ADC接口
【参    数】无
【返 回 值】无
【实    例】Test_ADC(); //打印并显示电压
【注意事项】
--------------------------------------------------------------------------------------------------------*/
#ifdef LQ_OLED
void Test_ADC(void)
{
    LED_Init();
    KEY_Init();
    UART4_Init();
    
    ADC_Init();
    
    OLED_Init();
    OLED_CLS();
    printf("ADC 测试例程");
    printf("ADC0_DP1  = 1,        // PTE16   \n ");  
    printf("ADC0_DP2  = 2,        // PTE4    \n ");
    printf("ADC0_DP3  = 3,        // PTE11   \n ");
    printf("ADC0_SE4a = 4,        // PTE6    \n ");
    printf("ADC0_SE5a = 5,        // PTE18   \n ");
    printf("ADC0_SE8  = 8,        // ADC0_SE8\n ");
    printf("ADC0_SE9  = 9,        // PTE17   \n ");
    printf("ADC0_SE10 =10,        // PTE5    \n ");
    
    
    
    
    uint16_t batv0 = (uint16_t)(ADC_Get(0)*0.806);
    uint16_t batv1 = (uint16_t)(ADC_Get(1)*0.806);
    uint16_t batv2 = (uint16_t)(ADC_Get(2)*0.806);
    uint16_t batv3 = (uint16_t)(ADC_Get(3)*0.806);
    uint16_t batv4 = (uint16_t)(ADC_Get(4)*0.806);
    uint16_t batv5 = (uint16_t)(ADC_Get(5)*0.806);
    uint16_t batv6 = (uint16_t)(ADC_Get(6)*0.806);
    uint16_t batv7 = (uint16_t)(ADC_Get(7)*0.806);
    uint8_t key = 0;
    char txt[16];
    while(1)
    {
        /* 获取 ADC通道值 */
        batv0 = (uint16_t)(ADC_Get(0)*0.806);
        batv1 = (uint16_t)(ADC_Get(1)*0.806);
        batv2 = (uint16_t)(ADC_Get(2)*0.806);
        batv3 = (uint16_t)(ADC_Get(3)*0.806);
        batv4 = (uint16_t)(ADC_Get(4)*0.806);
        batv5 = (uint16_t)(ADC_Get(5)*0.806);
        batv6 = (uint16_t)(ADC_Get(6)*0.806);
        batv7 = (uint16_t)(ADC_Get(7)*0.806); 
        switch(KEY_Read(1))     //按键按下翻页
        {
          case 1:
            OLED_CLS();          //LCD清屏
            key = 0;            //K0 按下 第一页
            break;           
          case 2: 
            OLED_CLS();          //LCD清屏
            key = 1;            //K1 按下 第二页
            break;
          case 3: 
            OLED_CLS();          //LCD清屏
            key = 2;            //K1 按下 第二页
            break;
          default:
            
            break;
        }
        
        if(0 == key)
        {
            OLED_P8x16Str(4,0,(uint8_t*)"LQ ADC Test Bat"); 
            
            printf("\r\n/E16***********************%5d mv\r\n ",batv0);
            sprintf(txt,"E16:%5d mv ",batv0);
            OLED_P8x16Str(20,2,(uint8_t*)txt);
            
            printf("\r\n/E4***********************%5d mv\r\n ",batv1);
            sprintf(txt,"E4 :%5d mv ",batv1);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
            
            printf("\r\n/E11***********************%5d mv\r\n ",batv2);
            sprintf(txt,"E11:%5d mv ",batv2);
            OLED_P8x16Str(20,6,(uint8_t*)txt);
            
        }
        if(1 == key)
        {
            printf("\r\n/E6***********************%5d mv\r\n ",batv3);
            sprintf(txt,"E6 :%5d mv ",batv3);
            OLED_P8x16Str(20,0,(uint8_t*)txt);
            
            printf("\r\n/E18***********************%5d mv\r\n ",batv4);
            sprintf(txt,"E18:%5d mv ",batv4);
            OLED_P8x16Str(20,2,(uint8_t*)txt);
            
            printf("\r\n/SE8***********************%5d mv\r\n ",batv5);
            sprintf(txt,"SE8:%5d mv ",batv5);
            OLED_P8x16Str(20,4,(uint8_t*)txt);
            
            printf("\r\n/E17***********************%5d mv\r\n ",batv6);
            sprintf(txt,"E17:%5d mv ",batv6);
            OLED_P8x16Str(20,6,(uint8_t*)txt);    
        }
        if(2 == key)
        {
            printf("\r\n/E5***********************%5d mv\r\n ",batv7);
            sprintf(txt,"E5 :%5d mv ",batv7);
            OLED_P8x16Str(20,0,(uint8_t*)txt);
            
        }
   
        LED_Reverse(1);    
        delayms(100);
    }
    
}
#else
void Test_ADC(void)
{
    LED_Init();
    UART4_Init();
    
    ADC_Init();
    
    TFTSPI_Init(1);                 //LCD初始化  0:横屏  1：竖屏
    TFTSPI_CLS(u16BLUE);
    printf("ADC 测试例程");
    printf("ADC0_DP1  = 1,        // PTE16   \n ");  
    printf("ADC0_DP2  = 2,        // PTE4    \n ");
    printf("ADC0_DP3  = 3,        // PTE11   \n ");
    printf("ADC0_SE4a = 4,        // PTE6    \n ");
    printf("ADC0_SE5a = 5,        // PTE18   \n ");
    printf("ADC0_SE8  = 8,        // ADC0_SE8\n ");
    printf("ADC0_SE9  = 9,        // PTE17   \n ");
    printf("ADC0_SE10 =10,        // PTE5    \n ");

    TFTSPI_P8X16Str(2,0,"LQ ADC Test",u16RED,u16BLUE);
    uint16_t batv0 = (uint16_t)(ADC_Get(0)*0.806);
    uint16_t batv1 = (uint16_t)(ADC_Get(1)*0.806);
    uint16_t batv2 = (uint16_t)(ADC_Get(2)*0.806);
    uint16_t batv3 = (uint16_t)(ADC_Get(3)*0.806);
    uint16_t batv4 = (uint16_t)(ADC_Get(4)*0.806);
    uint16_t batv5 = (uint16_t)(ADC_Get(5)*0.806);
    uint16_t batv6 = (uint16_t)(ADC_Get(6)*0.806);
    uint16_t batv7 = (uint16_t)(ADC_Get(7)*0.806);

    
    char txt[16];
    while(1)
    {
        /* 获取 ADC通道值 */
        batv0 = (uint16_t)(ADC_Get(0)*0.806);
        batv1 = (uint16_t)(ADC_Get(1)*0.806);
        batv2 = (uint16_t)(ADC_Get(2)*0.806);
        batv3 = (uint16_t)(ADC_Get(3)*0.806);
        batv4 = (uint16_t)(ADC_Get(4)*0.806);
        batv5 = (uint16_t)(ADC_Get(5)*0.806);
        batv6 = (uint16_t)(ADC_Get(6)*0.806);
        batv7 = (uint16_t)(ADC_Get(7)*0.806); 
          
        
            TFTSPI_P8X16Str(4,0,(char*)"LQ ADC Test Bat",u16RED,u16BLUE); 
            
            printf("\r\n/E16***********************%5d mv\r\n ",batv0);
            sprintf(txt,"E16:%5d mv ",batv0);
            TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);
            
            printf("\r\n/E4***********************%5d mv\r\n ",batv1);
            sprintf(txt,"E4 :%5d mv ",batv1);
            TFTSPI_P8X8Str(1,4,txt,u16RED,u16BLUE);
            
            printf("\r\n/E11***********************%5d mv\r\n ",batv2);
            sprintf(txt,"E11:%5d mv ",batv2);
            TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);
            
            printf("\r\n/E6***********************%5d mv\r\n ",batv3);
            sprintf(txt,"E6 :%5d mv ",batv3);
            TFTSPI_P8X8Str(1,6,txt,u16RED,u16BLUE);
            
            printf("\r\n/E18***********************%5d mv\r\n ",batv4);
            sprintf(txt,"E18:%5d mv ",batv4);
            TFTSPI_P8X8Str(1,7,txt,u16RED,u16BLUE);
            
            printf("\r\n/SE8***********************%5d mv\r\n ",batv5);
            sprintf(txt,"SE8:%5d mv ",batv5);
            TFTSPI_P8X8Str(1,8,txt,u16RED,u16BLUE);
            
            printf("\r\n/E17***********************%5d mv\r\n ",batv6);
            sprintf(txt,"E17:%5d mv ",batv6);
            TFTSPI_P8X8Str(1,9,txt,u16RED,u16BLUE);
            
            printf("\r\n/E5***********************%5d mv\r\n ",batv7);
            sprintf(txt,"E5 :%5d mv ",batv7);
            TFTSPI_P8X8Str(1,10,txt,u16RED,u16BLUE);
             
   
        LED_Reverse(1);    
        delayms(100);
    }
    
}
#endif