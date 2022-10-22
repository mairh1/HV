#include "./KEY/key.h"

/*
	����KEY���ų�ʼ��	
	��������
	���壺
		KEY4(sw5)-->������5s��д������   ���̰����˳�����
		KEY3(sw4)-->���ӵ�����+1��
		KEY2(sw2)-->Сʱ������+1��
		KEY1(sw1)-->ʱ��������жϣ�
*/
void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructrue;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	GPIO_InitStructrue.GPIO_Mode		= GPIO_Mode_IPU;
	GPIO_InitStructrue.GPIO_Pin			= KEY1|KEY2|KEY3;	
	GPIO_Init(GPIOA,&GPIO_InitStructrue);	

	GPIO_InitStructrue.GPIO_Pin			= KEY4;	
	GPIO_Init(GPIOB,&GPIO_InitStructrue);	

	
	EXTI_ClearITPendingBit(EXTI_Line4);
	EXTI_InitStructure.EXTI_Line    = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
}


