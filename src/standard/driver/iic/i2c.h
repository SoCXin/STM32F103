#ifndef __I2C_H_
#define __I2C_H_
#include "stm32f10x.h"
#include "delay.h"


//IO方向设置
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO操作函数	 
#define SCL    PBout(6) //SCL
#define SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 
#define flash_en()			 PBout(2) = 0
#define flash_dis()     PBout(2) = 1


void IIC_Init(void);      //IIC端口初始化
void save_gpio_Init(void);
void Start_I2c(void);		  //启动总线
void Stop_I2c(void);		  //停止总线
void SendByte(u8 c);	  //字节数据发送函数
u8  RcvByte(u8 ack);	  //字节数据接收函数 	    
u8 I2c_wait_ack(void);	  //等待应答信号到来




#endif
