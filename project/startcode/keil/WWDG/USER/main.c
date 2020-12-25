#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "wwdg.h"
//ALIENTEK Mini STM32开发板范例代码6
//窗口看门狗实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

 int main(void)
 {
 
	delay_init();	    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
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


