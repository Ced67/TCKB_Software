#ifndef LAYOUT_MINI_H
#define LAYOUT_MINI_H

#include "TCKB_Common_Constants.h"
#include "Keys.h"
#include "stm32l0xx_hal.h"

#define LAYOUT_DEFAULT_LAYER    0
#define LAYOUT_FUNCTION_LAYER   1
#define LAYOUT_BACKLIGHT_LAYER  2

extern Key_t Matrix[NUM_LAYER][NUM_ROW][NUM_COL];

extern const GPIO_TypeDef* _Row_Ports[NUM_ROW];
extern const uint16_t      _Row_Pins[NUM_ROW];

extern const GPIO_TypeDef* _Col_Ports[NUM_COL];
extern const uint16_t      _Col_Pins[NUM_COL];

extern void InitLayout(void);

#endif //LAYOUT_MINI_H
