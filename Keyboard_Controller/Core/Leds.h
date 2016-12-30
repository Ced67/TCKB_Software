#ifndef LEDS_H
#define LEDS_H

#include "stm32l0xx_hal.h"
#include "cmsis_os.h"

extern osThreadId LedTaskHandle;

typedef struct {
  //1 = led on
  uint8_t CapsLock   :1;
  uint8_t ScrollLock :1;
  uint8_t _free      :6;

  uint32_t Brightness;
} LedsState_t;

extern LedsState_t LedState;

extern void LedsInit(void);
extern void LedsProcess(void);

extern void SetLeds(uint8_t USBState);

#endif //LEDS_H
