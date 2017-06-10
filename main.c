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

#define k_up GPIO_ReadInputDataBit(GPIOA,K_UP)		  //获取按键的状态
#define k_down GPIO_ReadInputDataBit(GPIOE,K_DOWN)
#define k_left GPIO_ReadInputDataBit(GPIOE,K_LEFT)
#define k_right GPIO_ReadInputDataBit(GPIOE,K_RIGHT)

//PC1 -- 电机脉冲信号
//PC2 -- 方向控制，给0向上
//PC3 -- 电机使能，给0锁住

int last_height = 0;
int height = 0;
int data_flag = 0;
int run_flag = 0;

float Out_Filter(float turn_out);
int main()
{
	pwm_init();
	TFT_Init();	  //TFT彩屏初始化
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
	if(k_up == 1)	  //判断按键k_up是否按下
	{
		delay_ms(10); //消抖处理
		if(k_up == 1)	 //再次判断按键k_up是否按下
		{
			//按下则锁死,锁死会产生不良反应
			TIM_Cmd(TIM3,DISABLE);
		}
		while(k_up); //等待按键松开
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
			//按下
			GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		}
		while(!k_left);
	}
	if(k_right==0)
	{
		delay_ms(10);
		if(k_right==0)
		{
			//向上
			GPIO_SetBits(GPIOC, GPIO_Pin_2);
		}
		while(!k_right);
	}	
}
