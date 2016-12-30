#include <string.h>
#include "TCKBSerialProto.h"

uint8_t SP_Buffer[sizeof(SP_Messages_u)];
SP_Messages_u* SP_Message = (SP_Messages_u*)SP_Buffer;

/////////////////////////////////////////////////////////////
// Overwrite the HAL SPI Callbacks
// WARNING: These functions are called by interrupts!

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);  //Disable SPI Chip Select
}
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// Overwrite the HAL EXTI Callbacks
// WARNING: These functions are called by interrupts!

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if(SPI_CS_Pin == GPIO_Pin) {
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SPI_CS_GPIO_Port, SPI_CS_Pin)) {
      osSignalSet(CommTaskHandle, EVENT_SPI_CS_DOWN);
    }
    else {
      osSignalSet(CommTaskHandle, EVENT_SPI_CS_UP);
    }
  }
}
/////////////////////////////////////////////////////////////




//Produce checksum of target ptr
static uint8_t _SP_Calc_Checksum(uint8_t* Ptr, uint8_t Size) {
  uint8_t cs = 0;

  do {
    cs += Ptr[--Size];
  }while(Size > 0);

  return cs;
}


//Sends the current frame in the buffer
static void _SP_Send_Frame(uint16_t Size) {
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);  //Enable SPI Chip Select
  osDelay(2);
  HAL_SPI_Transmit_DMA(&SPI_HANDLER, SP_Buffer, Size);
  //SPI Chip Select will be disabled in HAL_SPI_TxCpltCallback(); when DMA completes.
}



//Master interface ---------------------------------------------
void SP_Master_Init(void) {
  HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);  //Disable SPI Chip Select
  HAL_SPI_DMAStop(&SPI_HANDLER);
}


void SP_Send_Pressed_Keys_6kro(Pressed_Keys_6kro_t* Ptr) {
  //Header
  SP_Message->Header.type = SET_PRESSED_KEYS_6KRO;
  SP_Message->Header.len  = sizeof(Pressed_Keys_6kro_t) - sizeof(SP_Frame_Header_t) - 1;

  //Content
  memcpy(&(SP_Message->Pressed_Keys_6kro.datas), &(Ptr->datas), SP_Message->Header.len);

  //Checksum
  SP_Message->Pressed_Keys_6kro.checksum = _SP_Calc_Checksum(SP_Buffer, sizeof(Pressed_Keys_6kro_t) -1);

  _SP_Send_Frame(sizeof(Pressed_Keys_6kro_t));
}


void SP_Send_Set_Led_Mode(Led_Action_e Action) {
  //Header
  SP_Message->Header.type = SET_LED_MODE;
  SP_Message->Header.len  = sizeof(Set_Led_Mode_t) - sizeof(SP_Frame_Header_t) - 1;

  //Content
  SP_Message->Set_Led_Mode.action = Action;

  //Checksum
  SP_Message->Set_Led_Mode.checksum = _SP_Calc_Checksum(SP_Buffer, sizeof(Set_Led_Mode_t) -1);

  _SP_Send_Frame(sizeof(Set_Led_Mode_t));
}



//Slave interface ----------------------------------------------
void SP_Slave_Init(void) {
  HAL_SPI_DMAStop(&SPI_HANDLER);
}


void SP_Listen_Frame(void) {
  memset(SP_Buffer, 0xFF, sizeof(SP_Buffer)); //Clean the buffer before listening to avoid process old datas after reception.
  HAL_SPI_Receive_DMA(&SPI_HANDLER, SP_Buffer, sizeof(SP_Buffer));
}


bool SP_Verify_Frame(void) {
  uint8_t _local_checksum = 0;
  uint8_t _frame_checksum = 0;

  HAL_SPI_DMAStop(&SPI_HANDLER);

  //Control if frame type is known
  if(SP_Message->Header.type >= _FRAME_TYPE_END_OF_ENUM) {
    //Unknown frame type.
    return false;
  }

  //Control if length field in header is incoherent:
  if(SP_Message->Header.len > (sizeof(SP_Buffer) - sizeof(SP_Frame_Header_t) - 1)) {
    //length exceed max.
    return false;
  }

  //Control received frame checksum:
  _frame_checksum = SP_Buffer[(sizeof(SP_Frame_Header_t) + SP_Message->Header.len)];
  _local_checksum = _SP_Calc_Checksum(SP_Buffer, sizeof(SP_Frame_Header_t) + SP_Message->Header.len);
  if(_local_checksum != _frame_checksum) {
    //Bad checksum
    return false;
  }

  //Frame is correct.
  return true;
}

