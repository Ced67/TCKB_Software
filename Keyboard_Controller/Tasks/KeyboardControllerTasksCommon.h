#ifndef KEYBOARD_CONTROLLER_TASKS_COMMON_H
#define KEYBOARD_CONTROLLER_TASKS_COMMON_H

#if defined(STM32L053xx)
  #include "stm32l0xx_hal.h"
#elif defined(STM32L100xB) || defined(STM32L100x8)
  #include "stm32l1xx_hal.h"
#else
  #error "UNDEFINED TARGET"
#endif
#include "cmsis_os.h"
#include "FreeRTOSConfig.h"

#if (configUSE_TRACE_FACILITY == 1)
	#include "trcKernelPort.h"
#endif

extern void StartUSBTask   (void const * argument);
extern void StartLedTask   (void const * argument);
extern void StartScanTask  (void const * argument);
extern void StartCommTask  (void const * argument);

#endif //KEYBOARD_CONTROLLER_TASKS_COMMON_H
