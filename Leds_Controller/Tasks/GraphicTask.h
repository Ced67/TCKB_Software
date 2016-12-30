#ifndef GRAPHIC_TASK_H
#define GRAPHIC_TASK_H

#include "TCKBSerialProto.h"

extern const uint8_t Brightness_Steps[ANIMATION_BRIGHTNESS_STEPS];

extern uint8_t Animation_Speed;
extern uint8_t Current_Brightness;
extern uint32_t _Last_Animation_Process_tick;

typedef struct {
  void (*Init)();
  void (*Anim)();
  void (*ProcessKeys6KRO)(Pressed_Keys_6kro_t* ptr);
} Animation_t;

extern Animation_t Current_Animation;

#endif //GRAPHIC_TASK_H
