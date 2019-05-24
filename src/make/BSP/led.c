#include "led.h"

//LED��IO�ڳ�ʼ������
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PC.14 LED1-->PC.15 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
}
 

//����LED1
void LED1_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_14);//PC14�øߵ�ƽ
}


//Ϩ��LED1
void LED1_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);//PC14�õ͵�ƽ
}


//����LED2
void LED2_ON(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_15);//PC15�øߵ�ƽ
}


//Ϩ��LED2
void LED2_OFF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_15);//PC15�õ͵�ƽ
}
