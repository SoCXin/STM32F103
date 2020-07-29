#ifndef __I2C_H_
#define __I2C_H_
#include "stm32f10x.h"
#include "delay.h"


//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO��������	 
#define SCL    PBout(6) //SCL
#define SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 
#define flash_en()			 PBout(2) = 0
#define flash_dis()     PBout(2) = 1


void IIC_Init(void);      //IIC�˿ڳ�ʼ��
void save_gpio_Init(void);
void Start_I2c(void);		  //��������
void Stop_I2c(void);		  //ֹͣ����
void SendByte(u8 c);	  //�ֽ����ݷ��ͺ���
u8  RcvByte(u8 ack);	  //�ֽ����ݽ��պ��� 	    
u8 I2c_wait_ack(void);	  //�ȴ�Ӧ���źŵ���




#endif
