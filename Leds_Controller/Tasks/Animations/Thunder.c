#include "Thunder.h"
#include "GraphicTask.h"

//---------------------------------------------------------------------------------
// ANIMATION: Illuminate column
//---------------------------------------------------------------------------------
void _ProcessKeys6KRO_Thunder(Pressed_Keys_6kro_t* ptr) {
  //Parse the pressed keys array:
  uint8_t i=0;
  uint8_t j=0;

  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(i=0; i<(ptr->datas.keycount); i++) {
      for(j=0; j<NUM_ROW; j++) {
        LedMatrix[j][ptr->datas.position[i].col] = Current_Brightness;
      }
    }
  osMutexRelease(MtxLedMatrixHandle);
  return;
}
