#include <string.h>
#include "KeyboardControllerTasksCommon.h"
#include "HID_Report.h"
#include "Mailboxes.h"
#include "TCKBSerialProto.h"

void StartCommTask(void const * argument) {
  osEvent result;

  SP_Master_Init();

  while(1) {
    result = osMailGet(SERIAL_Mailbox, osWaitForever);
    if(osEventMail == result.status) {
      SP_Messages_u* ptr = (SP_Messages_u*)result.value.p;

      switch(ptr->Header.type) {
        case SET_LED_MODE:
          SP_Send_Set_Led_Mode(ptr->Set_Led_Mode.action);
          break;

        case SET_PRESSED_KEYS_6KRO:
          SP_Send_Pressed_Keys_6kro((Pressed_Keys_6kro_t*)ptr);
          break;

        default:
          break;
      }

      osMailFree(SERIAL_Mailbox, ptr);
    }
  }
}
