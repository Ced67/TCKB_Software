#ifndef KRO6_H
#define KRO6_H

#include <stdbool.h>
#include "stm32l0xx_hal.h"
#include "usb_device.h"
#include "usbd_hid.h"
#include "Keys.h"
#include "TCKB_Common_Constants.h"

typedef struct {
  uint8_t Modifiers;
  uint8_t _reserved;
  uint8_t Keycode[KEYCOUNT_6KRO];
} __attribute__((packed)) hid_6kro_report_datas_t;


typedef struct {
  uint8_t _keycount; //Number of registered keys
  hid_6kro_report_datas_t datas;
} hid_6kro_report_t;


extern void KRO6_AddModifier(Key_t* Key);
extern void KRO6_AddKey(Key_t* Key);
extern bool KRO6_IsFull(void);
extern bool KRO6_IsEmpty(void);
extern void KRO6_Reset(void);

extern void KRO6_Send(void);

#endif //KRO6_H
