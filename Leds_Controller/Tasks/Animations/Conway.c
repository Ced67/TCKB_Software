#include "Conway.h"
#include "GraphicTask.h"
#include "Utils.h"

//---------------------------------------------------------------------------------
// ANIMATION: Conway's Game of Life
//---------------------------------------------------------------------------------


static bool Is_Empty = true;
static uint32_t _Last_Not_Empy_tick = 0;

void _Init_Anim_Conway(void) {
  uint8_t row=0;
  uint8_t col=0;

  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    //Cleaning matrix
    memset(LedMatrix, 0x00, sizeof(LedMatrix));

    //Splatter random cells on board
    for(row=0; row<NUM_ROW; row++) {
      for(col=0; col<NUM_COL; col++) {
        LedMatrix[row][col] = (Get_Random()%7)? 0 : Current_Brightness;
      }
    }
  osMutexRelease(MtxLedMatrixHandle);

  _Last_Not_Empy_tick = HAL_GetTick();
}

void _Anim_Conway(void) {
  uint8_t row=0;
  uint8_t col=0;

  int rm, rp, cm, cp, nsum;

  //Check if the animation delay is elapsed:
  if( (HAL_GetTick() - _Last_Animation_Process_tick) < 1000 ) {
    //Not yet.
    return;
  }

  //Save the last execution tick
  _Last_Animation_Process_tick = HAL_GetTick();

  osMutexWait(MtxLedMatrixHandle, osWaitForever);
    for(row=0; row<NUM_ROW; row++){
      for(col=0; col<NUM_COL; col++){
        LedMatrixTmp[row][col] = (LedMatrix[row][col])?1:0;
      }
    }
  osMutexRelease(MtxLedMatrixHandle);

  //To reset the playground if empty
  Is_Empty = true;

  // Inspired by: https://www.pdc.kth.se/education/tutorials/summer-school/mpi-exercises/mpi-lab-codes/game_of_life-serial.c/view
  for(row=0; row<NUM_ROW; row++){
    for(col=0; col<NUM_COL; col++){

      rm = (0 == row)           ? (NUM_ROW-1) : (row-1);
      rp = ((NUM_ROW-1) == row) ? 0           : (row+1);
      cm = (0 == col)           ? (NUM_COL-1) : (col-1);
      cp = ((NUM_COL-1) == col) ? 0           : (col+1);

      nsum =  LedMatrixTmp[rm][cm] + LedMatrixTmp[rm][col] + LedMatrixTmp[rm][cp]
           +  LedMatrixTmp[row][cm]                        + LedMatrixTmp[row][cp]
           +  LedMatrixTmp[rp][cm] + LedMatrixTmp[rp][col] + LedMatrixTmp[rp][cp];

      osMutexWait(MtxLedMatrixHandle, osWaitForever);
        switch(nsum){

          case 3:
            LedMatrix[row][col] = Current_Brightness;
            break;

          case 2:
            LedMatrix[row][col] = (LedMatrixTmp[row][col])?0:Current_Brightness;
            break;

          default:
            LedMatrix[row][col] = 0;
        }

        if(LedMatrix[row][col]) {
          Is_Empty = false;
        }
      osMutexRelease(MtxLedMatrixHandle);
    }
  }

  if(true == Is_Empty) {
    if( (HAL_GetTick() - _Last_Not_Empy_tick) > 3000 ) {
      //Playground is empty since a few seconds, it needs to be restarted
      _Init_Anim_Conway();
    }
  }
  else {
    _Last_Not_Empy_tick = HAL_GetTick();
  }
}
