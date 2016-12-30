#include "Leds.h"
#include "usb_device.h"
#include "usbd_hid.h"
#include "TCKB_Common_Constants.h"

LedsState_t LedState;



static void _LedsOFF(void) {
  #ifdef USE_CAPSLOCK_LED
    CAPS_LOCK_PWM_TIMER.Instance->CAPS_LOCK_PWM_CCR = 0; //CapsLock
  #endif
  #ifdef USE_SCROLLOCK_LED
    SCROLL_LOCK_PWM_TIMER.Instance->SCROLL_LOCK_PWM_CCR = 0; //ScrollLock
  #endif
}



void LedsInit(void) {
  memset(&LedState, 0x00, sizeof(LedsState_t));

  LedState.Brightness = LED_PWM_Delta;

  _LedsOFF();
}



void LedsProcess(void) {
  //If host system is halted, we should turn all leds off.
  if(USBD_STATE_SUSPENDED == hUsbDeviceFS.dev_state) {
    _LedsOFF();
    return;
  }

  //Manage system leds (on/off)
  #ifdef USE_CAPSLOCK_LED
    CAPS_LOCK_PWM_TIMER.Instance->CAPS_LOCK_PWM_CCR = (LedState.CapsLock)?(LedState.Brightness):(0);
  #endif
  #ifdef USE_SCROLLOCK_LED
    SCROLL_LOCK_PWM_TIMER.Instance->SCROLL_LOCK_PWM_CCR = (LedState.ScrollLock)?(LedState.Brightness):(0);
  #endif
}



void SetLeds(uint8_t USBState) {
  //Caps Lock Led
  LedState.CapsLock   = (USBState & 0x02)?1:0;

  //Scroll Lock Led
  LedState.ScrollLock = (USBState & 0x04)?1:0;

  osSignalSet(LedTaskHandle, 0x1);
}
