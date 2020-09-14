/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ�
��ע�����
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_ADC.h"
/*------------------------------------------------------------------------------------------------------
����    ����ADC_Get
����    �ܡ���ȡ����ĸ���ϵ�8·ADCֵ
����    ����num �� 0 ~ 7
���� �� ֵ��ADC��ȡ��ֵ
��ʵ    ����ADC_Get(0);           //��ȡAD0�ӿڵ�ֵ
��ע�����ADC0_DP1  = 1,        // PTE16         
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
����    ����Test_ADC
����    �ܡ�����ĸ���ϵ�10·ADC�ӿ�
����    ������
���� �� ֵ����
��ʵ    ����Test_ADC(); //��ӡ����ʾ��ѹ
��ע�����
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
    printf("ADC ��������");
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
        /* ��ȡ ADCͨ��ֵ */
        batv0 = (uint16_t)(ADC_Get(0)*0.806);
        batv1 = (uint16_t)(ADC_Get(1)*0.806);
        batv2 = (uint16_t)(ADC_Get(2)*0.806);
        batv3 = (uint16_t)(ADC_Get(3)*0.806);
        batv4 = (uint16_t)(ADC_Get(4)*0.806);
        batv5 = (uint16_t)(ADC_Get(5)*0.806);
        batv6 = (uint16_t)(ADC_Get(6)*0.806);
        batv7 = (uint16_t)(ADC_Get(7)*0.806); 
        switch(KEY_Read(1))     //�������·�ҳ
        {
          case 1:
            OLED_CLS();          //LCD����
            key = 0;            //K0 ���� ��һҳ
            break;           
          case 2: 
            OLED_CLS();          //LCD����
            key = 1;            //K1 ���� �ڶ�ҳ
            break;
          case 3: 
            OLED_CLS();          //LCD����
            key = 2;            //K1 ���� �ڶ�ҳ
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
    
    TFTSPI_Init(1);                 //LCD��ʼ��  0:����  1������
    TFTSPI_CLS(u16BLUE);
    printf("ADC ��������");
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
        /* ��ȡ ADCͨ��ֵ */
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