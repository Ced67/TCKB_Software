#include "FullOFF.h"

//---------------------------------------------------------------------------------
// ANIMATION: full OFF (fade out)
//---------------------------------------------------------------------------------
void _Anim_FullOFF(void) {
  uint8_t row = 0;
  uint8_t col = 0;

  //Check if the animation delay is elapsed:
  if( (HAL_GetTick() - _Last_Animation_Process_tick) < 15 ) {
    //Not yet.
    return;
  }
  //Save the last execution tick
  _Last_Animation_Process_tick = HAL_GetTick();

  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(col=0; col<NUM_COL; col++) {
      for(row=0; row<NUM_ROW; row++) {
        if(LedMatrix[row][col] > 0) {
          LedMatrix[row][col]--;
        }
      }
    }
  osMutexRelease(MtxLedMatrixHandle);
}






