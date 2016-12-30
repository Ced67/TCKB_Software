#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "TCKBLedControllerTasksCommon.h"
#include "TCKB_Common_Constants.h"

extern osMutexId MtxLedMatrixHandle;

extern uint8_t LedMatrix[NUM_ROW][NUM_COL];
extern uint8_t LedMatrixTmp[NUM_ROW][NUM_COL]; //For background computing

//Debug parameters. -----------------------------------------------------
//Should be all commented for production build

//End of debug parameters. ----------------------------------------------


typedef enum {
#ifdef USE_PATTERN_MATRIX
  ANIM_SCROLL,
#endif
  ANIM_PULSE,             //ON when pressed then fade out
  ANIM_REVERSE_PULSE,     //OFF when pressed then fade in
  ANIM_THUNDER,           //Light column keys above the pressed one (with vertical gradient)
  ANIM_CONWAY,            //Conway's Game of Life
  ANIM_SNAKE,             //Moving snake
  //ANIM_BREATH,            //Fade in and out in loop
  //ANIM_RECORDED,          //Light previously recorded
  _ANIM_END_,
} LedAnimationType_e;

#endif //LED_MATRIX_H
