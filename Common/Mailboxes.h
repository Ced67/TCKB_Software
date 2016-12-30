#ifndef TCKB_MAILBOXES_H
#define TCKB_MAILBOXES_H

#if defined(_PROJECT_LEDS_CONTROLLER)
  #include "stm32l1xx_hal.h"
#elif defined(_PROJECT_KEYBOARD_CONTROLLER)
  #include "stm32l0xx_hal.h"
#endif

#include "cmsis_os.h"

#include "TCKBSerialProto.h"


#ifdef _PROJECT_KEYBOARD_CONTROLLER
  #include "HID_Report.h"

  //6KRO mail from ScanTask to USBTask
  #define MAILBOX_6KRO_COUNT  1
  extern osMailQId KRO6_Mailbox;

  extern void TCKB_Init_KRO6_Mailbox(void);
#endif



//Serial mail from ScanTask to CommTask
#define MAILBOX_SERIAL_MESSAGES  1
extern osMailQId SERIAL_Mailbox;

extern void TCKB_Init_SERIAL_Mailbox(void);


#endif //TCKB_MAILBOXES_H
