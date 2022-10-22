#include "ds1302.h"
#include "delay.h"

uint8_t read_time[7];
struct TIMEData TimeData;

/* ds1302 SCLK��CE ���ų�ʼ�� */
void ds1302_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;

	GPIO_InitStructrue.GPIO_Mode		= GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructrue.GPIO_Pin			= DS_SCLK_PIN | DS_CE_PIN;	
	GPIO_Init(GPIOB,&GPIO_InitStructrue);
}

/* ds1302 IO ���������ʼ�� */
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

/* ds1302 IO ���������ʼ�� */
void ds1302_DataIn_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;
	
	GPIO_InitStructrue.GPIO_Mode		= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructrue.GPIO_Pin			= DS_IO_PIN;
	GPIO_Init(GPIOB,&GPIO_InitStructrue);
	
	GPIO_ResetBits(GPIOB,DS_IO_PIN);//����IO���ŷ�ֹ��������
}

/* ds1302 ����һ���ֽ����� */
void Write_Byte(uint8_t data)
{
	ds1302_DataOut_Init();//���� IO ����Ϊ���ģʽ
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

/* �� ds1302 �Ĵ�����ַ����һ���ֽ����� */
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

/* �� ds1302 ��ȡһ���ֽ����� */
uint8_t Read_ds1302_rig(uint8_t addr)
{
	uint8_t i = 0;
	uint8_t return_data = 0x00;
	CE_L;
	SCLK_L;
	CE_H;
	Write_Byte(addr);
	ds1302_DataIn_Init();//���� IO ����Ϊ����ģʽ
	for(i=0;i<8;i++)
	{
		return_data>>=1;
		SCLK_H;
		SCLK_L;
		if(GPIO_ReadInputDataBit(GPIOB,DS_IO_PIN))//��ȡ IO ��������
		{
			return_data = return_data | 0x80;
		}
	}
	CE_L;
	IO_L;
	return return_data;
}

/* ds1302 ���ݼĴ�����ʼ�� */
void ds1302_init(void)
{
	Write_ds1302_rig(0x8e,0x00);//�ر�д����
	Write_ds1302_rig(0x80,0x00);//00��
	Write_ds1302_rig(0x82,0x15);//15��
	Write_ds1302_rig(0x84,0x20);//20Сʱ
	Write_ds1302_rig(0x86,0x17);//30��
	Write_ds1302_rig(0x88,0x04);//9��
	Write_ds1302_rig(0x8a,0x03);//������
	Write_ds1302_rig(0x8c,0x22);//2020��
	Write_ds1302_rig(0x8e,0x80);//�ر�д����
}

/* ��ȡ ds1302 ���ݼĴ�����ֵ */
void ds1302_read_time(void)
{
	read_time[0]=Read_ds1302_rig(0x81);//����
	read_time[1]=Read_ds1302_rig(0x83);//����
	read_time[2]=Read_ds1302_rig(0x85);//��ʱ
	read_time[3]=Read_ds1302_rig(0x87);//����
	read_time[4]=Read_ds1302_rig(0x89);//����
	read_time[5]=Read_ds1302_rig(0x8b);//������
	read_time[6]=Read_ds1302_rig(0x8d);//����
}

/* ��ȡ ds1302 ʱ�� */
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

/* ���� ds1302 ʱ���� */
void REds1302_Write_data(uint8_t hour,uint8_t minuter)
{
	uint8_t temphour,tempminuter;
	
	temphour		=hour/10*16 + hour%10;
	tempminuter =minuter/10*16 + minuter%10;
	
	Write_ds1302_rig(0x8e,0x00);//�ر�д����
	Write_ds1302_rig(0x80,0x00);//00��

	Write_ds1302_rig(0x82,tempminuter);//XX��
	Write_ds1302_rig(0x84,temphour);//XXСʱ
	
	Write_ds1302_rig(0x8c,0x22);//2020��
	Write_ds1302_rig(0x8e,0x80);//�ر�д����
}
	














