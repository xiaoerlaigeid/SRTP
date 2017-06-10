#include "public.h"
#include "usart.h"
#include "systick.h"
#include "lcd_driver.h"
#include "gui.h"
#include "pwm.h"
#include "time.h"
#include "key.h"
void key_pros(void);

#define K_UP GPIO_Pin_0 //PA0
#define K_DOWN GPIO_Pin_3 //PE3
#define K_LEFT GPIO_Pin_2 //PE2
#define K_RIGHT GPIO_Pin_4 //PE4

#define k_up GPIO_ReadInputDataBit(GPIOA,K_UP)		  //��ȡ������״̬
#define k_down GPIO_ReadInputDataBit(GPIOE,K_DOWN)
#define k_left GPIO_ReadInputDataBit(GPIOE,K_LEFT)
#define k_right GPIO_ReadInputDataBit(GPIOE,K_RIGHT)

//PC1 -- ��������ź�
//PC2 -- ������ƣ���0����
//PC3 -- ���ʹ�ܣ���0��ס

int last_height = 0;
int height = 0;
int data_flag = 0;
int run_flag = 0;

float Out_Filter(float turn_out);
int main()
{
	pwm_init();
	TFT_Init();	  //TFT������ʼ��
	usart_init();
	TFT_ClearScreen(BLACK);
	GUI_Show12ASCII(0, 5, " hello world!  ", RED, YELLOW);
	GUI_Show12ASCII(110, 5, "waiting for data command..", RED, YELLOW);
	
	TFT_ClearPartScreen(YELLOW, 0, 21, TFT_XMAX - 2, 20);
	GUI_Border(RED, 0, 41, TFT_XMAX, TFT_YMAX - 41, 10);
	key_init();
	time_init();

	while(1)
	{
		if(data_flag == 1)
		{
			GUI_Show12Num(100, 100, height, WHITE, BLACK);
			data_flag = 0;
		}
		key_pros();
	}
}

void run_at()
{
	if(run_flag)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
		delay_us(312);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
		delay_us(312);
	}
}


float Out_Filter(float turn_out)
{
      float Turn_Out_Filtered; 
      
      static float Pre1_Error[4];     

      Pre1_Error[3]=Pre1_Error[2];
      
      Pre1_Error[2]=Pre1_Error[1];
      
      Pre1_Error[1]=Pre1_Error[0];
      
      Pre1_Error[0]=turn_out;

      Turn_Out_Filtered = Pre1_Error[0] * 0.4 + Pre1_Error[1] * 0.3 + Pre1_Error[2] * 0.2 + Pre1_Error[3] * 0.1;
      
      return Turn_Out_Filtered;      
}


void key_pros()
{
	if(k_up == 1)	  //�жϰ���k_up�Ƿ���
	{
		delay_ms(10); //��������
		if(k_up == 1)	 //�ٴ��жϰ���k_up�Ƿ���
		{
			//����������,���������������Ӧ
			TIM_Cmd(TIM3,DISABLE);
		}
		while(k_up); //�ȴ������ɿ�
	}
	if(k_down==0)
	{
		delay_ms(10);
		if(k_down==0)
		{
			TIM_Cmd(TIM3,ENABLE);
		}
		while(!k_down);
	}
	if(k_left==0)
	{
		delay_ms(10);
		if(k_left==0)
		{
			//����
			GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		}
		while(!k_left);
	}
	if(k_right==0)
	{
		delay_ms(10);
		if(k_right==0)
		{
			//����
			GPIO_SetBits(GPIOC, GPIO_Pin_2);
		}
		while(!k_right);
	}	
}
