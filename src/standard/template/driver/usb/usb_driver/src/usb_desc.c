/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Descriptors for Joystick Mouse Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
{
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize40*/
    0x83,                       /*idVendor (0x0483)*/
    0x04,
    0x50,                       /*idProduct = 0x5750*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing manufacturer */
    2,                          /*Index of string descriptor describing product*/
    3,                          /*Index of string descriptor describing the device serial number */
    0x01                        /*bNumConfigurations*/
}; /* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/* All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
{
    0x09, /* bLength: Configuation Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    CUSTOMHID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x02,         /* bNumInterfaces: 1 interface */
    0x01,         /* bConfigurationValue: Configuration value */
    0x00,         /* iConfiguration: Index of string descriptor describing
                                 the configuration*/
    0xC0,         /* bmAttributes: Bus powered */
                  /*Bus powered: 7th bit, Self Powered: 6th bit, Remote wakeup: 5th bit, reserved: 4..0 bits */
//    0x32,         /* MaxPower 100 mA: this current is used for detecting Vbus */
    0x96,         /* MaxPower 300 mA: this current is used for detecting Vbus */
	
	
		//第一个HID接口
    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x00,         /* bInterfaceNumber: Number of Interface */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x02,         /* bNumEndpoints */
    0x03,         /* bInterfaceClass: HID */
    0x00,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
    CUSTOMHID_SIZ_REPORT_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */

    0x82,          /* bEndpointAddress: Endpoint Address (IN) */               
                   // bit 3...0 : the endpoint number
                   // bit 6...4 : reserved
                    // bit 7     : 0(OUT), 1(IN)
    0x03,          /* bmAttributes: Interrupt endpoint */
    0x40,          /* wMaxPacketSize: 64 Bytes max */
    0x00,
    0x02,          /* bInterval: Polling Interval (2 ms) */
    /* 34 */
    	
    0x07,	/* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
			/*	Endpoint descriptor type */
    0x01,	/* bEndpointAddress: */
			/*	Endpoint Address (OUT) */
    0x03,	/* bmAttributes: Interrupt endpoint */
    0x40,	/* wMaxPacketSize: 64 Bytes max  */
    0x00,
    0x02,	/* bInterval: Polling Interval (2 ms) */
    /* 41 */
		
		
		//第二个HID接口
		 /* 09 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x01,         /* bInterfaceNumber: Number of Interface */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x02,         /* bNumEndpoints */
    0x03,         /* bInterfaceClass: HID */
    0x01,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x19,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
    CUSTOMHID_KEYBOARD_SIZ_REPORT_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */

    0x83,          /* bEndpointAddress: Endpoint Address (IN) */               
                   // bit 3...0 : the endpoint number
                   // bit 6...4 : reserved
                    // bit 7     : 0(OUT), 1(IN)
    0x03,          /* bmAttributes: Interrupt endpoint */
    0x40,          /* wMaxPacketSize: 64 Bytes max */
    0x00,
    0x01,          /* bInterval: Polling Interval (2 ms) */
    /* 34 */
    	
    0x07,	/* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
			/*	Endpoint descriptor type */
    0x03,	/* bEndpointAddress: */
			/*	Endpoint Address (OUT) */
    0x03,	/* bmAttributes: Interrupt endpoint */
    0x40,	/* wMaxPacketSize: 64 Bytes max  */
    0x00,
    0x01,	/* bInterval: Polling Interval (2 ms) */
    /* 41 */
}; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] = 
{ 
	0x05, 0x8c, /* USAGE_PAGE (ST Page) */ 
	0x09, 0x01, /* USAGE (Demo Kit) */ 
	0xa1, 0x01, /* COLLECTION (Application) */ 
	
	// The Input report 
	0x09,0x03, // USAGE ID - Vendor defined 
	0x15,0x00, // LOGICAL_MINIMUM (0) 
	0x26,0x00, 0xFF, // LOGICAL_MAXIMUM (255) 
	0x75,0x08, // REPORT_SIZE (8bit) 
	0x95,0x40, // REPORT_COUNT (64Byte) 
	0x81,0x02, // INPUT (Data,Var,Abs) 

	// The Output report 
	0x09,0x04, // USAGE ID - Vendor defined 
	0x15,0x00, // LOGICAL_MINIMUM (0) 
	0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255) 
	0x75,0x08, // REPORT_SIZE (8bit) 
	0x95,0x40, // REPORT_COUNT (64Byte) 
	0x91,0x02, // OUTPUT (Data,Var,Abs) 

	0xc0 /* END_COLLECTION */ 
}; /* CustomHID_ReportDescriptor */ 

const uint8_t CustomHID_KeyBoardReportDescriptor[CUSTOMHID_KEYBOARD_SIZ_REPORT_DESC] =
  {
	 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
	 0x09, 0x06, // USAGE (Keyboard)
	 0xa1, 0x01, // COLLECTION (Application)
	 0x05, 0x07, //     USAGE_PAGE (Keyboard/Keypad)
	 0x19, 0xe0, //     USAGE_MINIMUM (Keyboard LeftControl)
	 0x29, 0xe7, //     USAGE_MAXIMUM (Keyboard Right GUI)
	 0x15, 0x00, //     LOGICAL_MINIMUM (0)
	 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
	 0x95, 0x08, //     REPORT_COUNT (8)
	 0x75, 0x01, //     REPORT_SIZE (1)
	 0x81, 0x02, //     INPUT (Data,Var,Abs)
	 0x95, 0x01, //     REPORT_COUNT (1)
	 0x75, 0x08, //     REPORT_SIZE (8)
	 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
	 0x95, 0x06, //   REPORT_COUNT (6)
	 0x75, 0x08, //   REPORT_SIZE (8)
	 0x25, 0xFF, //   LOGICAL_MAXIMUM (255)
	 0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
	 0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
	 0x81, 0x00, //     INPUT (Data,Ary,Abs)
	 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
	 0x95, 0x02, //   REPORT_COUNT (2)
	 0x75, 0x01, //   REPORT_SIZE (1)
	 0x05, 0x08, //   USAGE_PAGE (LEDs)
	 0x19, 0x01, //   USAGE_MINIMUM (Num Lock)
	 0x29, 0x02, //   USAGE_MAXIMUM (Caps Lock)
	 0x91, 0x02, //   OUTPUT (Data,Var,Abs)
	 0x95, 0x01, //   REPORT_COUNT (1)
	 0x75, 0x06, //   REPORT_SIZE (6)
	 0x91, 0x03, //   OUTPUT (Cnst,Var,Abs)
	 0xc0        // END_COLLECTION
 
//		0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)	//63
//    0x09, 0x06,                    // USAGE (Keyboard)
//    0xa1, 0x01,                    // COLLECTION (Application)
//    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
//    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
//    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
//    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
//    0x75, 0x01,                    //   REPORT_SIZE (1)
//    0x95, 0x08,                    //   REPORT_COUNT (8)
//    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//    0x95, 0x01,                    //   REPORT_COUNT (1)
//    0x75, 0x08,                    //   REPORT_SIZE (8)
//    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
//    0x95, 0x05,                    //   REPORT_COUNT (5)
//    0x75, 0x01,                    //   REPORT_SIZE (1)
//    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
//    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
//    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
//    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
//    0x95, 0x01,                    //   REPORT_COUNT (1)
//    0x75, 0x03,                    //   REPORT_SIZE (3)
//    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
//    0x95, 0x06,                    //   REPORT_COUNT (6)
//    0x75, 0x08,                    //   REPORT_SIZE (8)
//    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
//    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
//    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
//    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
//    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
//    0xc0,                           // END_COLLECTION

  };


/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
{
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
}; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
{
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    // Manufacturer: "STMicroelectronics" 
    'M', 0, 'y', 0, 'U', 0,'S', 0,'B', 0, '_', 0, 'H', 0,'I',0,'D',0
};

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
{
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'B', 0, 'y', 0, ' ', 0, 'v', 0, 'i', 0, 'e', 0,'w',0,'t',0,'o',0,'o',0,'l',0
};
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
{
    CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'x', 0, 'x', 0, 'x', 0,'x', 0,'x', 0, 'x', 0, 'x', 0
};
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

