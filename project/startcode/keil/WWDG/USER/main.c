#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "wwdg.h"
//ALIENTEK Mini STM32�����巶������6
//���ڿ��Ź�ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾

 int main(void)
 {
 
	delay_init();	    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);
 	LED_Init(); 
	LED0=0;
	delay_ms(300);	  
	WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8);   
 	while(1)
	{
		LED0=1;			  	   
	}	 
 }


