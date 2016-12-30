#include "ReversePulse.h"
#include "GraphicTask.h"

//---------------------------------------------------------------------------------
// ANIMATION: OFF when pressed then fade in
//---------------------------------------------------------------------------------
void _ProcessKeys6KRO_ReversePulse(Pressed_Keys_6kro_t* ptr) {
  //Parse the pressed keys array:
  uint8_t i=0;
  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(i=0; i<(ptr->datas.keycount); i++) {
      LedMatrix[ptr->datas.position[i].row][ptr->datas.position[i].col] = 0;
    }
  osMutexRelease(MtxLedMatrixHandle);
  return;
}
