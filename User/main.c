#include "stm32f10x.h"

#include "delay.h"
#include "./OLED/OLED.h"
#include "./HC595/hc595.h"
#include "./ds1302/ds1302.h"
#include "./KEY/key.h"

extern struct TIMEData TimeData;
uint8_t temphourT,temphourB,tempminuteT,tempminuteB;

int main(void)
{ 
	ds1302_GPIO_Init();
	ds1302_read_realTime();
	
	HC595_Init();
	
	KEY_Init();
	
//	OLED_Init();
//	OLED_Clear();	
	while(1)
	{
		ds1302_read_realTime();//获取时间	

		temphourT=(TimeData.hour/10);
		temphourB=(TimeData.hour%10);
		tempminuteT=(TimeData.minute/10);
		tempminuteB=(TimeData.minute%10);
		
		HC595_HOUR_OUTPUT(temphourT,temphourB);
		HC595_MINNUTE_OUTPUT(tempminuteT,tempminuteB);
	};
}

