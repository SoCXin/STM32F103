#include "usb_manager.h"
//#include "uuid.h"
//#include "sys.h"
#include "hw_config.h"
#include "unicode.h"
extern u8 EP1BUSY;			//键盘数据发送忙标志
//设置USB 连接/断线
//enable:0,断开
//       1,允许连接

extern  u8 enter_sta;
extern  u8 tab_sta;
extern u8 moudle_flag;
extern u8 uuid_flag;

void usb_port_set(u8 enable)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
    else
    {
        _SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
        GPIOA->CRH&=0XFFF00FFF;
        GPIOA->CRH|=0X00033000;
        PAout(12)=0;
    }
}

void usb_use_init(void)
{
//
//		//USB的支持。。。。。。
//    usb_port_set(0); 	//USB先断开	【清零】
//	delay_ms(300);
//    usb_port_set(1);	//USB再次连接	【置位，则用于USB功能】

//	//USB配置
//	USB_Disconnect_Config();
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();

}
void usb_use_dis(void)
{
    usb_port_set(0); 	//USB先断开	【清零】
}


void usb_print(char *q)
{
    char *tok;
    char m;
    char *p;
    delay_ms(20);
    for( p= q ; (*p) != '\0' ; p++)
    {
        tok=strtok(p,"");
        m= *tok;

        Joystick_Send(m);
        delay_us(1000);
        delay_us(300);
        delay_us(500);
        delay_us(500);
//				delay_us(500);

//				Joystick_Send(m);
//        delay_us(1020);


        Joystick_Send(0);
        delay_us(1000);
        delay_us(300);
        delay_us(500);
        delay_us(500);
//				Joystick_Send(0);
//				delay_us(1020);


    }
}

void usb_print_by_chinese(char *q, unsigned char model)
{
    char *tok;
    char m,lengh,i,legh;
    char *p;
    u8 OutString[10]= {0};
    u32 out_unicode;

    delay_ms(20);
    for( p= q ; (*p) != '\0' ; p++)
    {
        if(model==GBK_CODE)
        {
            if(*(u8*)p<0x81)
            {
                tok=strtok(p,"");
                m= *tok;

                Joystick_Send_by_language(m,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);


                Joystick_Send_by_language(m,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);
            }
            else
            {
                GBK_STRING(p,OutString);
                lengh=strlen((char*)OutString);
                for(i=0; i<lengh; i++)
                {
                    Joystick_Send_by_language(254,DISABLE,ENGLISH_LANGUAGE);
                    delay_us(1500);
                    if(OutString[i]==0x30)
                        Joystick_Send_by_language(0x62,ENABLE,ENGLISH_LANGUAGE);
                    else
                        Joystick_Send_by_language(OutString[i]+0x28,ENABLE,ENGLISH_LANGUAGE);
                    delay_us(1500);
                }
                Joystick_Send_by_language(254,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);
                Joystick_Send_by_language(0,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);
                memset(OutString,0,10);
                p++;
            }
        }
        else if(model==UNICODE_CODE)
        {
            lengh=IsTextUTF8(p);
            if(lengh==1)
            {
                tok=strtok(p,"");
                m= *tok;

                Joystick_Send_by_language(m,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);


                Joystick_Send_by_language(0,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);
            }
            else
            {
                strncpy((char*)OutString,p,lengh);
                enc_utf8_to_unicode_one(OutString,&out_unicode);
                memset(OutString,0,10);
                sprintf((char*)OutString,"%d",(int)out_unicode);
                legh=strlen((char*)OutString);
                for(i=0; i<legh; i++)
                {
                    Joystick_Send_by_language(254,DISABLE,ENGLISH_LANGUAGE);
                    delay_us(1500);
                    if(OutString[i]==0x30)
                        Joystick_Send_by_language(0x62,ENABLE,ENGLISH_LANGUAGE);
                    else
                        Joystick_Send_by_language(OutString[i]+0x28,ENABLE,ENGLISH_LANGUAGE);
                    delay_us(1500);
                }
                Joystick_Send_by_language(254,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);
                Joystick_Send_by_language(0,DISABLE,ENGLISH_LANGUAGE);
                delay_us(1500);
                memset(OutString,0,10);
                p+=(lengh-1);

            }

        }
    }
}


