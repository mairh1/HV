#ifndef __HC595_H
#define __HC595_H

#include "stm32f10x.h"
//小时
#define SHCP1 	GPIO_Pin_14//时钟
#define	STCP1		GPIO_Pin_13//并出												 
#define DIN1		GPIO_Pin_12//数据
//分钟
#define STCP2		GPIO_Pin_5//并出
#define SHCP2		GPIO_Pin_4//	时钟
#define DIN2		GPIO_Pin_3//数据


void HC595_Init(void);
void HC595_HOUR_OUTPUT(uint8_t Data1,uint8_t Data2);
void HC595_MINNUTE_OUTPUT(uint8_t Data1,uint8_t Data2);


#endif 
