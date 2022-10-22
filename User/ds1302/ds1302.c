#include "ds1302.h"
#include "delay.h"

uint8_t read_time[7];
struct TIMEData TimeData;

/* ds1302 SCLK、CE 引脚初始化 */
void ds1302_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;

	GPIO_InitStructrue.GPIO_Mode		= GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructrue.GPIO_Pin			= DS_SCLK_PIN | DS_CE_PIN;	
	GPIO_Init(GPIOB,&GPIO_InitStructrue);
}

/* ds1302 IO 引脚输出初始化 */
void ds1302_DataOut_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;
	
	GPIO_InitStructrue.GPIO_Mode		= GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Pin			= DS_IO_PIN;
	GPIO_InitStructrue.GPIO_Speed		=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructrue);
	
	GPIO_ResetBits(GPIOB,DS_IO_PIN);
}

/* ds1302 IO 引脚输入初始化 */
void ds1302_DataIn_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;
	
	GPIO_InitStructrue.GPIO_Mode		= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructrue.GPIO_Pin			= DS_IO_PIN;
	GPIO_Init(GPIOB,&GPIO_InitStructrue);
	
	GPIO_ResetBits(GPIOB,DS_IO_PIN);//拉低IO引脚防止数据输入
}

/* ds1302 发送一个字节数据 */
void Write_Byte(uint8_t data)
{
	ds1302_DataOut_Init();//配置 IO 引脚为输出模式
	uint8_t i;
	SCLK_L;
	for(i=0;i<8;i++)
	{
		SCLK_L;
		if(data & 0x01)IO_H;
		else IO_L;
		SCLK_H;
		data>>=1;
	}
}

/* 往 ds1302 寄存器地址发送一个字节数据 */
void Write_ds1302_rig(uint8_t addr,uint8_t data)
{
	CE_L;
	SCLK_L;
	CE_H;
	Write_Byte(addr);
	Write_Byte(data);
	CE_L;
	SCLK_L;
}

/* 向 ds1302 读取一个字节数据 */
uint8_t Read_ds1302_rig(uint8_t addr)
{
	uint8_t i = 0;
	uint8_t return_data = 0x00;
	CE_L;
	SCLK_L;
	CE_H;
	Write_Byte(addr);
	ds1302_DataIn_Init();//配置 IO 引脚为输入模式
	for(i=0;i<8;i++)
	{
		return_data>>=1;
		SCLK_H;
		SCLK_L;
		if(GPIO_ReadInputDataBit(GPIOB,DS_IO_PIN))//读取 IO 引脚数据
		{
			return_data = return_data | 0x80;
		}
	}
	CE_L;
	IO_L;
	return return_data;
}

/* ds1302 数据寄存器初始化 */
void ds1302_init(void)
{
	Write_ds1302_rig(0x8e,0x00);//关闭写保护
	Write_ds1302_rig(0x80,0x00);//00秒
	Write_ds1302_rig(0x82,0x15);//15分
	Write_ds1302_rig(0x84,0x20);//20小时
	Write_ds1302_rig(0x86,0x17);//30日
	Write_ds1302_rig(0x88,0x04);//9月
	Write_ds1302_rig(0x8a,0x03);//星期天
	Write_ds1302_rig(0x8c,0x22);//2020年
	Write_ds1302_rig(0x8e,0x80);//关闭写保护
}

/* 读取 ds1302 数据寄存器的值 */
void ds1302_read_time(void)
{
	read_time[0]=Read_ds1302_rig(0x81);//读秒
	read_time[1]=Read_ds1302_rig(0x83);//读分
	read_time[2]=Read_ds1302_rig(0x85);//读时
	read_time[3]=Read_ds1302_rig(0x87);//读日
	read_time[4]=Read_ds1302_rig(0x89);//读月
	read_time[5]=Read_ds1302_rig(0x8b);//读星期
	read_time[6]=Read_ds1302_rig(0x8d);//读年
}

/* 获取 ds1302 时间 */
void ds1302_read_realTime(void)
{
	ds1302_read_time(); 
	TimeData.second		= (read_time[0]>>4)*10+(read_time[0]&0x0f);
	TimeData.minute		= ((read_time[1]>>4)&(0x07))*10+(read_time[1]&0x0f);
	TimeData.hour			= (read_time[2]>>4)*10+(read_time[2]&0x0f);
	TimeData.day			= (read_time[3]>>4)*10+(read_time[3]&0x0f);
	TimeData.month		= (read_time[4]>>4)*10+(read_time[4]&0x0f);
	TimeData.week			= read_time[5];
	TimeData.year			= (read_time[6]>>4)*10+(read_time[6]&0x0f);
	
}

/* 重置 ds1302 时分秒 */
void REds1302_Write_data(uint8_t hour,uint8_t minuter)
{
	uint8_t temphour,tempminuter;
	
	temphour		=hour/10*16 + hour%10;
	tempminuter =minuter/10*16 + minuter%10;
	
	Write_ds1302_rig(0x8e,0x00);//关闭写保护
	Write_ds1302_rig(0x80,0x00);//00秒

	Write_ds1302_rig(0x82,tempminuter);//XX分
	Write_ds1302_rig(0x84,temphour);//XX小时
	
	Write_ds1302_rig(0x8c,0x22);//2020年
	Write_ds1302_rig(0x8e,0x80);//关闭写保护
}
	














