#ifndef UTILS_H
#define UTILS_H

#include "cmsis_os.h"
#include "FreeRTOSConfig.h"
#include "TCKB_Common_Constants.h"

extern void Utils_Init(void);
extern uint8_t* Get_CPUID_str(void);

extern uint32_t Get_Random(void);

#endif //UTILS_H
