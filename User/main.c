#include "stm32f10x.h"
#include "delay.h"

#include "./HC595/hc595.h"
#include "./ds1302/ds1302.h"
#include "./KEY/key.h"

extern struct TIMEData TimeData;
uint8_t temphourT,temphourB,tempminuteT,tempminuteB;

int main(void)
{ 
	/* DS1302ʱ��оƬ��ʼ�� */
	ds1302_GPIO_Init();
	ds1302_read_realTime();
	
	/* hc595��ʼ�� */
	HC595_Init();
	
	/* ������ʼ�� */
	KEY_Init();
	
	while(1)
	{
		/* ��ȡʱ�� */
		ds1302_read_realTime();
		
		/* ��ȡ595��λ����*/
		temphourT=(TimeData.hour/10);
		temphourB=(TimeData.hour%10);
		tempminuteT=(TimeData.minute/10);
		tempminuteB=(TimeData.minute%10);
		
		/* �Թ������ʾ��˫����ʾ�� */
		if((TimeData.second)%2)
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);	
		else
			GPIO_SetBits(GPIOA,GPIO_Pin_6);
		
		/* �Թ��Сʱ��ʾ */
		HC595_HOUR_OUTPUT(temphourT,temphourB);
		
		/* �Թ�ܷ�����ʾ */
		HC595_MINNUTE_OUTPUT(tempminuteT,tempminuteB);
	};
}

