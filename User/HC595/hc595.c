#include "./HC595/hc595.h"
#include "delay.h"
//#include "./OLED/OLED.h"

/*
	74HC595引脚初始化	
	参数：无
*/
void HC595_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Mode		= GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStructrue.GPIO_Pin			= SHCP1|STCP1|DIN1|SHCP2|STCP2|DIN2;	
	GPIO_Init(GPIOB,&GPIO_InitStructrue);	
	
	GPIO_InitStructrue.GPIO_Pin			= GPIO_Pin_6;	
	GPIO_Init(GPIOA,&GPIO_InitStructrue);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);																		
}
 
/*
	74HC595数据输入	
	参数：74HC595 DIN引脚位 SHCP引脚位
*/
static void HC595_Send_Byte(uint16_t din,uint16_t shcp,uint8_t byte)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++)
	{
		if(byte & 0x80)//高位在前
			GPIO_WriteBit(GPIOB, din, Bit_SET);
		else
			GPIO_WriteBit(GPIOB, din, Bit_RESET);
		
		GPIO_WriteBit(GPIOB, shcp, Bit_RESET);
		Delay_us(10);
		GPIO_WriteBit(GPIOB, shcp, Bit_SET);
		Delay_us(10);
		byte <<= 1;
	}
}

/*
	74HC595并出锁存器数据
	参数：74HC595 STCP引脚位
*/
static void HC595_STCP_OUTPUT(uint16_t stcp)
{
	GPIO_WriteBit(GPIOB, stcp, Bit_RESET);
	Delay_us(10);
	GPIO_WriteBit(GPIOB, stcp, Bit_SET);
	Delay_us(10);
}

/*
	74HC595控制小时输出	
	参数：Data1=(0~9) Data2=(0~9)	
*/
void HC595_HOUR_OUTPUT(uint8_t Data1,uint8_t Data2)
{
	uint32_t temp=0;
	uint8_t temp1=0,temp2=0,temp3=0;
	
	temp=1<<(Data1+10);  //Data1=(0~9)
	temp|=1<<Data2; 	   //Data2=(0~9)
	
	temp1=temp;
	temp2=(temp>>8);
	temp3=(temp>>16);
	
	HC595_Send_Byte(DIN1,SHCP1,temp3);
	HC595_Send_Byte(DIN1,SHCP1,temp2);
	HC595_Send_Byte(DIN1,SHCP1,temp1);

	/*595并出3字节*/
	HC595_STCP_OUTPUT(STCP1);
}

/*
	74HC595控制分钟输出	
	参数：Data1=(0~9) Data2=(0~9)	
*/
void HC595_MINNUTE_OUTPUT(uint8_t Data1,uint8_t Data2)
{
	uint32_t temp=0;
	uint8_t temp1=0,temp2=0,temp3=0;
	
	temp=1<<(Data1+10);  //Data1=(0~9)
	temp|=1<<Data2; 		 //Data2=(0~9)
	
	temp1=temp;
	temp2=(temp>>8);
	temp3=(temp>>16);
	
	HC595_Send_Byte(DIN2,SHCP2,temp3);
	HC595_Send_Byte(DIN2,SHCP2,temp2);
	HC595_Send_Byte(DIN2,SHCP2,temp1);

	/*595并出3字节*/
	HC595_STCP_OUTPUT(STCP2);
}
