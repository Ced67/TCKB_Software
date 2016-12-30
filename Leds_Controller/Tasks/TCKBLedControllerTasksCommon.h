#ifndef LED_CONTROLLER_TASKS_COMMON_H
#define LED_CONTROLLER_TASKS_COMMON_H

#if defined(_PROJECT_LEDS_CONTROLLER)
  #include "stm32l1xx_hal.h"
#elif defined(_PROJECT_KEYBOARD_CONTROLLER)
  #include "stm32l0xx_hal.h"
#endif

#include "cmsis_os.h"
#include "FreeRTOSConfig.h"

extern void StartMultiplexTask	(void const * argument);
extern void StartGraphicTask	(void const * argument);
extern void StartCommTask		(void const * argument);

#endif //LED_CONTROLLER_TASKS_COMMON_H
