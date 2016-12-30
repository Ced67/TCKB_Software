#include "SimpleLeftScroll.h"

//---------------------------------------------------------------------------------
// ANIMATION: Simple matrix left scroll (for debug purposes)
//---------------------------------------------------------------------------------
void _Init_Anim_SimpleLeftScroll(void) {
  //memcpy(LedMatrix, LedMatrix_Test, sizeof(LedMatrix_Test));
}


void _Anim_SimpleLeftScroll(void) {
  uint8_t Col0[NUM_ROW];
  uint8_t row = 0;
  uint8_t col = 0;

  //Check if the animation delay is elapsed:
  if( (HAL_GetTick() - _Last_Animation_Process_tick) < (25 + (50* Animation_Speed)) ) {
    //Not yet.
    return;
  }
  //Save the last execution tick
  _Last_Animation_Process_tick = HAL_GetTick();

  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(row=0; row<NUM_ROW; row++) {
      Col0[row] = LedMatrix[row][0];
    }
  osMutexRelease(MtxLedMatrixHandle);


  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(col=0; col<(NUM_COL-1); col++) {
      for(row=0; row<NUM_ROW; row++) {
        LedMatrix[row][col] = LedMatrix[row][col+1];
      }
    }
  osMutexRelease(MtxLedMatrixHandle);


  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(row=0; row<NUM_ROW; row++) {
      LedMatrix[row][NUM_COL-1] = Col0[row];
    }
  osMutexRelease(MtxLedMatrixHandle);
}


void _ProcessKeys6KRO_SimpleLeftScroll(Pressed_Keys_6kro_t* ptr) {
  //nothing_to_do();
  return;
}
