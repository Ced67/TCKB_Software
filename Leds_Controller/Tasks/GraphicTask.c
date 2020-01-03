#include "TCKBLedMatrix.h"
#include "TCKBSerialProto.h"
#include "Mailboxes.h"
#include "eeprom.h"
#include <string.h> //memcpy()

#include "GraphicTask.h"

//Animations includes
#include "TCKBLedMatrix.h"

#include "Breath.h"
#include "Conway.h"
#include "NOP.h"
#include "FullON.h"
#include "FullOFF.h"
#include "Pulse.h"
#include "ReversePulse.h"
#include "Snake.h"
#include "Thunder.h"

typedef enum {
  CURRENT = 0,
  CURRENT_TO_IDLE,
  IDLE,
  IDLE_TO_CURRENT
} Anim_State;

static Anim_State state = CURRENT;


const uint8_t Brightness_Steps[ANIMATION_BRIGHTNESS_STEPS] = {
  1 * (ANIMATION_BRIGHTNESS_MAX/(ANIMATION_BRIGHTNESS_STEPS-1)), //14,
  2 * (ANIMATION_BRIGHTNESS_MAX/(ANIMATION_BRIGHTNESS_STEPS-1)), //21,
  3 * (ANIMATION_BRIGHTNESS_MAX/(ANIMATION_BRIGHTNESS_STEPS-1)), //28,
  4 * (ANIMATION_BRIGHTNESS_MAX/(ANIMATION_BRIGHTNESS_STEPS-1)), //42,
  5 * (ANIMATION_BRIGHTNESS_MAX/(ANIMATION_BRIGHTNESS_STEPS-1)), //56,
  6 * (ANIMATION_BRIGHTNESS_MAX/(ANIMATION_BRIGHTNESS_STEPS-1)), //70,
  ANIMATION_BRIGHTNESS_MAX
};

LedAnimationType_e LedAnimationType = (LedAnimationType_e)0;

Animation_t Current_Animation;
Animation_t Idle_Animation;

uint32_t _Last_Animation_Process_tick = 0;
uint32_t _Last_Idle_tick = 0;
uint8_t  _Allow_Idle_Anim = 0;

uint8_t Current_Brightness = 0;



static void Set_Brightness_UP(void) {
  if (Get_EEP_BrightnessIndex() < ANIMATION_BRIGHTNESS_STEPS - 1) {
    Set_EEP_BrightnessIndex(Get_EEP_BrightnessIndex() + 1);
    Current_Brightness = Brightness_Steps[Get_EEP_BrightnessIndex()];
  }
}

static void Set_Brightness_DOWN(void) {
  if (Get_EEP_BrightnessIndex() > 0) {
    Set_EEP_BrightnessIndex(Get_EEP_BrightnessIndex() - 1);
    Current_Brightness = Brightness_Steps[Get_EEP_BrightnessIndex()];
  }
}


static void Set_Animation_Speed_UP(void) {
  if (Get_EEP_AnimationSpeed() > ANIMATION_SPEED_MAX) {
    Set_EEP_AnimationSpeed(Get_EEP_AnimationSpeed() - 1);
  }
}


static void Set_Animation_Speed_DOWN(void) {
  if (Get_EEP_AnimationSpeed() < ANIMATION_SPEED_MIN) {
    Set_EEP_AnimationSpeed(Get_EEP_AnimationSpeed() +- 1);
  }
}


static void Set_Animation_ptr(Animation_t* ptr, LedAnimationType_e anim) {
  switch(anim) {
    case ANIM_PULSE:
      ptr->Init            = _Init_Anim_NOP;
      ptr->Anim            = _Anim_FullOFF;
      ptr->ProcessKeys6KRO = _ProcessKeys6KRO_Pulse;
      break;

    case ANIM_REVERSE_PULSE:
      ptr->Init            = _Init_Anim_NOP;
      ptr->Anim            = _Anim_FullON;
      ptr->ProcessKeys6KRO = _ProcessKeys6KRO_ReversePulse;
      break;

    case ANIM_THUNDER:
      ptr->Init            = _Init_Anim_NOP;
      ptr->Anim            = _Anim_FullOFF;
      ptr->ProcessKeys6KRO = _ProcessKeys6KRO_Thunder;
      break;

    case ANIM_CONWAY:
      ptr->Init            = _Init_Anim_Conway;
      ptr->Anim            = _Anim_Conway;
      ptr->ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
      break;

    case ANIM_SNAKE:
      ptr->Init            = _Init_Anim_Snake;
      ptr->Anim            = _Anim_Snake;
      ptr->ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
      break;

    case ANIM_BREATH:
      ptr->Init            = _Init_Anim_Breath;
      ptr->Anim            = _Anim_Breath;
      ptr->ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
      break;

  //case ANIM_RECORDED:
    default:
      return;
  };
}

static void Set_Animation(LedAnimationType_e anim) {
  Set_Animation_ptr(&Current_Animation, anim);
  Current_Animation.Init();
}

static void Set_Idle(LedAnimationType_e anim) {
  Set_Animation_ptr(&Idle_Animation, anim);
  Idle_Animation.Init();
}


static void Set_Next_Animation(void) {
  if ((Get_EEP_LedAnimationType() + 1) >= _ANIM_END_) {
    Set_EEP_LedAnimationType((LedAnimationType_e) 0);
  } else {
    Set_EEP_LedAnimationType(Get_EEP_LedAnimationType() + 1);
  }

  Set_Animation(Get_EEP_LedAnimationType());
}


static void Process_Led_Action(Led_Action_e action) {
  switch (action) {
    case GF_BRIGHTNESS_UP:
      Set_Brightness_UP();
      break;

    case GF_BRIGHTNESS_DOWN:
      Set_Brightness_DOWN();
      break;

    case GF_SPEED_UP:
      Set_Animation_Speed_UP();
      break;

    case GF_SPEED_DOWN:
      Set_Animation_Speed_DOWN();
      break;

    case GF_OFF:
      Set_EEP_Flags_FullOFF();
      Current_Animation.Init = _Init_Anim_NOP;
      Current_Animation.Anim = _Anim_FullOFF;
      Current_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
      Current_Animation.Init();
      break;

    case GF_ON:
      Set_EEP_Flags_FullON();
      Current_Animation.Init = _Init_Anim_NOP;
      Current_Animation.Anim = _Anim_FullON;
      Current_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
      Current_Animation.Init();
      break;

    case GF_NEXT:
      Set_Next_Animation();
      break;

    case GF_SAVE_AS_IDLE:
      Set_EEP_IdleAnimationType(Get_EEP_LedAnimationType());
      break;

    case GF_RECORD_ON:
    case GF_RECORD_END:
    default:
      break;
  };
}


void InitIdle(void) {
  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(DIS_BKLIGHT_GPIO_Port, DIS_BKLIGHT_Pin)) {
    //Idle animation disabled
    _Allow_Idle_Anim = 0;

    //Disable pulldown on pin DIS_BKLIGHT_Pin to prevent current draw
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin  = DIS_BKLIGHT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DIS_BKLIGHT_GPIO_Port, &GPIO_InitStruct);
  }
  else {
    _Allow_Idle_Anim = 1;
  }
}


//---------------------------------------------------------------------------------
// Here is the main leds effect task, calling the appropriate animation or
// setting coming from keyboard controller (CommTask).
//---------------------------------------------------------------------------------

void __attribute__((optimize("O0"))) StartGraphicTask(void const * argument) {
  osEvent result;
  uint8_t message_received = 0;

  EEP_Init();
  InitIdle();

  osDelay(1500);

  Current_Brightness = Brightness_Steps[Get_EEP_BrightnessIndex()];

  if(Get_EEP_Flags_FullON()) {
    Process_Led_Action(GF_ON);
  }
  else if(Get_EEP_Flags_FullOFF()) {
    Process_Led_Action(GF_OFF);
  }
  else {
    Set_Animation(Get_EEP_LedAnimationType());
  }

//  Idle_Animation.Init            = _Init_Anim_Breath;
//  Idle_Animation.Anim            = _Anim_Breath;
//  Idle_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
  _Last_Idle_tick = HAL_GetTick();

  while (1) {
    message_received = 0;

    //Is there a message?
    result = osMailGet(SERIAL_Mailbox, 0);
    if (osEventMail == result.status) {
      SP_Messages_u* ptr = (SP_Messages_u*) result.value.p;

      switch (ptr->Header.type) {
        case SET_LED_MODE:
          Process_Led_Action(ptr->Set_Led_Mode.action);
          break;

        case SET_PRESSED_KEYS_6KRO:
          Current_Animation.ProcessKeys6KRO(&(ptr->Pressed_Keys_6kro));
          break;

        case SET_PRESSED_KEYS_NKRO:
          //
          break;

        default:
          break;
      }

      osMailFree(SERIAL_Mailbox, ptr);

      //There was some action on the keys --> back to the curent anim
      message_received = 1;
      _Last_Idle_tick = HAL_GetTick(); //idle timeout reset
    }


    switch(state) {
      case CURRENT:
        Current_Animation.Anim();

        //Check if the idle timeout is elapsed:
        if ((1 == _Allow_Idle_Anim) && ((HAL_GetTick() - _Last_Idle_tick) > IDLE_TIMEOUT_ms)) {
          state = CURRENT_TO_IDLE;
        }
        break;

      case CURRENT_TO_IDLE:
        Set_Idle(Get_EEP_IdleAnimationType());
        state = IDLE;
        break;

      case IDLE:
        Idle_Animation.Anim();
        if(1 == message_received) {
          state = IDLE_TO_CURRENT;
        }
        break;

      case IDLE_TO_CURRENT:
        Set_Animation(Get_EEP_LedAnimationType());
        state = CURRENT;
        break;
    }

    //Animation speed is pattern dependent. osDelay() is managed in each animation function.
    //osDelay(XX);
  }
}
