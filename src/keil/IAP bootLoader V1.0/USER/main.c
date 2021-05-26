#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "lcd.h"
#include "stmflash.h"   
#include "iap.h"	 
//ALIENTEK MiniSTM32������ʵ��32
//����IAPʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 

//FLASH����: 0X0800 0000~0x0800 8000,��IAPʹ��,��32K�ֽ�,FLASH APP����256-32=224KB����.
// SRAM����: 0X2000 1000����ʼ,���ڴ��SRAM IAP����,��44K�ֽڿ���,�û��������ж�44K�ռ����ROM��RAM���ķ���
//           �ر�ע�⣬SRAM APP��ROMռ�������ܴ���41K�ֽڣ���Ϊ�����������һ�ν���41K�ֽڣ����ܳ���������ơ�

int main(void)
 { 
	u8 t;
	u8 key;
	u16 oldcount=0;	//�ϵĴ��ڽ�������ֵ
	u16 applenth=0;	//���յ���app���볤��
	u8 clearflag=0; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(256000);	 	//���ڳ�ʼ��Ϊ256000
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD 
 	KEY_Init();				//������ʼ�� 	
 
 	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"Mini STM32");	
	LCD_ShowString(60,70,200,16,16,"IAP TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2014/3/15");  
	LCD_ShowString(60,130,200,16,16,"WK_UP:Copy APP2FLASH"); 
	LCD_ShowString(60,150,200,16,16,"KEY0:Run SRAM APP");
	LCD_ShowString(60,170,200,16,16,"KEY1:Run FLASH APP"); 
	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		t++;
		delay_ms(10);
		if(t==30)
		{
			LED0=!LED0;
			t=0;
			if(clearflag)
			{
				clearflag--;
				if(clearflag==0)LCD_Fill(60,210,240,210+16,WHITE);//�����ʾ
			}
		}	  	 
		key=KEY_Scan(0);
		if(key==WKUP_PRES)			//WK_UP��������
		{
			if(applenth)
			{
				printf("��ʼ���¹̼�...\r\n");	
				LCD_ShowString(60,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
					LCD_ShowString(60,210,200,16,16,"Copy APP Successed!!");
					printf("�̼��������!\r\n");	
				}else 
				{
					LCD_ShowString(60,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("��FLASHӦ�ó���!\r\n");
				}
 			}else 
			{
				printf("û�п��Ը��µĹ̼�!\r\n");
				LCD_ShowString(60,210,200,16,16,"No APP!");
			}
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ									 
		} 
		if(key==KEY1_PRES)
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}else 
			{
				printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
				LCD_ShowString(60,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ	  
		}
		if(key==KEY0_PRES)
		{
			printf("��ʼִ��SRAM�û�����!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//�ж��Ƿ�Ϊ0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM��ַ
			}else 
			{
				printf("��SRAMӦ�ó���,�޷�ִ��!\r\n");
				LCD_ShowString(60,210,200,16,16,"Illegal SRAM APP!");	   
			}									 
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ	 
		}				   
		 
	}   	   
}












