#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "./OLED/OLED.h"

#define KEY1 GPIO_Pin_4
#define KEY2 GPIO_Pin_5
#define KEY3 GPIO_Pin_7
#define KEY4 GPIO_Pin_11

void KEY_Init(void);
#endif /* __DELAY_H */
