/*-----------------------------------------------------------------------------------------------------
【平    台】龙邱K60核心板-智能车板
【编    写】LQ-005
【E-mail  】chiusir@163.com
【软件版本】V1.0，龙邱开源代码，仅供参考，后果自负
【最后更新】2019年04月02日
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【编译平台】IAR 8.2
【功    能】UART例子
【注意事项】
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_UART.h"



//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//IAR，需要在options -> C/C++compiler -> Preprocessor 添加宏定义 _DLIB_FILE_DESCRIPTOR
#if 1
//#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数  使用串口4作为printf的打印口
int fputc(int ch, FILE *f)
{ 	
    while(!(UART_GetStatusFlags(UART4) & kUART_TxDataRegEmptyFlag));
    //发送数据
    UART_WriteByte(UART4, (uint8_t)ch);
	return ch;
}
#endif 


//-------------------------------------------------------------------------*
//函数名: UART_PutChar                                                        
//功  能: 发送一个字节                                                       
//参  数: base:模块名如：UART0 
//         ch: 发送的字节
//返  回: 无                                                              
//简  例: UART_PutChar (UART4, 0x66);                               
//-------------------------------------------------------------------------*
void UART_PutChar(UART_Type *base, char ch)
{
    //等待发送缓冲区空
    while(!(UART_GetStatusFlags(base) & kUART_TxDataRegEmptyFlag));
    //发送数据
    UART_WriteByte(base, (uint8_t)ch);
}


//-------------------------------------------------------------------------*
//函数名: UART_PutBuff                                                        
//功  能: 发送指定len个字节长度数组 （包括 NULL 也会发送）                                                       
//参  数: base:模块名如：UART0 
//        buff: 发送的地址、
//        len : 发送指定长度
//返  回: 无                                                              
//简  例: UART_PutBuff (UART4, "123456789",5);实际发送5个字节‘1’‘2’‘3’‘4’‘5’                               
//-------------------------------------------------------------------------*
void UART_PutBuff(UART_Type *base, uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        UART_PutChar(base, *buff);
        buff++;
    }
}



//-------------------------------------------------------------------------*
//函数名: UART_PutStr                                                        
//功  能: 发送字符串(遇 NULL 停止发送)                                                       
//参  数: base:模块名如：UART0 
//        str: 发送的地址、
//返  回: 无                                                              
//简  例: UART_PutStr (UART4, "123456789");实际发送9个字节                              
//-------------------------------------------------------------------------*
void UART_PutStr(UART_Type *base, char *str)
{
    while(*str)
    {
        UART_PutChar(base, *str++);
    }
}

//-------------------------------------------------------------------------*
//函数名: UART_GetChar                                                        
//功  能: 发送字符串(遇 NULL 停止发送)                                                       
//参  数: base:模块名如：UART0 
//返  回: 串口收到的数据                                                               
//简  例: UART_GetChar (UART4); 获取串口收到的数据                              
//-------------------------------------------------------------------------*
char UART_GetChar(UART_Type *base)
{
     /* 等待接收满了 */
    while (!(UART_GetStatusFlags(base) & kUART_RxDataRegFullFlag));    

    /* 获取接收到的8位数据 */
    return UART_ReadByte(base);
}

/**
  * @brief    串口4初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     波特率 115200
  *
  * @example  
  *
  * @date     2019/4/23 星期二
  */
void UART4_Init(void)
{
	/* 使能时钟 */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* 将PTE24 设置为  UART4_TX */
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt8);

    /* 将PTE24 设置为  UART4_RX */
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt8);
	
	
	uart_config_t config;
	/*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = 115200;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(UART4, &config, CLOCK_GetFreq(kCLOCK_FastPeriphClk));
    
	/* 开启接收中断  */
	UART_EnableInterrupts(UART4, kUART_RxDataRegFullInterruptEnable);
	
    /* 优先级配置 抢占优先级1  子优先级2   越小优先级越高  抢占优先级可打断别的中断 */
    NVIC_SetPriority(UART4_RX_TX_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
    NVIC_EnableIRQ(UART4_RX_TX_IRQn);			          //使能UART4_RX_TX_IRQn的中断

}

