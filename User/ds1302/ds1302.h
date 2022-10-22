#ifndef __DS1302_h
#define __DS1302_h

#include "stm32f10x.h" 

struct TIMEData
{
	uint16_t	year;
	uint8_t		month;
	uint8_t		day;
	uint8_t		hour;
	uint8_t		minute;
	uint8_t		second;
	uint8_t		week;
};

extern struct TIMEData TimeData;

//引脚定义
/*****************************************************************/
#define DS_SCLK_PIN						GPIO_Pin_10      //PB10 时钟引脚

#define DS_IO_PIN							GPIO_Pin_1       //PB1  数据引脚

#define DS_CE_PIN							GPIO_Pin_0       //PB0  使能引脚
/*****************************************************************/

#define SCLK_H								GPIO_SetBits	(GPIOB,DS_SCLK_PIN)
#define SCLK_L								GPIO_ResetBits(GPIOB,DS_SCLK_PIN)

#define CE_H									GPIO_SetBits	(GPIOB,DS_CE_PIN)
#define CE_L									GPIO_ResetBits(GPIOB,DS_CE_PIN)

#define IO_H									GPIO_SetBits	(GPIOB,DS_IO_PIN)
#define IO_L									GPIO_ResetBits(GPIOB,DS_IO_PIN)

/*****************************************************************/

void ds1302_GPIO_Init(void);
void ds1302_DataOut_Init(void);
void ds1302_DataIn_Init(void);		
void Write_Byte(uint8_t data);	
void Write_ds1302_rig(uint8_t addr,uint8_t data);
uint8_t Read_ds1302_rig(uint8_t addr)	;
void ds1302_init(void);	
void ds1302_read_time(void);	
void ds1302_read_realTime(void);	
void REds1302_Write_data(uint8_t hour,uint8_t minuter);

#endif /* __DS1302_h */
