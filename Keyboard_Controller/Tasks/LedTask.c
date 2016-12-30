//LedsInit

#include "KeyboardControllerTasksCommon.h"
#include "Leds.h"
#include "Mailboxes.h"
#include "TCKB_Common_Constants.h"

void StartLedTask(void const * argument) {
  #ifdef USE_CAPSLOCK_LED
    HAL_TIM_PWM_Start(&(CAPS_LOCK_PWM_TIMER), CAPS_LOCK_PWM_CHANNEL);
  #endif

  #ifdef USE_SCROLLOCK_LED
    HAL_TIM_PWM_Start(&(SCROLL_LOCK_PWM_TIMER), SCROLL_LOCK_PWM_CHANNEL);
  #endif

  LedsInit();

  while(1) {
    //Wait for a notification from USB stack (through USBD_HID_DataOut() IRQ)
    osSignalWait(0x1, osWaitForever);
    LedsProcess(); //Refresh led
  }
}
