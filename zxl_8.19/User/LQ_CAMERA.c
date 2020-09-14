/*---------------------------------------------------------------
��ƽ    ̨������K60���İ�-���ܳ���
����    д��LQ-005
��E-mail  ��chiusir@163.com
�������汾��V1.0������Դ���룬�����ο�������Ը�
�������¡�2019��04��02��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
������ƽ̨��IAR 8.2
����    �ܡ�OV7725����
��ע�����
----------------------------------------------------------------*/
#include "include.h"

uint8_t Image_Data[IMAGEH][IMAGEW];      //ͼ��ԭʼ���ݴ��
uint8_t Image_Use[LCDH][LCDW];           //ѹ����֮�����ڴ����Ļ��ʾ����
uint8_t Pixle[LCDH][LCDW];               //��ֵ��������OLED��ʾ������
uint8_t  Line_Cont=0;                    //�м���
uint8_t  Field_Over_Flag=0;              //����ʶ


/*---------------------------------------------------------------
����    ����PORTD_Interrupt
����    �ܡ�PORTD�˿ڵ��жϷ����� ��������ͷ�г��ж�
����    ������
���� �� ֵ����
��ע�����ע������Ҫ����жϱ�־λ
----------------------------------------------------------------*/
void PORTD_IRQHandler(void)
{     
    //���ж�PTD13
    int n;    
    n=13;   //���ж�
    if((PORTD_ISFR & (1<<n) ))//���ж� (1<<13)
    {    
        PORTD_ISFR |= (1<<n);   //����жϱ�ʶ
        // �û�����         
        DMATransDataStart(DMA_CH0,(uint32_t)(&Image_Data[Line_Cont][0]));   //����DMA���� 
        if(Line_Cont > IMAGEH)  //�ɼ�����
        { 
            Line_Cont=0; 
            return ;
        } 
        ++Line_Cont;            //�м���
        return ; 
    }
    //���ж�PTD14
    n=14;  //���ж�
    if((PORTD_ISFR & (1<<n)))//(1<<14)
    {
        PORTD_ISFR |= (1<<n);  //����жϱ�ʶ
        // �û����� 
        Line_Cont = 0;         //�м�������
        Field_Over_Flag=1;     //��������ʶ
    } 
}


/*---------------------------------------------------------------
����    ����Test_����ͷ
����    �ܡ��������� ��OLED����ʾͼ��
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Test_Camera(void)
{  
    OLED_Init();
    OLED_CLS();
    OLED_Show_Frame94();      //��ͼ�� LCDW*LCDH ���
    
   /*  ����ͷ��ʼ�� */
    LQMT9V034_Init(50);            //����ͷ��ʼ��
    
    
    /* ����ͷ��ʼ������ */
    
    while(1)
    { 
        if(Field_Over_Flag)       //���һ��ͼ��ɼ�
        { 

	    Get_Use_Image();      //�Ӳɼ�ͼ��������ȡ���Լ���Ҫʹ�õĴ�С
            Get_01_Value(1);      //��ֵ��ͼ������
            
            Draw_Road();          //����OLEDģ����ʾ��̬ͼ��
            
            Start_Deect();
            
            Field_Over_Flag= 0;  
        }    
    }
}


/*---------------------------------------------------------------
����    ����Get_Use_Image
����    �ܡ���ȡ��Ҫʹ�õ�ͼ���С
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
__ramfunc void Get_Use_Image(void)
{
  int i = 0,j = 0,row = 0,line = 0;
  int div_w = IMAGEW/LCDW;
  int div_h = IMAGEH/LCDH;
  for(i = 0; i  < IMAGEH; i+=div_h)  
  {
    for(j = 0;j < IMAGEW; j+=div_w)  
    {        
      Image_Use[row][line] = Image_Data[i][j];         
      line++;
    }      
    line = 0;
    row++;      
  }  
}

/*---------------------------------------------------------------
����    ����Get_01_Value
����    �ܡ���ֵ��
����    ����mode  ��  0��ʹ�ô����ֵ    1��ʹ��ƽ����ֵ
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Get_01_Value(uint8_t mode)
{
  int i = 0,j = 0;
  uint8_t Threshold;
  uint32_t  tv=0;
  char txt[16];
  if(mode)
  {
      //�ۼ�
      for(i = 0; i <LCDH; i++)
      {    
          for(j = 0; j <LCDW; j++)
          {                            
              tv+=Image_Use[i][j];   //�ۼ�  
          } 
      }
      Threshold=tv/LCDH/LCDW;        //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100
      Threshold=Threshold*7/10+10;   //�˴���ֵ���ã����ݻ����Ĺ������趨 
  }
  else
  {
      Threshold = GetOSTU(Image_Use);//�����ֵ
      Threshold = (uint8_t)(Threshold * 0.5) + 70;
  } 
  sprintf(txt,"%03d",Threshold);//ǰ��Ϊ�����õ���ֵ������Ϊƽ��ֵ  
  OLED_P6x8Str(80,1,(u8*)txt);
  
  for(i = 0; i < LCDH; i++)
  {
    for(j = 0; j < LCDW; j++)
    {                                
      if(Image_Use[i][j] >Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����    
        Pixle[i][j] =1;        
      else                                        
        Pixle[i][j] =0;
    }    
  }
}


/*---------------------------------------------------------------
����    ����Draw_Road
����    �ܡ���OLED�ϻ�������ͷ��ͼ��
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Draw_Road(void)
{ 	 
  u8 i = 0, j = 0,temp=0;
  
  //����֡ͷ��־
  for(i=8;i<56;i+=8)//6*8=48�� 
  {
    OLED_Set_Pos(18,i/8+1);//��ʼλ��
    for(j=0;j<LCDW;j++)  //����
    { 
      temp=0; 
      if(Pixle[0+i][j]) temp|=1;
      if(Pixle[1+i][j]) temp|=2;
      if(Pixle[2+i][j]) temp|=4;
      if(Pixle[3+i][j]) temp|=8;
      if(Pixle[4+i][j]) temp|=0x10;
      if(Pixle[5+i][j]) temp|=0x20;
      if(Pixle[6+i][j]) temp|=0x40;
      if(Pixle[7+i][j]) temp|=0x80;
      OLED_WrDat(temp); 	  	  	  	  
    }
  }  
}

/*---------------------------------------------------------------
����    ����Pixle_Filter
����    �ܡ��������
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Pixle_Filter(void)
{  
  int nr; //��
  int nc; //��
  
  for(nr=1; nr<LCDH-1; nr++)
  {  	    
    for(nc=1; nc<LCDW-1; nc=nc+1)
    {
      if((Pixle[nr][nc]==0)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]>2))         
      {
        Pixle[nr][nc]=1;
      }	
      else if((Pixle[nr][nc]==1)&&(Pixle[nr-1][nc]+Pixle[nr+1][nc]+Pixle[nr][nc+1]+Pixle[nr][nc-1]<2))         
      {
        Pixle[nr][nc]=0;
      }	
    }	  
  }  
}



/***************************************************************
* 
* �������ƣ�SendPicture 
* ����˵��������ͼ����λ�� ����ͬ����λ��ע���޸Ķ�Ӧ�����ݽ���Э��
* ����˵���� 
* �������أ�void 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
***************************************************************/ 
void UARTSendPicture(uint8_t  tmImage[IMAGEH][IMAGEW]) 
{ 
  int i = 0, j = 0; 
  UART_PutChar(UART4,0xfe); //����֡ͷ��־ WindowsFormsApplication1.exe
  UART_PutChar(UART4,0xef); //����֡ͷ��־ WindowsFormsApplication1.exe
  for(i=0;i < IMAGEH; i++) 
  { 
    for(j=0;j <IMAGEW ;j++) 
    { 
      if(tmImage[i][j]==0xfe) 
      { 
        tmImage[i][j]=0xff; //��ֹ���ͱ�־λ 
      } 
      UART_PutChar(UART4,tmImage[i][j]); 
    } 
  }
  UART_PutChar(UART4,0xef); //����֡β��־ 
  UART_PutChar(UART4,0xfe); //����֡β��־ 
} 

/*************************************************************** 
* 
* �������ƣ�uint8_t GetOSTU(uint8_t tmImage[IMAGEH][IMAGEW]) 
* ����˵��������ֵ��С 
* ����˵���� 
* �������أ���ֵ��С 
* �޸�ʱ�䣺2018��3��27�� 
* �� ע�� 
�ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
      https://www.cnblogs.com/moon1992/p/5092726.html
      https://www.cnblogs.com/zhonghuasong/p/7250540.html     
Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����
4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
6) i++��ת��4)��ֱ��iΪ256ʱ��������
7�������g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
ȱ��:OSTU�㷨�ڴ������ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
***************************************************************/ 
uint8_t GetOSTU(uint8_t tmImage[LCDH][LCDW]) 
{ 
  int16_t i,j; 
  uint32_t Amount = 0; 
  uint32_t PixelBack = 0; 
  uint32_t PixelIntegralBack = 0; 
  uint32_t PixelIntegral = 0; 
  int32_t PixelIntegralFore = 0; 
  int32_t PixelFore = 0; 
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��; 
  int16_t MinValue, MaxValue; 
  uint8_t Threshold = 0;
  uint8_t HistoGram[256];              //  

  for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ 
  
  for (j = 0; j < LCDH; j++) 
  { 
    for (i = 0; i < LCDW; i++) 
    { 
      HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    } 
  } 
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ
      
  if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ    
  if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ
    
  for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������
    
  PixelIntegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];   //ǰ�����ص���
    PixelFore = Amount - PixelBack;         //�������ص���
    OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
    OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
    PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
    MicroBack = (double)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (double)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //���������ֵ;
} 