#include "stm32f10x.h"
#include "delay.h"

#include "./HC595/hc595.h"
#include "./ds1302/ds1302.h"
#include "./KEY/key.h"

extern struct TIMEData TimeData;
uint8_t temphourT,temphourB,tempminuteT,tempminuteB;

int main(void)
{ 
	/* DS1302时钟芯片初始化 */
	ds1302_GPIO_Init();
	ds1302_read_realTime();
	
	/* hc595初始化 */
	HC595_Init();
	
	/* 按键初始化 */
	KEY_Init();
	
	while(1)
	{
		/* 获取时间 */
		ds1302_read_realTime();
		
		/* 获取595移位数据*/
		temphourT=(TimeData.hour/10);
		temphourB=(TimeData.hour%10);
		tempminuteT=(TimeData.minute/10);
		tempminuteB=(TimeData.minute%10);
		
		/* 辉光管秒显示（双数显示） */
		if((TimeData.second)%2)
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);	
		else
			GPIO_SetBits(GPIOA,GPIO_Pin_6);
		
		/* 辉光管小时显示 */
		HC595_HOUR_OUTPUT(temphourT,temphourB);
		
		/* 辉光管分钟显示 */
		HC595_MINNUTE_OUTPUT(tempminuteT,tempminuteB);
	};
}

