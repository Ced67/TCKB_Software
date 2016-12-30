#include "TCKBLedMatrix.h"
#include "TCKBSerialProto.h"
#include "Mailboxes.h"
#include "eeprom.h"
#include <string.h> //memcpy()

#include "GraphicTask.h"

//Animations includes
#include "TCKBLedMatrix.h"

#include "Conway.h"
#include "NOP.h"
#include "FullON.h"
#include "FullOFF.h"
#include "Pulse.h"
#include "ReversePulse.h"
#include "Snake.h"
#include "Thunder.h"


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

uint32_t _Last_Animation_Process_tick = 0;

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


static void Set_Animation(LedAnimationType_e anim) {
  switch(anim) {
    case ANIM_PULSE:
      Current_Animation.Init            = _Init_Anim_NOP;
      Current_Animation.Anim            = _Anim_FullOFF;
      Current_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_Pulse;
      break;

    case ANIM_REVERSE_PULSE:
      Current_Animation.Init            = _Init_Anim_NOP;
      Current_Animation.Anim            = _Anim_FullON;
      Current_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_ReversePulse;
      break;

    case ANIM_THUNDER:
      Current_Animation.Init            = _Init_Anim_NOP;
      Current_Animation.Anim            = _Anim_FullOFF;
      Current_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_Thunder;
      break;

    case ANIM_CONWAY:
      Current_Animation.Init            = _Init_Anim_Conway;
      Current_Animation.Anim            = _Anim_Conway;
      Current_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
      break;

    case ANIM_SNAKE:
      Current_Animation.Init            = _Init_Anim_Snake;
      Current_Animation.Anim            = _Anim_Snake;
      Current_Animation.ProcessKeys6KRO = _ProcessKeys6KRO_NOP;
      break;

  //case ANIM_BREATH:
  //case ANIM_RECORDED:
    default:
      return;
  };

  Current_Animation.Init();
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

    case GF_RECORD_ON:
    case GF_RECORD_END:
    default:
      break;
  };
}

//---------------------------------------------------------------------------------
// Here is the main leds effect task, calling the appropriate animation or
// setting coming from keyboard controller (CommTask).
//---------------------------------------------------------------------------------

void __attribute__((optimize("O0"))) StartGraphicTask(void const * argument) {
  osEvent result;

  EEP_Init();

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

  while (1) {
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
    }

    Current_Animation.Anim();

    //Animation speed is pattern dependent. osDelay() is managed in each animation function.
    //osDelay(XX);
  }
}

