#include "KeyboardControllerTasksCommon.h"
#include "Matrix.h"
#include "HID_Report.h"

void StartScanTask(void const * argument)
{
  Report_Init();
  MatrixInit();

  while(1){
    switch(Report_GetMode()) {
    case MODE_6KRO:
      osDelay(MATRIX_SCAN_DELAY_6KRO_Tk);
      break;

    case MODE_NKRO:
      osDelay(MATRIX_SCAN_DELAY_NKRO_Tk);
      break;
    }

    MatrixScanKeys();
    Report_Send();
  }
}
