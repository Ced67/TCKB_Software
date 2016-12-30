#include "Snake.h"
#include "GraphicTask.h"

//---------------------------------------------------------------------------------
// ANIMATION: moving snake
//---------------------------------------------------------------------------------


void _Init_Anim_Snake(void) {
  int i=0;

  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    //Cleaning matrix
    memset(LedMatrix, 0x00, sizeof(LedMatrix));

    //Drawing initial snake
    for(i=0; i<ANIMATION_BRIGHTNESS_STEPS; i++) {
      LedMatrix[0][i] = Brightness_Steps[i];
    }
  osMutexRelease(MtxLedMatrixHandle);
}

void _Anim_Snake(void) {
  uint8_t last_value = 0;

  //Check if the animation delay is elapsed:
  if( (HAL_GetTick() - _Last_Animation_Process_tick) < 100 ) {
    //Not yet.
    return;
  }

  //Save the last execution tick
  _Last_Animation_Process_tick = HAL_GetTick();

  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    last_value = LedMatrix[NUM_ROW-1][NUM_COL-1];
    memmove(((uint8_t*)LedMatrix)+1, LedMatrix, sizeof(LedMatrix)-1);
    LedMatrix[0][0] = last_value;
  osMutexRelease(MtxLedMatrixHandle);
}
