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
#include "./HC595/hc595.h"
#include "./KEY/key.h"
#include "./ds1302/ds1302.h"
#include "delay.h"
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


/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void RTC_IRQHandler(void)
{

}

/*
	按键修改时间	
	参数：无
*/
void EXTI4_IRQHandler(void)
{
	uint8_t IT_minute,IT_hour;
	
	IT_hour = TimeData.hour;    //获取当前时间（小时）
	IT_minute = TimeData.minute;//获取当前时间（分钟）
		
	if(EXTI_GetITStatus(EXTI_Line4)!= RESET)//有中断申请
	{	
		while(1)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_6);//设置":"为常亮
			
			if(GPIO_ReadInputDataBit(GPIOA,KEY2)==RESET)//修改小时
			{
				uint8_t IT_hourT,IT_hourB;
				IT_hour++;
				
				if(IT_hour>23)
					IT_hour=0;
				
				/* 获取595移位数据 */
				IT_hourT=(IT_hour/10);
				IT_hourB=(IT_hour%10);
				
				/* 辉光管更新显示 */
				HC595_HOUR_OUTPUT(IT_hourT,IT_hourB);
				
				Delay_ms(400);
			}		
			
			if(GPIO_ReadInputDataBit(GPIOA,KEY3)==RESET)//修改分钟
			{
				uint8_t IT_minuteT,IT_minuteB;
				IT_minute++;
				
				if(IT_minute>59)
					IT_minute=0;
				
				/* 获取595移位数据 */
				IT_minuteT=(IT_minute/10);
				IT_minuteB=(IT_minute%10);
				
				/* 辉光管更新显示 */
				HC595_MINNUTE_OUTPUT(IT_minuteT,IT_minuteB);

				Delay_ms(400);
			}	
			
			if(GPIO_ReadInputDataBit(GPIOB,KEY4)==RESET)//保存退出
			{
				REds1302_Write_data(IT_hour,IT_minute);//DS1302写入新时间时间
				EXTI_ClearITPendingBit(EXTI_Line4);//清除LINE4上的中断标志位
				break;
			}
		}							
	}
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
