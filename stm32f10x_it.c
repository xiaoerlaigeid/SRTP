/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "gui.h"
#include "lcd_driver.h"

extern int last_height;
extern int height;
extern int data_flag;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
int time_all;
void USART1_IRQHandler(void)
{
	static int i = 0;
	static u8 k;
	static u8 data[4];
	float floatVariable = 0;
	
	unsigned char * pf;   
	unsigned char * px;
	
	float * ptr_var = &floatVariable;

	px = data;
	pf = (char *)ptr_var;
	
	USART_ClearFlag(USART1,USART_FLAG_TC);
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)//检查指定的USART中断发生与否
	{
		k = USART_ReceiveData(USART1);
		data[i] = k;
		i ++;
		if(i == 4)
		{
			for(i = 0; i < 4; i ++)
			{
				*(pf + i) = *(px + i);
			}
			data_flag = 1;
			height = (int)(floatVariable);
			
			//变化较大才启动电机
			if(height - last_height > 10 || last_height - height > 10)
			{
				time_all = 0;
				
			//	if(height > 140)	//向上转
				//	GPIO_ResetBits(GPIOC, GPIO_Pin_2);
				if(height < 140)
					GPIO_SetBits(GPIOC, GPIO_Pin_2);
				
				TIM_Cmd(TIM3, ENABLE);
				last_height = height;
			}
			i = 0;
		}
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == Bit_RESET);	
	}
}


void TIM3_IRQHandler()	 //定时器3中断函数
{
	static int i = 0;
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	if(i == 0)
	{
		i = 1;
		time_all ++;
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	}
	else
	{
		i = 0;
		time_all ++;
		GPIO_SetBits(GPIOC, GPIO_Pin_1);
	}
	
	if(time_all >= 3000)
	{
		time_all = 0;
		TIM_Cmd(TIM3, DISABLE);
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
