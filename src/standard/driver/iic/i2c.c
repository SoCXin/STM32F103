																																		   
 /*************************�˲���ΪI2C���ߵ���������*************************************/


#include "i2c.h"





void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}

void save_gpio_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;					 //����GPIO�ṹ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	//PB2
	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;			 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	 GPIO_Init(GPIOB, &GPIO_InitStructure);			
	 flash_en();
//	 flash_dis();	
}



//�����ߺ���               
//Start_I2c();  
//����:     ����I2C����,������I2C��ʼ����.  
void Start_I2c(void)
{
  SDA_OUT();     //sda�����
  SDA=1;         //������ʼ�����������ź�
  SCL=1;
  delay_us(4); 
  SDA=0;         //������ʼ�ź�
  delay_us(4);      
  SCL=0;       //ǯסI2C���ߣ�׼�����ͻ�������� 
}


//�������ߺ���               
//Stop_I2c();  
//����:     ����I2C����,������I2C��������.  
void Stop_I2c(void)
{
  SDA_OUT();     //sda�����
  SCL=0;
  SDA=0;      //���ͽ��������������ź�
  delay_us(4);
  SCL=1;      //������������ʱ�����4��s
  SDA=1;      //����I2C���߽����ź�
  delay_us(4);
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 I2c_wait_ack(void)
{
	u8 Time=0; 
    SDA_IN();			  //����SDAΪ����
    SDA=1;                //׼������Ӧ��λ
    delay_us(1);  
    SCL=1;
    delay_us(1); 			 
	while(READ_SDA)			   
	{
		Time++;
		if(Time>250)
		{
			Stop_I2c();
			return 1;	 //��Ӧ�𷵻�1
		}
	}
	SCL=0;//ʱ�����0 
	
	return 0;  			 //��Ӧ�𷵻�0
} 



//�ֽ����ݷ��ͺ���               
//SendByte(u8 c);
//����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������
void  SendByte(u8  c)
{
 u8  BitCnt;

 SDA_OUT();     //sda�����
 SCL=0;			//����ʱ�ӿ�ʼ���ݴ���
 								  //���� һ��Ҫ�������� ����SCL����0״̬ ���ܽ���д��
 for(BitCnt=0;BitCnt<8;BitCnt++)  //Ҫ���͵����ݳ���Ϊ8λ
    {
     if((c<<BitCnt)&0x80)SDA=1;   //�жϷ���λ  �������ɸ�λ��ʼ����
       else  SDA=0;                
     delay_us(2); 
     SCL=1;               //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
     delay_us(2);        
     SCL=0;
     delay_us(2); 	   	  
    }
    
}


 //����ACKӦ��
void I2C_Ack(void)
{
	SCL=0;
	SDA_OUT();     //sda�����
	SDA=0;
	delay_us(2); 		  
	SCL=1;
	delay_us(2);
	SCL=0;
}
//������ACKӦ��		    
void I2C_NAck(void)
{
  	SCL=0;
	SDA_OUT();     //sda�����
	SDA=1;
	delay_us(2); 
	SCL=1;
	delay_us(2);
	SCL=0;
}


//�ֽ����ݽ��պ���               
//RcvByte();
//����:  �������մ���������������,���ж����ߴ���(��Ӧ���ź�)
//ack  1 ����Ӧ��  0 ������Ӧ��
u8 RcvByte(u8 ack)
{
  u8 retc=0,i;
  SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++)
   {         
        SCL=0;                  //��ʱ����Ϊ�ͣ�׼����������λ
        delay_us(2);
        SCL=1;                  //��ʱ����Ϊ��ʹ��������������Ч
		delay_us(1); 
        retc<<=1;
        if(READ_SDA)retc++;       //������λ,���յ�����λ����retc�� 
		delay_us(1); 
   }

    if (!ack)
        I2C_NAck();//����nACK
    else
        I2C_Ack(); //����ACK   

  return retc;					
}



