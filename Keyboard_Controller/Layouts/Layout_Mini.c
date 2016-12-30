#include "Keys.h"
#include "Layout.h"

Key_t Matrix[NUM_LAYER][NUM_ROW][NUM_COL] = {
  {
    {KEY_LSHIFT,    KEY_1,    KEY_A,      KEY_J,    KEY_ENTER},
    {KEY_Fn,        KEY_LALT, KEY_SPACE,  KEY_V,    KEY_P}
  },
  {
    {KEY_CAPSLOCK,  KEY_F1,   KEY_BSPACE, KEY_X,    KEY_NONE},
    {KEY_Fn,        KEY_NONE, KEY_NONE,   KEY_NONE, KEY_LEFT}
  }
};


GPIO_TypeDef* _Row_Ports[NUM_ROW] = {Line_1_GPIO_Port,  Line_2_GPIO_Port};
uint16_t      _Row_Pins[NUM_ROW]  = {Line_1_Pin,        Line_2_Pin};

GPIO_TypeDef* _Col_Ports[NUM_COL] = {Col_1_GPIO_Port,   Col_2_GPIO_Port,   Col_3_GPIO_Port,   Col_4_GPIO_Port,   Col_5_GPIO_Port};
uint16_t      _Col_Pins[NUM_COL]  = {Col_1_Pin,         Col_2_Pin,         Col_3_Pin,         Col_4_Pin,         Col_5_Pin};
