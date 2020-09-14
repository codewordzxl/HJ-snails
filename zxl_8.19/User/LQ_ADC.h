#ifndef __LQ_ADC_H
#define __LQ_ADC_H


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
uint16_t ADC_Get(uint8_t num);




/*------------------------------------------------------------------------------------------------------
【函    数】Test_ADC
【功    能】测试母板上的10路ADC接口
【参    数】无
【返 回 值】无
【实    例】Test_ADC(); //打印并显示电压
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void Test_ADC(void);
#endif