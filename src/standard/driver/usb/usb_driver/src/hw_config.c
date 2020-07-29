/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "usb_lib.h"
#include "usb_manager.h"


//�Լ��̵�֧��
#define KEY_SEL    0x01
#define KEY_RIGHT  0x02
#define KEY_LEFT   0x04
#define KEY_DOWN   0x10
#define KEY_UP     0x08

							
//�����ּ���֧��
#define KEY_0   	0x27	   //��ֵ0
#define KEY_1		0x1e	   //��ֵ1
#define KEY_2		0x1f	   //��ֵ2
#define KEY_3		0x20	   //��ֵ3
#define KEY_4		0x21	   //��ֵ4
#define KEY_5		0x22	   //��ֵ5
#define KEY_6		0x23	   //��ֵ6
#define KEY_7		0x24	   //��ֵ7
#define KEY_8		0x25	   //��ֵ8
#define KEY_9		0x26	   //��ֵ9
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL
    /* Configure PLLs *********************************************************/
    /* PLL2 configuration: PLL2CLK = (HSE / 2) * 10 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div2);
    RCC_PLL2Config(RCC_PLL2Mul_10);

    /* Enable PLL2 */
    RCC_PLL2Cmd(ENABLE);

    /* Wait till PLL2 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
#endif

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock configuration.
       User can add here some code to deal with this error */    

    /* Go to infinite loop */
    while (1)
    {
    }
  }
  
  /* enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Set all the GPIOs to AIN */
  GPIO_AINConfig();

#ifndef STM32F10X_CL
  /* Enable the USB disconnect GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* USB_DISCONNECT used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
#endif /* STM32F10X_CL */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 	
  //GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE);//ʹ�ܸı䴮��һ�ܽŵ�ӳ��

  /* Configure the LED IOs */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
#ifdef STM32F10X_CL
  /* Select USBCLK source */
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  /* Enable the USB clock */ 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
#else
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
#endif /* STM32F10X_CL */
}

/*******************************************************************************
* Function Name  : GPIO_AINConfig
* Description    : Configures all IOs as AIN to reduce the power consumption.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void GPIO_AINConfig(void)
{

}
/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{

}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{

}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

#ifdef STM32F10X_CL
  /* Enable the USB Interrupts */
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_WKUP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);  
#else
  /* Enable the USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

#endif /* STM32F10X_CL */

}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef STM32F10X_CL  
  if (NewState != DISABLE)
  {
    USB_DevConnect();
  }
  else
  {
    USB_DevDisconnect();
  }
#else /* USE_STM3210B_EVAL or USE_STM3210E_EVAL */
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
#endif /* USE_STM3210C_EVAL */
}

/*******************************************************************************
* Function Name : JoyState.
* Description   : Decodes the Joystick direction.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
//uint8_t JoyState(void)
//{
//  /* "right" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_RIGHT))
//  {
//    return RIGHT;
//  }
//  /* "left" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_LEFT))
//  {
//    return LEFT;
//  }
//  /* "up" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_UP))
//  {
//    return UP;
//  }
//  /* "down" key is pressed */
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_DOWN))
//  {
//    return DOWN;
//  }
//  if (!GPIO_ReadInputDataBit(GPIOC, JOY_LEFT_BUTTON))
//  {
//    return LEFT_BUTTON;
//  }
//   if (!GPIO_ReadInputDataBit(GPIOC, JOY_RIGHT_BUTTON))
//  {
//    return RIGHT_BUTTON;
//  }
//  /* No key is pressed */
//  else
//  {
//    return 0;
//  }
//}

/*******************************************************************************
* Function Name : Joystick_Send.
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
//void Joystick_Send(uint8_t Keys)
//{
//  uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
//  int8_t X = 0, Y = 0,MouseButton=0;
//
//  switch (Keys)
//  {
//    case LEFT:
//      X -= CURSOR_STEP;
//      break;
//
//    case RIGHT:
//      X += CURSOR_STEP;
//      break;
//	  
//    case UP:
//      Y -= CURSOR_STEP;
//      break;
//
//    case DOWN:
//      Y += CURSOR_STEP;
//	  break;
//	   
//	case LEFT_BUTTON:
//      MouseButton = MouseButton|0x01;
//      break;
//
// 	case RIGHT_BUTTON:
//      MouseButton = MouseButton|0x02;
//      break;
//
//    default:
//      return;
//  }
//
//  /* prepare buffer to send */
//  Mouse_Buffer[0] = MouseButton;
//  Mouse_Buffer[1] = X;
//  Mouse_Buffer[2] = Y;
//  
//  /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
//  USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
//
//  if(Mouse_Buffer[0]!= 0)
//  {
//    Mouse_Buffer[0] = 0;
//    /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
//    USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
//  }   
//
//#ifndef STM32F10X_CL
//  /* Enable endpoint for transmission */
//  SetEPTxValid(ENDP1);
//#endif /* STM32F10X_CL */
//  
//  
//}

//void Joystick_Send(u8 buf0, u8 buf1)
//{
//  u8 Buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//  
//  Buffer[0] = buf0;
//  /* prepare buffer to send */
//  Buffer[3] = buf1;

//  if(Buffer[0]==0)	//����
//  {
//	  /*copy mouse position info in ENDP1 Tx Packet Memory Area*/
//	  UserToPMABufferCopy(Buffer, GetEPTxAddr(ENDP1), 8);
//	  /* enable endpoint for transmission */
//	  SetEPTxValid(ENDP1);
//  }
//  else				//���
//  {
//	  UserToPMABufferCopy(Buffer, GetEPTxAddr(ENDP3), 8);
//	  SetEPTxValid(ENDP3);
//  }
//														 
//}
void Joystick_Send(u8 Keys)
{
  u8 Buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //0,1,2,3,4,5,6,7
//  u8 i;
//  i=2;
  
  //�Ը����������д���ע�⣬���������ҡ��5������
  //������ͬʱ���£����Է��ص���ͨ���������ᳬ��6����
  //�����ļ���ͬʱ���µ���ͨ���ܹ�����6���Ļ�������Ҫ��
  //�����⴦���ˣ�������6�ֽ�ȫ������Ϊ0xFF����ʾ�����޷�ʶ��

   //��СдӢ����ĸ��֧��
  
//�Դ�дӢ����ĸ��֧��
	if(Keys == '\a')
	{
			Buffer[0] = 0x02;
	}
	else if (('A' <= Keys)&&(Keys <= 'Z') ) {    //int islower(int c);   ������c�Ƿ�ΪСдӢ����ĸ...
	  Buffer[0] = 0x02;
		Buffer[7] = (Keys - ('A' - 0x04));    
  }
  else if (('a' <= Keys)&&(Keys <= 'z'))
	{    //int islower(int c);   ������c�Ƿ�ΪСдӢ����ĸ...
	   Buffer[7] = (Keys - ('a' - 0x04));  
  }
	else {
  //�����ּ���֧��
	if(1)
	{
			switch(Keys){
			case 48 :
			Buffer[7] = KEY_0;	  //0  ���ַ�0��Ӧ��ASIC���� 48��
			break;
			case 49 :
			Buffer[7] = KEY_1;	  //1
			break;
			case 50 :
			Buffer[7] = KEY_2;    //2		
			break;
			case 51 :
			Buffer[7] = KEY_3;    //3
			break;
			case 52 :
			Buffer[7] = KEY_4;    //4
			break;
			case 53 :
			Buffer[7] = KEY_5;    //5
				break;
			case 54 :
			Buffer[7] = KEY_6;    //6
			break;
			case 55 :
			Buffer[7] = KEY_7;    //7
			break;
			case 56 :
			Buffer[7] = KEY_8;    //8
			break;
			case 57 :
			Buffer[7] = KEY_9;    //9
			break;	
			case 0x2c :
			Buffer[7] = 0x36;    //,
			break;
			case 0x2e :
			Buffer[7] = 0x37;    //.
			break;
			case 0x2F :
			Buffer[7] = 0x38;    //	/
			break;
			case 0x3b :
			Buffer[7] = 0x33;    //;    
			break;
			case 0x27 :
			Buffer[7] = 0x34;    //'    
			break;
			case 0x5b :
			Buffer[7]= 0x2f;    //[    
				break;
			case 0x5d  :
			Buffer[7] = 0x30;    //]    
			break;
			case 0x5c :
			Buffer[7] =0x31;    //\   
			break;
			case 0x60 :
			Buffer[7] = 0x35;    //`    
			break;
			case 0x2d :
			Buffer[7] = 0x2d;    //-     
			break;
			case 0x3d  :
			Buffer[7] = 0x2e;    //=  
			break;
			case '\n'  :
			Buffer[7] = 0x28; //�س��� 
			break;
			case '\t'  :
			Buffer[7] = 0x2b; //tab��
			break;
			case 32  :
			Buffer[7] = 0x2c; //tab��
			break;
		 
			
			//����shift��---
			case 0x3c :
			Buffer[0] =0x02; 
			Buffer[7]= 0x36;   //  <��Ӧ��ASICII����0x3c���ϱ�ֵ��0x36
			break;
			case 0x3e :
			Buffer[0] =0x02; 
			Buffer[7] = 0x37;    //  >��Ӧ��ASICII����0x3e���ϱ�ֵ��0x37
			break; 
			case 0x3f :
			Buffer[0] = 0x02;
			Buffer[7] = 0x38;   //  ?��Ӧ��ASICII����0x3f���ϱ�ֵ��0x38
			break;   
			case 0x3a :
			Buffer[0] = 0x02;
				Buffer[7] = 0x33;   //  :��Ӧ��ASICII����0x3a���ϱ�ֵ��0x33
			break;
			case 0x22 :
			Buffer[0] = 0x02;
			Buffer[7] = 0x34;	 //  "��Ӧ��ASICII����0x22���ϱ�ֵ��0x34
			break;
			case 0x7b :
			Buffer[0] = 0x02;
			Buffer[7] = 0x2f;	 //  {��Ӧ��ASICII����0x7b���ϱ�ֵ��0x2f
			break;
			case 0x7d :
			Buffer[0] = 0x02;
			Buffer[7] = 0x30;	 //  }��Ӧ��ASICII����0x7d���ϱ�ֵ��0x30	
			break;
			case 0x7c :
			Buffer[0] = 0x02;
			Buffer[7] = 0x31;	 //  |��Ӧ��ASICII����0x7c���ϱ�ֵ��0x31			 
			break;
			case 0x7e :
			Buffer[0] = 0x02;
			Buffer[7] = 0x35;	 //  ~��Ӧ��ASICII����0x7e���ϱ�ֵ��0x32
			break;
			case 0x21 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x1e;	 //  !��Ӧ��ASICII����0x21���ϱ�ֵ��0x1e
			 break;	

			case 0x40 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x1f;	 //  @��Ӧ��ASICII����0x40���ϱ�ֵ��0x1f
			 break;

				
			case 0x23 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x20;	 //  #��Ӧ��ASICII����0x23���ϱ�ֵ��0x20
			 break;

			case 0x24 :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x21;	 //  $��Ӧ��ASICII����0x24���ϱ�ֵ��0x21
			 break;
			
			case 0x25 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x22;	 //  %��Ӧ��ASICII����0x25���ϱ�ֵ��0x22
			 break;  

			case 0x5e :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x23;	 //  ^��Ӧ��ASICII����0x5e���ϱ�ֵ��0x23
			 break;

				
			case 0x26 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x24;	 //  &��Ӧ��ASICII����0x26���ϱ�ֵ��0x24
			 break;

				
			case 0x2a :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x25;	 //  *��Ӧ��ASICII����0x2a���ϱ�ֵ��0x25
			 break;

				
			case 0x28 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x26;	 //  (��Ӧ��ASICII����0x28���ϱ�ֵ��0x26
			 break;


				
			case 0x29 :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x27;	 //  )��Ӧ��ASICII����0x29���ϱ�ֵ��0x27
			 break;

			case 0x5f :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x2d;	 //  _��Ӧ��ASICII����0x5f���ϱ�ֵ��0x2d
			 break;

				
			case 0x2b :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x2e;	 //  +��Ӧ��ASICII����0x2b���ϱ�ֵ��0x2e
			 break;

			}

 }
	

}


	//���о������������������Ļ����������޷�ֹͣ����ģ��������Ҫ���������ʾֹͣ������
   if (Keys==0)
   {
	 Buffer[7] =0;           	
   }


  /*copy mouse position info in ENDP1 Tx Packet Memory Area ��PMA Packet Memory Area ��*/
  UserToPMABufferCopy(Buffer, ENDP3_TXADDR, 8);   //��8���ֽ�
//  /* enable endpoint for transmission */
//	 
  SetEPTxValid(ENDP3);
//	UserToPMABufferCopy(Buffer, ENDP3_TXADDR, 8);
//	SetEPTxCount(ENDP3, 8);
//	SetEPTxValid(ENDP3);

}

void Joystick_Send_by_language(u8 Keys,u8 confirm,u8 language)
{
  u8 Buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //0,1,2,3,4,5,6,7
//  u8 i;
//  i=2;
  
  //�Ը����������д���ע�⣬���������ҡ��5������
  //������ͬʱ���£����Է��ص���ͨ���������ᳬ��6����
  //�����ļ���ͬʱ���µ���ͨ���ܹ�����6���Ļ�������Ҫ��
  //�����⴦���ˣ�������6�ֽ�ȫ������Ϊ0xFF����ʾ�����޷�ʶ��

   //��СдӢ����ĸ��֧��
  
//�Դ�дӢ����ĸ��֧��
	if(Keys==254)
	{
			Buffer[0] =0x40;
	}
	else if (Keys>=0x59&&Keys<=0x62&&confirm==ENABLE)
	{
			Buffer[0] =0x40;
		  Buffer[7] =Keys;
	}
	else if(Keys == '\a')
	{
			Buffer[0] = 0x02;
	}
	else if (('A' <= Keys)&&(Keys <= 'Z') ) {    //int islower(int c);   ������c�Ƿ�ΪСдӢ����ĸ...
	  Buffer[0] = 0x02;
		Buffer[7] = (Keys - ('A' - 0x04));    
  }
  else if (('a' <= Keys)&&(Keys <= 'z'))
	{    //int islower(int c);   ������c�Ƿ�ΪСдӢ����ĸ...
	   Buffer[7] = (Keys - ('a' - 0x04));  
  }
	else {
  //�����ּ���֧��
	if(language == ENGLISH_LANGUAGE)
	{
			switch(Keys){
			case 48 :
			Buffer[7] = KEY_0;	  //0  ���ַ�0��Ӧ��ASIC���� 48��
			break;
			case 49 :
			Buffer[7] = KEY_1;	  //1
			break;
			case 50 :
			Buffer[7] = KEY_2;    //2		
			break;
			case 51 :
			Buffer[7] = KEY_3;    //3
			break;
			case 52 :
			Buffer[7] = KEY_4;    //4
			break;
			case 53 :
			Buffer[7] = KEY_5;    //5
				break;
			case 54 :
			Buffer[7] = KEY_6;    //6
			break;
			case 55 :
			Buffer[7] = KEY_7;    //7
			break;
			case 56 :
			Buffer[7] = KEY_8;    //8
			break;
			case 57 :
			Buffer[7] = KEY_9;    //9
			break;	
			case 0x2c :
			Buffer[7] = 0x36;    //,
			break;
			case 0x2e :
			Buffer[7] = 0x37;    //.
			break;
			case 0x2F :
			Buffer[7] = 0x38;    //	/
			break;
			case 0x3b :
			Buffer[7] = 0x33;    //;    
			break;
			case 0x27 :
			Buffer[7] = 0x34;    //'    
			break;
			case 0x5b :
			Buffer[7]= 0x2f;    //[    
				break;
			case 0x5d  :
			Buffer[7] = 0x30;    //]    
			break;
			case 0x5c :
			Buffer[7] =0x31;    //\   
			break;
			case 0x60 :
			Buffer[7] = 0x35;    //`    
			break;
			case 0x2d :
			Buffer[7] = 0x2d;    //-     
			break;
			case 0x3d  :
			Buffer[7] = 0x2e;    //=  
			break;
			case '\n'  :
			Buffer[7] = 0x28; //�س��� 
			break;
			case '\t'  :
			Buffer[7] = 0x2b; //tab��
			break;
			case 32  :
			Buffer[7] = 0x2c; //tab��
			break;
		 
			
			//����shift��---
			case 0x3c :
			Buffer[0] =0x02; 
			Buffer[7]= 0x36;   //  <��Ӧ��ASICII����0x3c���ϱ�ֵ��0x36
			break;
			case 0x3e :
			Buffer[0] =0x02; 
			Buffer[7] = 0x37;    //  >��Ӧ��ASICII����0x3e���ϱ�ֵ��0x37
			break; 
			case 0x3f :
			Buffer[0] = 0x02;
			Buffer[7] = 0x38;   //  ?��Ӧ��ASICII����0x3f���ϱ�ֵ��0x38
			break;   
			case 0x3a :
			Buffer[0] = 0x02;
				Buffer[7] = 0x33;   //  :��Ӧ��ASICII����0x3a���ϱ�ֵ��0x33
			break;
			case 0x22 :
			Buffer[0] = 0x02;
			Buffer[7] = 0x34;	 //  "��Ӧ��ASICII����0x22���ϱ�ֵ��0x34
			break;
			case 0x7b :
			Buffer[0] = 0x02;
			Buffer[7] = 0x2f;	 //  {��Ӧ��ASICII����0x7b���ϱ�ֵ��0x2f
			break;
			case 0x7d :
			Buffer[0] = 0x02;
			Buffer[7] = 0x30;	 //  }��Ӧ��ASICII����0x7d���ϱ�ֵ��0x30	
			break;
			case 0x7c :
			Buffer[0] = 0x02;
			Buffer[7] = 0x31;	 //  |��Ӧ��ASICII����0x7c���ϱ�ֵ��0x31			 
			break;
			case 0x7e :
			Buffer[0] = 0x02;
			Buffer[7] = 0x35;	 //  ~��Ӧ��ASICII����0x7e���ϱ�ֵ��0x32
			break;
			case 0x21 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x1e;	 //  !��Ӧ��ASICII����0x21���ϱ�ֵ��0x1e
			 break;	

			case 0x40 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x1f;	 //  @��Ӧ��ASICII����0x40���ϱ�ֵ��0x1f
			 break;

				
			case 0x23 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x20;	 //  #��Ӧ��ASICII����0x23���ϱ�ֵ��0x20
			 break;

			case 0x24 :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x21;	 //  $��Ӧ��ASICII����0x24���ϱ�ֵ��0x21
			 break;
			
			case 0x25 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x22;	 //  %��Ӧ��ASICII����0x25���ϱ�ֵ��0x22
			 break;  

			case 0x5e :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x23;	 //  ^��Ӧ��ASICII����0x5e���ϱ�ֵ��0x23
			 break;

				
			case 0x26 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x24;	 //  &��Ӧ��ASICII����0x26���ϱ�ֵ��0x24
			 break;

				
			case 0x2a :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x25;	 //  *��Ӧ��ASICII����0x2a���ϱ�ֵ��0x25
			 break;

				
			case 0x28 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x26;	 //  (��Ӧ��ASICII����0x28���ϱ�ֵ��0x26
			 break;


				
			case 0x29 :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x27;	 //  )��Ӧ��ASICII����0x29���ϱ�ֵ��0x27
			 break;

			case 0x5f :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x2d;	 //  _��Ӧ��ASICII����0x5f���ϱ�ֵ��0x2d
			 break;

				
			case 0x2b :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x2e;	 //  +��Ӧ��ASICII����0x2b���ϱ�ֵ��0x2e
			 break;

			}

 }
	else if(language == SPAIN_LANGUAGE)
	{
	   switch(Keys){
			case 48 :
			Buffer[7] = KEY_0;	  //0  ���ַ�0��Ӧ��ASIC���� 48��
			break;
			case 49 :
			Buffer[7] = KEY_1;	  //1
			break;
			case 50 :
			Buffer[7] = KEY_2;    //2		
			break;
			case 51 :
			Buffer[7] = KEY_3;    //3
			break;
			case 52 :
			Buffer[7] = KEY_4;    //4
			break;
			case 53 :
			Buffer[7] = KEY_5;    //5
				break;
			case 54 :
			Buffer[7] = KEY_6;    //6
			break;
			case 55 :
			Buffer[7] = KEY_7;    //7
			break;
			case 56 :
			Buffer[7] = KEY_8;    //8
			break;
			case 57 :
			Buffer[7] = KEY_9;    //9
			break;	
			case 0x2c :
			Buffer[7] = 0x36;    //,
			break;
			case 0x2e :
			Buffer[7] = 0x37;    //.
			break;
			case 0x2F :
			Buffer[0] =0x02;
			Buffer[7] = 0x24;    //	/
			break;
			case 0x3b :
			Buffer[0] =0x02;
			Buffer[7] = 0x36;    //;    
			break;
			case 0x27 :
			Buffer[7] = 0x34;    //'    
			break;
			case 0x5b :
			Buffer[0] =0x05;	
			Buffer[7]= 0x2f;    //[    
				break;
			case 0x5d  :
			Buffer[0] =0x05;	
			Buffer[7] = 0x30;    //]    
			break;
			case 0x5c :
			Buffer[0] =0x05;	
			Buffer[7] =0x35;    //\   
			break;
			case 0x60 :
			Buffer[7] = 0x2f;    //`    
			break;
			case 0x2d :
			Buffer[7] = 0x38;    //-     
			break;
			case 0x3d  :
			Buffer[0] =0x02;
			Buffer[7] = 0x27;    //=  
			break;
			case '\n'  :
			Buffer[7] = 0x28; //�س��� 
			break;
			case '\t'  :
			Buffer[7] = 0x2b; //tab��
			break;
			case 32  :
			Buffer[7] = 0x2c; //tab��
			break;
		 
			
			//����shift��---
			case 0x3c :
			Buffer[7]= 0x64;   //  <
			break; 
			case 0x3e :
			Buffer[0] =0x02; 
			Buffer[7] = 0x64;    //  >
			break; 
			case 0x3f :
			Buffer[0] = 0x02;
			Buffer[7] = 0x2d;   //  ?
			break;   
			case 0x3a :
			Buffer[0] = 0x02;
				Buffer[7] = 0x37;   //  :
			break;
			case 0x22 :
			Buffer[0] = 0x02;
			Buffer[7] = 0x34;	 //  "
			break;
			case 0x7b :
			Buffer[0] = 0x05;
			Buffer[7] = 0x34;	 //  {
			break;
			case 0x7d :
			Buffer[0] = 0x05;
			Buffer[7] = 0x31;	 //  }
			break;
			case 0x7c :
			Buffer[0] = 0x05;
			Buffer[7] = 0x1e;	 //  |		 
			break;
			case 0x7e :
			Buffer[0] = 0x05;
			Buffer[7] = 0x21;	 //  ~
			break;
			case 0x21 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x1e;	 //  !
			 break;	

			case 0x40 :
			 Buffer[0] = 0x05;
			 Buffer[7] = 0x1f;	 //  @
			 break;

				
			case 0x23 :
			 Buffer[0] = 0x05;
			 Buffer[7] = 0x20;	 //  #
			 break;

			case 0x24 :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x21;	 //  $
			 break;
			
			case 0x25 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x22;	 //  %
			 break;  

			case 0x5e :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x2f;	 //  ^
			 break;

				
			case 0x26 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x23;	 //  &
			 break;

				
			case 0x2a :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x30;	 //  *
			 break;

				
			case 0x28 :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x25;	 //  (
			 break;


				
			case 0x29 :
			 Buffer[0] = 0x02;
			 Buffer[7]= 0x26;	 //  )
			 break;

			case 0x5f :
			 Buffer[0] = 0x02;
			 Buffer[7] = 0x38;	 //  _
			 break;

				
			case 0x2b :
			 Buffer[7] = 0x30;	 //  +
			 break;

			}
		
	}

}


	//���о������������������Ļ����������޷�ֹͣ����ģ��������Ҫ���������ʾֹͣ������
   if (Keys==0)
   {
	 Buffer[7] =0;           	
   }


  /*copy mouse position info in ENDP1 Tx Packet Memory Area ��PMA Packet Memory Area ��*/
  UserToPMABufferCopy(Buffer, GetEPTxAddr(ENDP1), 8);   //��8���ֽ�
  /* enable endpoint for transmission */
  SetEPTxValid(ENDP1);

}


/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &CustomHID_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &CustomHID_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}
#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : USB_OTG_BSP_uDelay.
* Description    : provide delay (usec).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
  RCC_ClocksTypeDef  RCC_Clocks;  

  /* Configure HCLK clock as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  
  RCC_GetClocksFreq(&RCC_Clocks);
  
  SysTick_Config(usec * (RCC_Clocks.HCLK_Frequency / 1000000));  
  
  SysTick->CTRL  &= ~SysTick_CTRL_TICKINT_Msk ;
  
  while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
}
#endif
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

void USB_Disconnect_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable USB_DISCONNECT GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	/* USB_DISCONNECT_PIN used as USB pull-up */
	GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(USB_DISCONNECT,&GPIO_InitStructure);
}


