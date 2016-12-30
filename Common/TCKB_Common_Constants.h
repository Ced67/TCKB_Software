#ifndef TCKB_COMMON_CONSTANTS
#define TCKB_COMMON_CONSTANTS

#if defined(_PROJECT_LEDS_CONTROLLER)
  #include "stm32l1xx_hal.h"
#elif defined(_PROJECT_KEYBOARD_CONTROLLER)
  #include "stm32l0xx_hal.h"
#endif

#include "main.h"

#define LED_PWM_Max 1000
#define LED_PWM_Delta (LED_PWM_Max/14)

#define NUM_LAYER 3
//0 = Normal keys
//1 = Alt keys
//2 = Backlight control

#ifdef _PROJECT_KEYBOARD_CONTROLLER
/*
  #warning "SEULEMENT POUR DEBUG NUCLEO"
  #define NUM_ROW 2
  #define NUM_COL 5
*/
  #define NUM_ROW 5
  #define NUM_COL 14

  #define CAPS_LOCK_PWM_TIMER       htim2
  #define CAPS_LOCK_PWM_CHANNEL     TIM_CHANNEL_1
  #define CAPS_LOCK_PWM_CCR         CCR1
  extern TIM_HandleTypeDef          (CAPS_LOCK_PWM_TIMER);

  #define SCROLL_LOCK_PWM_TIMER     htim2
  #define SCROLL_LOCK_PWM_CHANNEL   TIM_CHANNEL_2
  #define SCROLL_LOCK_PWM_CCR       CCR2
  extern TIM_HandleTypeDef          (SCROLL_LOCK_PWM_TIMER);

  #define USE_CAPSLOCK_LED
  //#define USE_SCROLLOCK_LED
#endif


#ifdef _PROJECT_LEDS_CONTROLLER
  #define NUM_ROW 5
  #define NUM_COL 14

  #define LINE0_PWM_TIMER       htim9
  #define LINE0_PWM_CHANNEL     TIM_CHANNEL_1
  #define LINE0_PWM_CCR         CCR1
  extern TIM_HandleTypeDef      (LINE0_PWM_TIMER);

  #define LINE1_PWM_TIMER       htim9
  #define LINE1_PWM_CHANNEL     TIM_CHANNEL_2
  #define LINE1_PWM_CCR         CCR2
  extern TIM_HandleTypeDef      (LINE1_PWM_TIMER);

  #define LINE2_PWM_TIMER       htim2
  #define LINE2_PWM_CHANNEL     TIM_CHANNEL_1
  #define LINE2_PWM_CCR         CCR1
  extern TIM_HandleTypeDef      (LINE2_PWM_TIMER);

  #define LINE3_PWM_TIMER       htim3
  #define LINE3_PWM_CHANNEL     TIM_CHANNEL_1
  #define LINE3_PWM_CCR         CCR1
  extern TIM_HandleTypeDef      (LINE3_PWM_TIMER);

  #define LINE4_PWM_TIMER       htim3
  #define LINE4_PWM_CHANNEL     TIM_CHANNEL_2
  #define LINE4_PWM_CCR         CCR2
  extern TIM_HandleTypeDef      (LINE4_PWM_TIMER);
#endif

#define MATRIX_SCAN_DELAY_6KRO_Tk 1
#define MATRIX_SCAN_DELAY_NKRO_Tk 1

#define KRO6_REPORT_SEND_DELAY_Tk 10
#define NKRO_REPORT_SEND_DELAY_Tk 1

#define KEYCOUNT_6KRO 6
#define MODIFIERSCOUNT_6KRO 8
#define COMMTASK_PRESSED_6KRO_KEYCOUNT (KEYCOUNT_6KRO + MODIFIERSCOUNT_6KRO)

#define COMMTASK_PRESSED_KEYS_MESSAGE_DELAY_tk 100
#define COMMTASK_PRESSED_KEYS_COUNT_MAX (NUM_ROW*NUM_COL)

#define ANIMATION_SPEED_MIN 5 //longest delay
#define ANIMATION_SPEED_MAX 1 //shortest delay

#define ANIMATION_BRIGHTNESS_MAX    100 //PWM
#define ANIMATION_BRIGHTNESS_STEPS  7

/*
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })
*/

#endif //TCKB_COMMON_CONSTANTS
