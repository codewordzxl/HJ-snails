#include "include.h"

void Start_Deect()
{
    int i,j;
    int Black_White=0;
    int White_Black=0;
    int count=0;
    if(Pixle[8][47]==1&&Pixle[9][47]==1&&Pixle[10][47]==1&&Pixle[11][47]==1&&Pixle[12][47]==1&&Pixle[13][47]==1
       &&Pixle[8][45]==1&&Pixle[9][45]==1&&Pixle[10][45]==1&&Pixle[11][45]==1&&Pixle[12][45]==1&&Pixle[13][45]==1
         &&Pixle[8][49]==1&&Pixle[9][49]==1&&Pixle[10][49]==1&&Pixle[11][49]==1&&Pixle[12][49]==1&&Pixle[13][49]==1)
    {
        for(i=15;i<20;i++)//ÐÐ
        {
          for(j=0;j<90;j++)
          {
            if(Pixle[i][j]==0&&Pixle[i][j+1]==1)
            {
              Black_White++;
            }
            if(Pixle[i][j]==1&&Pixle[i][j+1]==0)
            {
              White_Black++;
            }
          }
          
          if(Black_White>5&&Black_White<11&&White_Black>5&&White_Black<11)
          {
            count++;
          }
          Black_White=0;
          White_Black=0;
        }
    }
    
    if(count>2)
    {
      Controlvar.Stop_flag=1;
      GPIO_Pin_Write(PTA17,0);
    }
//    else
//    {
//      GPIO_Pin_Write(PTA17,1);
//    }
}