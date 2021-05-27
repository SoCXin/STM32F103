
#ifndef USB_CONF_H_INCLUDED
#define USB_CONF_H_INCLUDED

#include <libopencm3/usb/usbd.h>

#define USB_VID                 0x1209
#define USB_PID                 0xdb42
#define USB_CONTROL_BUF_SIZE    1024
#define USB_SERIAL_NUM_LENGTH   24
#define INTF_DFU                0

extern void usb_set_serial_number(const char* serial);
extern usbd_device* usb_setup(void);

#endif
