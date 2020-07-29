/**
*
*  @Fuction: main.c
*  @Author: 
*  @Descripe :
*
**/

#include "stm32f10x.h"
#include "uart.h"
#include "delay.h"
#include "usb_manager.h"


void InitDriver()
{
	delay_init();
	 uart_init(115200);
	printf("serial init success \n");
    usb_use_init();
	printf("system init success \n");

}

void main()
{
  InitDriver();
  while(1)
	{
		delay_ms(1000);	
		printf("start usb send \n");
		usb_print("test QITAS \n");
	}
}