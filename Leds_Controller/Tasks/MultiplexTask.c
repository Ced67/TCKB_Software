#include "TCKBLedMatrix.h"
#include <string.h> //memset()

//Empty matrix with leds off at startup.
uint8_t LedMatrix[NUM_ROW][NUM_COL] = {
  {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
};

uint8_t LedMatrixTmp[NUM_ROW][NUM_COL];

typedef struct {
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin;
} Column_Desc_t;


const Column_Desc_t Column_Desc[NUM_COL] = {
  {Col_0_GPIO_Port,   Col_0_Pin},
  {Col_1_GPIO_Port,   Col_1_Pin},
  {Col_2_GPIO_Port,   Col_2_Pin},
  {Col_3_GPIO_Port,   Col_3_Pin},
  {Col_4_GPIO_Port,   Col_4_Pin},
  {Col_5_GPIO_Port,   Col_5_Pin},
  {Col_6_GPIO_Port,   Col_6_Pin},
  {Col_7_GPIO_Port,   Col_7_Pin},
  {Col_8_GPIO_Port,   Col_8_Pin},
  {Col_9_GPIO_Port,   Col_9_Pin},
  {Col_10_GPIO_Port,  Col_10_Pin},
  {Col_11_GPIO_Port,  Col_11_Pin},
  {Col_12_GPIO_Port,  Col_12_Pin},
  {Col_13_GPIO_Port,  Col_13_Pin},
};


void __attribute__((optimize("O0"))) StartMultiplexTask(void const * argument) {
  uint8_t col = 0;

  //Lines are PWM outputs
  HAL_TIM_PWM_Start(&(LINE0_PWM_TIMER), LINE0_PWM_CHANNEL);
  HAL_TIM_PWM_Start(&(LINE1_PWM_TIMER), LINE1_PWM_CHANNEL);
  HAL_TIM_PWM_Start(&(LINE2_PWM_TIMER), LINE2_PWM_CHANNEL);
  HAL_TIM_PWM_Start(&(LINE3_PWM_TIMER), LINE3_PWM_CHANNEL);
  HAL_TIM_PWM_Start(&(LINE4_PWM_TIMER), LINE4_PWM_CHANNEL);

  //GPIO_PIN_SET   = Led off
  //GPIO_PIN_RESET = Led on
  for(col=0; col<NUM_COL; col++) {
    HAL_GPIO_WritePin(Column_Desc[col].GPIOx , Column_Desc[col].GPIO_Pin, GPIO_PIN_SET);
  }

  osDelay(500);

  // Infinite loop. This task needs to run until poweroff
  while(1) {
    for(col=0; col<NUM_COL; col++) {
      //Column ON
      HAL_GPIO_WritePin(Column_Desc[col].GPIOx , Column_Desc[col].GPIO_Pin, GPIO_PIN_RESET);

      //Set PWM values for column leds
      osMutexWait(MtxLedMatrixHandle, osWaitForever);
        LINE0_PWM_TIMER.Instance->LINE0_PWM_CCR = LedMatrix[0][col];
        LINE1_PWM_TIMER.Instance->LINE1_PWM_CCR = LedMatrix[1][col];
        LINE2_PWM_TIMER.Instance->LINE2_PWM_CCR = LedMatrix[2][col];
        LINE3_PWM_TIMER.Instance->LINE3_PWM_CCR = LedMatrix[3][col];
        LINE4_PWM_TIMER.Instance->LINE4_PWM_CCR = LedMatrix[4][col];
      osMutexRelease(MtxLedMatrixHandle);

      osDelay(1);

      //Set PWM off to avoid ghost lights between columns
      LINE0_PWM_TIMER.Instance->LINE0_PWM_CCR = 0;
      LINE0_PWM_TIMER.Instance->CNT = LINE0_PWM_TIMER.Instance->ARR;

      LINE1_PWM_TIMER.Instance->LINE1_PWM_CCR = 0;
      LINE1_PWM_TIMER.Instance->CNT = LINE1_PWM_TIMER.Instance->ARR;

      LINE2_PWM_TIMER.Instance->LINE2_PWM_CCR = 0;
      LINE2_PWM_TIMER.Instance->CNT = LINE2_PWM_TIMER.Instance->ARR;

      LINE3_PWM_TIMER.Instance->LINE3_PWM_CCR = 0;
      LINE3_PWM_TIMER.Instance->CNT = LINE3_PWM_TIMER.Instance->ARR;

      LINE4_PWM_TIMER.Instance->LINE4_PWM_CCR = 0;
      LINE4_PWM_TIMER.Instance->CNT = LINE4_PWM_TIMER.Instance->ARR;

      //Column OFF
      HAL_GPIO_WritePin(Column_Desc[col].GPIOx , Column_Desc[col].GPIO_Pin, GPIO_PIN_SET);
    }
  };
}
