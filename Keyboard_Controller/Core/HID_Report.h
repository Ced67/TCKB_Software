#ifndef HID_REPORT_H
#define HID_REPORT_H

#include <stdbool.h>
#include "cmsis_os.h"
#include "stm32l0xx_hal.h"

#include "6kro_report.h"
#include "Nkro_report.h"

typedef enum {
  MODE_6KRO = 0,
  MODE_NKRO,
} Report_Mode_t;


extern void Report_SetMode(Report_Mode_t Mode);
extern Report_Mode_t Report_GetMode(void);

extern void Report_Init(void);

extern void Report_AddModifier(Key_t* Key);
extern void Report_AddKey(Key_t* Key);
extern bool Report_IsFull(void);
extern bool Report_IsEmpty(void);
extern void Report_Reset(void);

extern void Report_Send(void);

#endif //HID_REPORT_H
