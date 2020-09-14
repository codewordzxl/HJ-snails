/*-----------------------------------------------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ�UART����
��ע�����
-------------------------------------------------------------------------------------------------------*/
#include "include.h"
#include "LQ_UART.h"



//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//IAR����Ҫ��options -> C/C++compiler -> Preprocessor ��Ӻ궨�� _DLIB_FILE_DESCRIPTOR
#if 1
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����  ʹ�ô���4��Ϊprintf�Ĵ�ӡ��
int fputc(int ch, FILE *f)
{ 	
    while(!(UART_GetStatusFlags(UART4) & kUART_TxDataRegEmptyFlag));
    //��������
    UART_WriteByte(UART4, (uint8_t)ch);
	return ch;
}
#endif 


//-------------------------------------------------------------------------*
//������: UART_PutChar                                                        
//��  ��: ����һ���ֽ�                                                       
//��  ��: base:ģ�����磺UART0 
//         ch: ���͵��ֽ�
//��  ��: ��                                                              
//��  ��: UART_PutChar (UART4, 0x66);                               
//-------------------------------------------------------------------------*
void UART_PutChar(UART_Type *base, char ch)
{
    //�ȴ����ͻ�������
    while(!(UART_GetStatusFlags(base) & kUART_TxDataRegEmptyFlag));
    //��������
    UART_WriteByte(base, (uint8_t)ch);
}


//-------------------------------------------------------------------------*
//������: UART_PutBuff                                                        
//��  ��: ����ָ��len���ֽڳ������� ������ NULL Ҳ�ᷢ�ͣ�                                                       
//��  ��: base:ģ�����磺UART0 
//        buff: ���͵ĵ�ַ��
//        len : ����ָ������
//��  ��: ��                                                              
//��  ��: UART_PutBuff (UART4, "123456789",5);ʵ�ʷ���5���ֽڡ�1����2����3����4����5��                               
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
//������: UART_PutStr                                                        
//��  ��: �����ַ���(�� NULL ֹͣ����)                                                       
//��  ��: base:ģ�����磺UART0 
//        str: ���͵ĵ�ַ��
//��  ��: ��                                                              
//��  ��: UART_PutStr (UART4, "123456789");ʵ�ʷ���9���ֽ�                              
//-------------------------------------------------------------------------*
void UART_PutStr(UART_Type *base, char *str)
{
    while(*str)
    {
        UART_PutChar(base, *str++);
    }
}

//-------------------------------------------------------------------------*
//������: UART_GetChar                                                        
//��  ��: �����ַ���(�� NULL ֹͣ����)                                                       
//��  ��: base:ģ�����磺UART0 
//��  ��: �����յ�������                                                               
//��  ��: UART_GetChar (UART4); ��ȡ�����յ�������                              
//-------------------------------------------------------------------------*
char UART_GetChar(UART_Type *base)
{
     /* �ȴ��������� */
    while (!(UART_GetStatusFlags(base) & kUART_RxDataRegFullFlag));    

    /* ��ȡ���յ���8λ���� */
    return UART_ReadByte(base);
}

/**
  * @brief    ����4��ʼ��
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ������ 115200
  *
  * @example  
  *
  * @date     2019/4/23 ���ڶ�
  */
void UART4_Init(void)
{
	/* ʹ��ʱ�� */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* ��PTE24 ����Ϊ  UART4_TX */
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt8);

    /* ��PTE24 ����Ϊ  UART4_RX */
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
    
	/* ���������ж�  */
	UART_EnableInterrupts(UART4, kUART_RxDataRegFullInterruptEnable);
	
    /* ���ȼ����� ��ռ���ȼ�1  �����ȼ�2   ԽС���ȼ�Խ��  ��ռ���ȼ��ɴ�ϱ���ж� */
    NVIC_SetPriority(UART4_RX_TX_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1,2));
    
    NVIC_EnableIRQ(UART4_RX_TX_IRQn);			          //ʹ��UART4_RX_TX_IRQn���ж�

}

