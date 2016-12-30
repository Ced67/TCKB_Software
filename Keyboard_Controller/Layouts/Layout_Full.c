#include "Keys.h"
#include "Layout.h"
#include <string.h>

//This is the default ISO/ANSI layout
Key_t Matrix[NUM_LAYER][NUM_ROW][NUM_COL] = {
  { //0 = Normal keys
    {KEY_ESC,      KEY_1,         KEY_2,    KEY_3,    KEY_4,    KEY_5,    KEY_6,     KEY_7,      KEY_8,    KEY_9,        KEY_0,         KEY_MINS,       KEY_EQUAL,    KEY_BSPACE},
    {KEY_TAB,      KEY_Q,         KEY_W,    KEY_E,    KEY_R,    KEY_T,    KEY_Y,     KEY_U,      KEY_I,    KEY_O,        KEY_P,         KEY_LBRACKET,   KEY_RBRACKET, KEY_BSLASH},
    {KEY_Fn,       KEY_A,         KEY_S,    KEY_D,    KEY_F,    KEY_G,    KEY_H,     KEY_J,      KEY_K,    KEY_L,        KEY_SEMICOLON, KEY_QUOTE,      KEY_BSLASH,   KEY_ENTER},
    {KEY_LSHIFT,   KEY_NU_BSLASH, KEY_Z,    KEY_X,    KEY_C,    KEY_V,    KEY_B,     KEY_N,      KEY_M,    KEY_COMMA,    KEY_DOT,       KEY_SLASH,      KEY_NONE,     KEY_RSHIFT},
    {KEY_LCTRL,    KEY_LGUI,      KEY_LALT, KEY_NONE, KEY_NONE, KEY_NONE, KEY_SPACE, KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_RALT,      KEY_RGUI,       KEY_BL,       KEY_RCTRL},
  },
  { //1 = Alt keys
    {KEY_GRAVE,    KEY_F1,        KEY_F2,   KEY_F3,   KEY_F4,   KEY_F5,   KEY_F6,    KEY_F7,     KEY_F8,   KEY_F9,       KEY_F10,       KEY_F11,        KEY_F12,      KEY_DELETE},
    {KEY_NONE,     KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_PAGEUP, KEY_UP,   KEY_PAGEDOWN, KEY_PRINTSCR,  KEY_SCROLLLOCK, KEY_PAUSE,    KEY_NONE},
    {KEY_Fn,       KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_HOME,  KEY_LEFT,   KEY_DOWN, KEY_RIGHT,    KEY_INSERT,    KEY_NONE,       KEY_NONE,     KEY_NONE},
    {KEY_LSHIFT,   KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_END,    KEY_NONE, KEY_NONE,     KEY_NONE,      KEY_NONE,       KEY_NONE,     KEY_CAPSLOCK},
    {KEY_LCTRL,    KEY_LGUI,      KEY_LALT, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_RALT,      KEY_RGUI,       KEY_NONE,     KEY_RCTRL},
  },
  {
    {KEY_BL_OFF,   KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_BL_BRIGHT_DOWN, KEY_BL_BRIGHT_UP,  KEY_NONE,   KEY_NONE, KEY_BL_NEXT,     KEY_NONE,      KEY_BL_SPEED_DOWN,       KEY_BL_SPEED_UP,     KEY_NONE},
    {KEY_NONE,     KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_NONE,      KEY_NONE,       KEY_NONE,     KEY_NONE},
    {KEY_NONE,     KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_NONE,      KEY_NONE,       KEY_NONE,     KEY_BL_ON},
    {KEY_NONE,     KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_NONE,      KEY_NONE,       KEY_NONE,     KEY_NONE},
    {KEY_NONE,     KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_NONE,      KEY_NONE,       KEY_BL,       KEY_NONE},
  }
};

//This alternative layout is activated with switch DIP_2 = ON
//The Backlight control layer is unchanged
//At the moment it is equal to standard but can be set to dvorak or colemak or anything you want!
const Key_t AlternativeLayout[NUM_LAYER-1][NUM_ROW][NUM_COL] = {
  { //0 = Normal keys
    {KEY_ESC,      KEY_1,         KEY_2,    KEY_3,    KEY_4,    KEY_5,    KEY_6,     KEY_7,      KEY_8,    KEY_9,        KEY_0,         KEY_MINS,       KEY_EQUAL,    KEY_BSPACE},
    {KEY_TAB,      KEY_Q,         KEY_W,    KEY_E,    KEY_R,    KEY_T,    KEY_Y,     KEY_U,      KEY_I,    KEY_O,        KEY_P,         KEY_LBRACKET,   KEY_RBRACKET, KEY_BSLASH},
    {KEY_Fn,       KEY_A,         KEY_S,    KEY_D,    KEY_F,    KEY_G,    KEY_H,     KEY_J,      KEY_K,    KEY_L,        KEY_SEMICOLON, KEY_QUOTE,      KEY_BSLASH,   KEY_ENTER},
    {KEY_LSHIFT,   KEY_NU_BSLASH, KEY_Z,    KEY_X,    KEY_C,    KEY_V,    KEY_B,     KEY_N,      KEY_M,    KEY_COMMA,    KEY_DOT,       KEY_SLASH,      KEY_NONE,     KEY_RSHIFT},
    {KEY_LCTRL,    KEY_LGUI,      KEY_LALT, KEY_NONE, KEY_NONE, KEY_NONE, KEY_SPACE, KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_RALT,      KEY_RGUI,       KEY_BL,       KEY_RCTRL},
  },
  { //1 = Alt keys
    {KEY_GRAVE,    KEY_F1,        KEY_F2,   KEY_F3,   KEY_F4,   KEY_F5,   KEY_F6,    KEY_F7,     KEY_F8,   KEY_F9,       KEY_F10,       KEY_F11,        KEY_F12,      KEY_DELETE},
    {KEY_NONE,     KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_PAGEUP, KEY_UP,   KEY_PAGEDOWN, KEY_PRINTSCR,  KEY_SCROLLLOCK, KEY_PAUSE,    KEY_NONE},
    {KEY_Fn,       KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_HOME,  KEY_LEFT,   KEY_DOWN, KEY_RIGHT,    KEY_INSERT,    KEY_NONE,       KEY_NONE,     KEY_NONE},
    {KEY_LSHIFT,   KEY_NONE,      KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_END,    KEY_NONE, KEY_NONE,     KEY_NONE,      KEY_NONE,       KEY_NONE,     KEY_CAPSLOCK},
    {KEY_LCTRL,    KEY_LGUI,      KEY_LALT, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,  KEY_NONE,   KEY_NONE, KEY_NONE,     KEY_RALT,      KEY_RGUI,       KEY_NONE,     KEY_RCTRL},
  },
};



const GPIO_TypeDef* _Row_Ports[NUM_ROW] = {Line_0_GPIO_Port,  Line_1_GPIO_Port,  Line_2_GPIO_Port,  Line_3_GPIO_Port,  Line_4_GPIO_Port};
const uint16_t      _Row_Pins[NUM_ROW]  = {Line_0_Pin,        Line_1_Pin,        Line_2_Pin,        Line_3_Pin,        Line_4_Pin      };

const GPIO_TypeDef* _Col_Ports[NUM_COL] = {Col_0_GPIO_Port,   Col_1_GPIO_Port,   Col_2_GPIO_Port,   Col_3_GPIO_Port,   Col_4_GPIO_Port,   Col_5_GPIO_Port,   Col_6_GPIO_Port,   Col_7_GPIO_Port,   Col_8_GPIO_Port,   Col_9_GPIO_Port,   Col_10_GPIO_Port,   Col_11_GPIO_Port,   Col_12_GPIO_Port,   Col_13_GPIO_Port};
const uint16_t      _Col_Pins[NUM_COL]  = {Col_0_Pin,         Col_1_Pin,         Col_2_Pin,         Col_3_Pin,         Col_4_Pin,         Col_5_Pin,         Col_6_Pin,         Col_7_Pin,         Col_8_Pin,         Col_9_Pin,         Col_10_Pin,         Col_11_Pin,         Col_12_Pin,         Col_13_Pin      };


void InitLayout(void) {
  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(ALT_LAYOUT_GPIO_Port, ALT_LAYOUT_Pin)) {
    //Init alternative layout
    memcpy(&(Matrix[0][0][0]), AlternativeLayout, sizeof(AlternativeLayout));

    //Disable pulldown on pin ALT_LAYOUT to prevent current draw
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin  = ALT_LAYOUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ALT_LAYOUT_GPIO_Port, &GPIO_InitStruct);
  }
}

