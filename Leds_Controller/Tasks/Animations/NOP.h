#ifndef ANIM_NOP
#define ANIM_NOP

#include "TCKBLedMatrix.h"
#include "TCKBSerialProto.h"
#include <string.h> //memcpy()

#include "GraphicTask.h"

extern void _Init_Anim_NOP(void);
extern void _Anim_NOP(void);
extern void _ProcessKeys6KRO_NOP(Pressed_Keys_6kro_t* ptr);

#endif //ANIM_NOP
