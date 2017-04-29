#ifndef NKRO_H
#define NKRO_H

#include <stdbool.h>
#if defined(STM32L053xx)
  #include "stm32l0xx_hal.h"
#elif defined(STM32L100xB) || defined(STM32L100x8)
  #include "stm32l1xx_hal.h"
#else
  #error "UNDEFINED TARGET"
#endif
#include "usb_device.h"
#include "usbd_hid.h"
#include "Keys.h"

//#define NKRO_REPORT_SEND_DELAY_Tk  1 //Nkro reports must leave at least 1ms between each occurence.

/*
typedef struct {
  uint8_t Modifiers;
  uint8_t _reserved;
  uint8_t Keycode[KEYCOUNT_6KRO];
} __attribute__((packed)) hid_6kro_report_datas_t;


typedef struct {
  uint8_t _keycount; //Number of registered keys
  hid_6kro_report_datas_t datas;
} hid_6kro_report_t;


extern void KRO6_AddModifier(uint8_t Mod);
extern void KRO6_AddKey(uint8_t Key);
extern bool KRO6_IsFull(void);
extern void KRO6_Reset(void);

extern void KRO6_Send(void);
*/
#endif //NKRO_H
