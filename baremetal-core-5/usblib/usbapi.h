#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <usb.h>
#include <stm32.h>
#include <usb_cdc.h>
#include <gpio.h>

#define CDC_EP0_SIZE    0x08
#define CDC_RXD_EP      0x01
#define CDC_TXD_EP      0x81
#define CDC_DATA_SZ     0x40
#define CDC_NTF_EP      0x82
#define CDC_NTF_SZ      0x08
#define HID_RIN_EP      0x83
#define HID_RIN_SZ      0x10

#define CDC_LOOPBACK
//#define ENABLE_HID_COMBO

//#define SIGNAL_MODEM  /* uncomment to signal modem capabilities */
//#define CDC_USE_IRQ   /* uncomment to build interrupt-based demo */

#if defined(SIGNAL_MODEM)
#define CDC_PROTOCOL USB_CDC_PROTO_V25TER
#else
#define CDC_PROTOCOL USB_PROTO_NONE
#endif

/* Declaration of the report descriptor */
struct cdc_config {
    struct usb_config_descriptor        config;
    struct usb_iad_descriptor           comm_iad;
    struct usb_interface_descriptor     comm;
    struct usb_cdc_header_desc          cdc_hdr;
    struct usb_cdc_call_mgmt_desc       cdc_mgmt;
    struct usb_cdc_acm_desc             cdc_acm;
    struct usb_cdc_union_desc           cdc_union;
    struct usb_endpoint_descriptor      comm_ep;
    struct usb_interface_descriptor     data;
    struct usb_endpoint_descriptor      data_eprx;
    struct usb_endpoint_descriptor      data_eptx;
} __attribute__((packed));

 
extern usbd_device udev;
extern uint32_t	ubuf[];
extern uint8_t     fifo[];
extern uint32_t    fpos;
extern int len;

void cdc_init_usbd(void);

#define UART_RX0_BUFFER_SIZE 64

#define UART_RX0_BUFFER_MASK (UART_RX0_BUFFER_SIZE - 1)
#define UART_FRAME_ERROR      0x0800             
#define UART_OVERRUN_ERROR    0x0400            
#define UART_BUFFER_OVERFLOW  0x0200           
#define UART_NO_DATA          0x0100   

extern uint8_t UART_RxBuf[];
extern uint8_t UART_TxHead;
extern uint8_t UART_TxTail;
extern uint8_t UART_RxHead;
extern uint8_t UART_RxTail;
extern uint8_t UART_LastRxError;
extern uint16_t usb_available(void);
extern uint16_t usb_getc(void);
extern uint8_t readBytes(char *buffer,uint8_t length);

