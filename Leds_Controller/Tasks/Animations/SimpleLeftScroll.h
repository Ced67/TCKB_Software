#ifndef SIMPLE_LEFT_SCROLL
#define SIMPLE_LEFT_SCROLL

#include "TCKBLedMatrix.h"
#include "TCKBSerialProto.h"
#include <string.h> //memcpy()

#include "GraphicTask.h"

extern void _Init_Anim_SimpleLeftScroll(void);
extern void _Anim_SimpleLeftScroll(void);
extern void _ProcessKeys6KRO_SimpleLeftScroll(Pressed_Keys_6kro_t* ptr);

#endif //SIMPLE_LEFT_SCROLL
