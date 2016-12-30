#ifndef TCKB_EEPROM_H
#define TCKB_EEPROM_H

#include <stdbool.h>
#include "cmsis_os.h"
#include "FreeRTOSConfig.h"
#include "TCKB_Common_Constants.h"
#include "TCKBLedMatrix.h"

#define TCKB_EEP_LEDS_STRUCT_VERSION 1


typedef union {
  uint8_t All;
  struct {
    //lsb
    uint8_t FullON  :1;
    uint8_t FullOFF :1;
    uint8_t _free   :6;
    //msb
  };
} __attribute__((packed)) EEP_flags_t;


extern void EEP_Init(void);

extern uint8_t Get_EEP_BrightnessIndex(void);
extern void Set_EEP_BrightnessIndex(uint8_t val);

extern uint8_t Get_EEP_AnimationSpeed(void);
extern void Set_EEP_AnimationSpeed(uint8_t val);

extern LedAnimationType_e Get_EEP_LedAnimationType(void);
extern void Set_EEP_LedAnimationType(LedAnimationType_e val);

extern bool Get_EEP_Flags_FullON(void);
extern void Set_EEP_Flags_FullON(void);

extern bool Get_EEP_Flags_FullOFF(void);
extern void Set_EEP_Flags_FullOFF(void);

#endif //TCKB_EEPROM_H
