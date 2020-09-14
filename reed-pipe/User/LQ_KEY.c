#include "include.h"

/*------------------------------------------------------------------------------------------------------
【函    数】LED_Init
【功    能】初始化核心板和母板上的LED
【参    数】无
【返 回 值】无
【实    例】LED_Init(); //初始化LED
【注意事项】
--------------------------------------------------------------------------------------------------------*/
void KEY_Init(void)
{
    GPIO_Pin_Init(PTB0, GPI_UP, 1);
    GPIO_Pin_Init(PTB1, GPI_UP, 1);
    GPIO_Pin_Init(PTB2, GPI_UP, 1);
    GPIO_Pin_Init(PTB3, GPI_UP, 1);
    GPIO_Pin_Init(PTB4, GPI_UP, 1);
}

void KEY_TEST(void)
{
    LED_Init();
    KEY_Init();
    while(1){
        if(GPIO_Pin_Read(PTB0)==0){
            LED_ON(0);
        }
        else{
            LED_OFF(0);
        }
        if(GPIO_Pin_Read(PTB1)==0){
            LED_ON(1);
        }
        else{
            LED_OFF(1);
        }
        if(GPIO_Pin_Read(PTB2)==0){
            LED_ON(2);
        }
        else{
            LED_OFF(2);
        }
        if(GPIO_Pin_Read(PTB3)==0){
            LED_ON(3);
        }
        else{
            LED_OFF(3);
        }
    }
}
