#ifndef __GENERIC_USB_CDC_H
#define __GENERIC_USB_CDC_H

#include <stdint.h> // uint_fast8_t

// endpoint sizes
enum {
    USB_CDC_EP0_SIZE = 16,
    USB_CDC_EP_ACM_SIZE = 8,
    USB_CDC_EP_BULK_OUT_SIZE = 64,
    USB_CDC_EP_BULK_IN_SIZE = 64,
};

// callbacks provided by board specific code
int_fast8_t usb_read_bulk_out(void *data, uint_fast8_t max_len);
int_fast8_t usb_send_bulk_in(void *data, uint_fast8_t len);
int_fast8_t usb_read_ep0(void *data, uint_fast8_t max_len);
int_fast8_t usb_read_ep0_setup(void *data, uint_fast8_t max_len);
int_fast8_t usb_send_ep0(const void *data, uint_fast8_t len);
int_fast8_t usb_send_ep0_progmem(const void *data, uint_fast8_t len);
void usb_stall_ep0(void);
void usb_set_address(uint_fast8_t addr);
void usb_set_configure(void);
void usb_request_bootloader(void);
void usb_set_serial(uint8_t *);

// usb_cdc.c
void usb_notify_bulk_in(void);
void usb_notify_bulk_out(void);
void usb_notify_ep0(void);

#define CONFIG_USB_UID_CHIPID 1
#define USB_UID_LEN           16
#define USB_UID_DEFAULT       "0123456789ABCDEF0123456789ABCDEF"

#endif // usb_cdc.h
