#include "Pulse.h"
#include "GraphicTask.h"

//---------------------------------------------------------------------------------
// ANIMATION: ON when pressed then fade out
//---------------------------------------------------------------------------------
void _ProcessKeys6KRO_Pulse(Pressed_Keys_6kro_t* ptr) {
  //Parse the pressed keys array:
  uint8_t i=0;
  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(i=0; i<(ptr->datas.keycount); i++) {
      LedMatrix[ptr->datas.position[i].row][ptr->datas.position[i].col] = Current_Brightness;
    }
  osMutexRelease(MtxLedMatrixHandle);
  return;
}
