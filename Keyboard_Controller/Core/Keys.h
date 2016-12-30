#ifndef KEYS_H
#define KEYS_H

#include <stdbool.h>

#include "stm32l0xx_hal.h"
#include "names_key.h"
#include "TCKBSerialProto.h" //For backlig

//Do not change the fields order in that structure!! (will break all the keys definitions below)
typedef struct {
  union {
    uint8_t Flags;
    struct {
      uint8_t Modifier  :1;
      uint8_t Fn        :1;
      uint8_t Bl        :1;
      uint8_t Newpress  :1; //Oneshot press event (for backlight action)
      uint8_t Pressed   :1; //Pressed state (for constant usb report transmission)
      uint8_t Debounce  :3;
    };
  } __attribute__((packed));

  uint8_t ModValue; //Type of modifier (LCTRL, RALT, etc..)

  union {
    uint8_t Code;     //Normal Key Code
    uint8_t Action;   //Backlight control action
  } __attribute__((packed));

} __attribute__((packed)) Key_t;


#define FL_STD  0x00  //Standard key
#define FL_MOD  0x01  //Modifier
#define FL_FN   0x02  //Fn Key for second layer
#define FL_BL   0x04  //Backlight control

//------------------------------------------------------------------
//Public functions
extern void     ReadKey   (GPIO_TypeDef* Col_Port,
                           uint16_t Col_Pin,
                           Key_t* Key);

extern bool     IsPressed (Key_t* Key);
extern bool     IsNewpress(Key_t* Key);
extern uint8_t  GetCode   (Key_t* Key);
extern uint8_t  ModValue  (Key_t* Key);
extern bool     IsFn      (Key_t* Key);
extern bool     IsBl      (Key_t* Key);
extern bool     IsMod     (Key_t* Key);


//------------------------------------------------------------------
//Modifiers value
#define MOD_L_CTRL      0x01
#define MOD_L_SHIFT     0x02
#define MOD_L_ALT       0x04
#define MOD_L_GUI       0x08
#define MOD_R_CTRL      0x10
#define MOD_R_SHIFT     0x20
#define MOD_R_ALT       0x40
#define MOD_R_GUI       0x80



//------------------------------------------------------------------
//Most used key declaration
#define KEY_ESC       {.Flags=FL_STD, 0x00,         .Code=KC_ESC}
#define KEY_1         {.Flags=FL_STD, 0x00,         .Code=KC_1}
#define KEY_2         {.Flags=FL_STD, 0x00,         .Code=KC_2}
#define KEY_3         {.Flags=FL_STD, 0x00,         .Code=KC_3}
#define KEY_4         {.Flags=FL_STD, 0x00,         .Code=KC_4}
#define KEY_5         {.Flags=FL_STD, 0x00,         .Code=KC_5}
#define KEY_6         {.Flags=FL_STD, 0x00,         .Code=KC_6}
#define KEY_7         {.Flags=FL_STD, 0x00,         .Code=KC_7}
#define KEY_8         {.Flags=FL_STD, 0x00,         .Code=KC_8}
#define KEY_9         {.Flags=FL_STD, 0x00,         .Code=KC_9}
#define KEY_0         {.Flags=FL_STD, 0x00,         .Code=KC_0}
#define KEY_MINS      {.Flags=FL_STD, 0x00,         .Code=KC_MINS}
#define KEY_EQUAL     {.Flags=FL_STD, 0x00,         .Code=KC_EQUAL}
#define KEY_BSPACE    {.Flags=FL_STD, 0x00,         .Code=KC_BSPACE}

#define KEY_TAB       {.Flags=FL_STD, 0x00,         .Code=KC_TAB}
#define KEY_Q         {.Flags=FL_STD, 0x00,         .Code=KC_Q}
#define KEY_W         {.Flags=FL_STD, 0x00,         .Code=KC_W}
#define KEY_E         {.Flags=FL_STD, 0x00,         .Code=KC_E}
#define KEY_R         {.Flags=FL_STD, 0x00,         .Code=KC_R}
#define KEY_T         {.Flags=FL_STD, 0x00,         .Code=KC_T}
#define KEY_Y         {.Flags=FL_STD, 0x00,         .Code=KC_Y}
#define KEY_U         {.Flags=FL_STD, 0x00,         .Code=KC_U}
#define KEY_I         {.Flags=FL_STD, 0x00,         .Code=KC_I}
#define KEY_O         {.Flags=FL_STD, 0x00,         .Code=KC_O}
#define KEY_P         {.Flags=FL_STD, 0x00,         .Code=KC_P}
#define KEY_LBRACKET  {.Flags=FL_STD, 0x00,         .Code=KC_LBRACKET}
#define KEY_RBRACKET  {.Flags=FL_STD, 0x00,         .Code=KC_RBRACKET}
#define KEY_BSLASH    {.Flags=FL_STD, 0x00,         .Code=KC_BSLASH}

#define KEY_Fn        {.Flags=FL_FN,  0x00,         .Code=0x00}
#define KEY_A         {.Flags=FL_STD, 0x00,         .Code=KC_A}
#define KEY_S         {.Flags=FL_STD, 0x00,         .Code=KC_S}
#define KEY_D         {.Flags=FL_STD, 0x00,         .Code=KC_D}
#define KEY_F         {.Flags=FL_STD, 0x00,         .Code=KC_F}
#define KEY_G         {.Flags=FL_STD, 0x00,         .Code=KC_G}
#define KEY_H         {.Flags=FL_STD, 0x00,         .Code=KC_H}
#define KEY_J         {.Flags=FL_STD, 0x00,         .Code=KC_J}
#define KEY_K         {.Flags=FL_STD, 0x00,         .Code=KC_K}
#define KEY_L         {.Flags=FL_STD, 0x00,         .Code=KC_L}
#define KEY_SEMICOLON {.Flags=FL_STD, 0x00,         .Code=KC_SEMICOLON}
#define KEY_QUOTE     {.Flags=FL_STD, 0x00,         .Code=KC_QUOTE}
//#define KEY_BSLASH    {.Flags=FL_STD, 0x00,         .Code=KC_BSLASH}
#define KEY_ENTER     {.Flags=FL_STD, 0x00,         .Code=KC_ENTER}

#define KEY_LSHIFT    {.Flags=FL_MOD, MOD_L_SHIFT,  .Code=KC_LSHIFT}
#define KEY_NU_BSLASH {.Flags=FL_STD, 0x00,         .Code=KC_NU_BSLASH}
#define KEY_Z         {.Flags=FL_STD, 0x00,         .Code=KC_Z}
#define KEY_X         {.Flags=FL_STD, 0x00,         .Code=KC_X}
#define KEY_C         {.Flags=FL_STD, 0x00,         .Code=KC_C}
#define KEY_V         {.Flags=FL_STD, 0x00,         .Code=KC_V}
#define KEY_B         {.Flags=FL_STD, 0x00,         .Code=KC_B}
#define KEY_N         {.Flags=FL_STD, 0x00,         .Code=KC_N}
#define KEY_M         {.Flags=FL_STD, 0x00,         .Code=KC_M}
#define KEY_COMMA     {.Flags=FL_STD, 0x00,         .Code=KC_COMMA}
#define KEY_DOT       {.Flags=FL_STD, 0x00,         .Code=KC_DOT}
#define KEY_SLASH     {.Flags=FL_STD, 0x00,         .Code=KC_SLASH}
#define KEY_NONE      {.Flags=FL_STD, 0x00,         .Code=0x00}
#define KEY_RSHIFT    {.Flags=FL_MOD, MOD_R_SHIFT,  .Code=KC_RSHIFT}

#define KEY_LCTRL     {.Flags=FL_MOD, MOD_L_CTRL,   .Code=KC_LCTRL}
#define KEY_LGUI      {.Flags=FL_MOD, MOD_L_GUI,    .Code=KC_LGUI}
#define KEY_LALT      {.Flags=FL_MOD, MOD_L_ALT,    .Code=KC_LALT}
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
#define KEY_SPACE     {.Flags=FL_STD, 0x00,         .Code=KC_SPACE}
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
#define KEY_RALT      {.Flags=FL_MOD, MOD_R_ALT,    .Code=KC_RALT}
#define KEY_RGUI      {.Flags=FL_MOD, MOD_R_GUI,    .Code=KC_RGUI}
#define KEY_BL        {.Flags=FL_BL,  0x00,         .Code=0x00}
#define KEY_RCTRL     {.Flags=FL_MOD, MOD_R_CTRL,   .Code=KC_RCTRL}



//Second layer
#define KEY_GRAVE     {.Flags=FL_STD, 0x00,         .Code=KC_GRAVE}
#define KEY_F1        {.Flags=FL_STD, 0x00,         .Code=KC_F1}
#define KEY_F2        {.Flags=FL_STD, 0x00,         .Code=KC_F2}
#define KEY_F3        {.Flags=FL_STD, 0x00,         .Code=KC_F3}
#define KEY_F4        {.Flags=FL_STD, 0x00,         .Code=KC_F4}
#define KEY_F5        {.Flags=FL_STD, 0x00,         .Code=KC_F5}
#define KEY_F6        {.Flags=FL_STD, 0x00,         .Code=KC_F6}
#define KEY_F7        {.Flags=FL_STD, 0x00,         .Code=KC_F7}
#define KEY_F8        {.Flags=FL_STD, 0x00,         .Code=KC_F8}
#define KEY_F9        {.Flags=FL_STD, 0x00,         .Code=KC_F9}
#define KEY_F10       {.Flags=FL_STD, 0x00,         .Code=KC_F10}
#define KEY_F11       {.Flags=FL_STD, 0x00,         .Code=KC_F11}
#define KEY_F12       {.Flags=FL_STD, 0x00,         .Code=KC_F12}
#define KEY_DELETE    {.Flags=FL_STD, 0x00,         .Code=KC_DELETE}

//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
#define KEY_PAGEUP     {.Flags=FL_STD, 0x00,         .Code=KC_PAGEUP}
#define KEY_UP         {.Flags=FL_STD, 0x00,         .Code=KC_UP}
#define KEY_PAGEDOWN   {.Flags=FL_STD, 0x00,         .Code=KC_PAGEDOWN}
#define KEY_PRINTSCR   {.Flags=FL_STD, 0x00,         .Code=KC_PRINTSCR}
#define KEY_SCROLLLOCK {.Flags=FL_STD, 0x00,         .Code=KC_SCROLLLOCK}
#define KEY_PAUSE      {.Flags=FL_STD, 0x00,         .Code=KC_PAUSE}
//#define KEY_NONE

//#define KEY_Fn
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
#define KEY_HOME       {.Flags=FL_STD, 0x00,         .Code=KC_HOME}
#define KEY_LEFT       {.Flags=FL_STD, 0x00,         .Code=KC_LEFT}
#define KEY_DOWN       {.Flags=FL_STD, 0x00,         .Code=KC_DOWN}
#define KEY_RIGHT      {.Flags=FL_STD, 0x00,         .Code=KC_RIGHT}
#define KEY_INSERT     {.Flags=FL_STD, 0x00,         .Code=KC_INSERT}
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE

#define KEY_CAPSLOCK   {.Flags=FL_STD, 0x00,         .Code=KC_CAPSLOCK}
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
#define KEY_END        {.Flags=FL_STD, 0x00,         .Code=KC_END}
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE
//#define KEY_NONE


//Backlight action keys
#define KEY_BL_BRIGHT_UP   {.Flags=FL_STD, 0x00,  .Action=GF_BRIGHTNESS_UP}
#define KEY_BL_BRIGHT_DOWN {.Flags=FL_STD, 0x00,  .Action=GF_BRIGHTNESS_DOWN}
#define KEY_BL_SPEED_UP    {.Flags=FL_STD, 0x00,  .Action=GF_SPEED_UP}
#define KEY_BL_SPEED_DOWN  {.Flags=FL_STD, 0x00,  .Action=GF_SPEED_DOWN}

#define KEY_BL_OFF         {.Flags=FL_STD, 0x00,  .Action=GF_OFF}
#define KEY_BL_ON          {.Flags=FL_STD, 0x00,  .Action=GF_ON}
#define KEY_BL_NEXT        {.Flags=FL_STD, 0x00,  .Action=GF_NEXT}

#define KEY_BL_RECORD_ON   {.Flags=FL_STD, 0x00,  .Action=GF_RECORD_ON}
#define KEY_BL_RECORD_END  {.Flags=FL_STD, 0x00,  .Action=GF_RECORD_END}
#endif //KEYS_H
