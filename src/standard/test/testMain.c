/**
*
*  @File Name: testMain.c
*  @Author: xiaolai    @Date: 2016-12-11
*  @Descripe :
*
**/

#include "stm32f10x.h"
#include "uart.h"
#include "delay.h"

#define ENBALE   1
#define DISABLE  0

#define TEST_ABILITY  DISABLE     //����ʹ�ܣ�Ҫ��main.c������mainע��
#define TEST_USART1   ENABLE      //����1����


void InitDriverTest()
{
	delay_init();
	
#if TEST_USART1
     uart_init(115200);
#endif
	 
}

#if TEST_ABILITY
void main()
{
    InitDriverTest();
    unsigned int temp =10;
	
    while(temp--)
	{
		
#if TEST_USART1
		delay_ms(1000);
	    printf("test uart 1 \r\n");
#endif
		
	}
}
#endif
