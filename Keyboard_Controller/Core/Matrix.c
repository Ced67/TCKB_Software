#include "Matrix.h"
#include "HID_Report.h"
#include "Mailboxes.h"

static uint8_t layer = 0;

static SP_Messages_u _Message;
static uint32_t      _last_send_pressed_keys_tick = 0;
static bool          _send_pressed_keys = false;
static bool          _fn_already_pressed = false; //if multiples fn keys are defined, only one must have effect during the scan
static bool          _bl_already_pressed = false; //if multiples bl keys are defined, only one must have effect during the scan

static void _MatrixComm_AddPressedKey_6kro(uint8_t row, uint8_t col) {
  //Check if the key is already registered
  uint8_t i = 0;
  bool add_to_buffer = true;

  for(i=0; i<_Message.Pressed_Keys_6kro.datas.keycount; i++) {
    if((_Message.Pressed_Keys_6kro.datas.position[i].row == row) && (_Message.Pressed_Keys_6kro.datas.position[i].col == col)) {
      add_to_buffer = false;
      break;
    }
  }

  if(true == add_to_buffer) {
    //Check if the buffer is full
    if(_Message.Pressed_Keys_6kro.datas.keycount < COMMTASK_PRESSED_6KRO_KEYCOUNT) {
      _Message.Pressed_Keys_6kro.datas.position[_Message.Pressed_Keys_6kro.datas.keycount].row = row;
      _Message.Pressed_Keys_6kro.datas.position[_Message.Pressed_Keys_6kro.datas.keycount].col = col;
      _Message.Pressed_Keys_6kro.datas.keycount++;
    }
    _send_pressed_keys = true;
  }
}


static void _MatrixComm_SendPressedKey_6kro(void) {
  //Is there a message to send
  if(_send_pressed_keys) {
    //Is it time to send the message?
    if((HAL_GetTick() - _last_send_pressed_keys_tick) > COMMTASK_PRESSED_KEYS_MESSAGE_DELAY_tk) {
      SP_Messages_u* ptr = (SP_Messages_u*) osMailAlloc(SERIAL_Mailbox, osWaitForever);
      if(NULL != ptr) {
        _Message.Header.type = SET_PRESSED_KEYS_6KRO;

        //Pool allocation successful
        memcpy(ptr, &_Message, sizeof(SP_Messages_u));
        osMailPut(SERIAL_Mailbox, ptr);

        //Reset local message
        _last_send_pressed_keys_tick = HAL_GetTick();
        memset(&_Message, 0x00, sizeof(SP_Messages_u));
      }

      _send_pressed_keys = false;
    }
  }
}


static void _MatrixComm_Send_Led_Mode(Led_Action_e mode) {
  SP_Messages_u* ptr = (SP_Messages_u*) osMailAlloc(SERIAL_Mailbox, osWaitForever);
  if(NULL != ptr) {
    _Message.Header.type = SET_LED_MODE;
    _Message.Set_Led_Mode.action = mode;

    //Pool allocation successful
    memcpy(ptr, &_Message, sizeof(SP_Messages_u));
    osMailPut(SERIAL_Mailbox, ptr);

    memset(&_Message, 0x00, sizeof(SP_Messages_u));
  }
}


void MatrixInit(void) {
  InitLayout();

  memset(&_Message, 0x00, sizeof(SP_Messages_u));
  _last_send_pressed_keys_tick = HAL_GetTick();
}


//GCC optimization produce some nasty shadow between adjacent keys.
//We may keep this function as straightforward as possible by disabling those optimizations with "__attribute__((optimize("O0")))"
void __attribute__((optimize("O0"))) MatrixScanKeys(void) {
  uint8_t row = 0;
  uint8_t col = 0;
  Key_t*  key = NULL;

  for(row=0; row<NUM_ROW; row++) {
    HAL_GPIO_WritePin((GPIO_TypeDef*)_Row_Ports[row], (uint16_t)_Row_Pins[row], GPIO_PIN_SET);   //Enable row

    for(col=0; col<NUM_COL; col++) {
      key = &(Matrix[layer][row][col]);
      ReadKey((GPIO_TypeDef*)_Col_Ports[col], (uint16_t)_Col_Pins[col], key);

      //Fn Key?
      if(IsFn(key)) {
        if(IsPressed(key) && (false == _fn_already_pressed)) {
          //Fn key newly pressed?
          _fn_already_pressed = true;
          layer = LAYOUT_FUNCTION_LAYER;
        }
        else if(!IsPressed(key) && (true == _fn_already_pressed)) {
          //Fn key newly released?
          _fn_already_pressed = false;
          layer = LAYOUT_DEFAULT_LAYER;
        }
      }

      //BL Key?
      else if(IsBl(key)) {
        if(IsPressed(key) && (false == _bl_already_pressed)) {
          //Bl key newly pressed?
          _bl_already_pressed = true;
          layer = LAYOUT_BACKLIGHT_LAYER;
        }
        else if(!IsPressed(key) && (true == _bl_already_pressed)) {
          //Bl key newly released?
          _bl_already_pressed = false;
          layer = LAYOUT_DEFAULT_LAYER;
        }
      }

      if(LAYOUT_BACKLIGHT_LAYER != layer) {
        //Modifier?
        /*else*/ if(IsMod(key)) {
          if(IsPressed(key)) {
            Report_AddModifier(key);
          }
        }

        //Normal key..
        else {
          if(IsPressed(key)) {
            Report_AddKey(key);
          }
        }

        //Send pressed keys event to backlight:
        if(IsPressed(key)) {
          switch(Report_GetMode()) {
          case MODE_6KRO:
            _MatrixComm_AddPressedKey_6kro(row, col);
            break;

          case MODE_NKRO:
            //TODO: _MatrixComm_AddPressedKey_Nkro(row, col);
            break;
          }
        }

        //Send pressed keys into spi
        switch(Report_GetMode()) {
        case MODE_6KRO:
          _MatrixComm_SendPressedKey_6kro();
          break;

        case MODE_NKRO:
          //TODO: _MatrixComm_AddPressedKey_Nkro(row, col);
          break;
        }

        if(Report_IsFull()) {
          //No need to continue the matrix scan
          break;
        }
      }
      else {
        //Sends backlight control command (no usb key events)
        if (false == IsBl(key)) {
          if(IsNewpress(key)) {
            if(GF_NO_ACTION != key->Action) {
              _MatrixComm_Send_Led_Mode(key->Action);
            }
          }
        }
      }

    } //for (col)

    HAL_GPIO_WritePin((GPIO_TypeDef*)_Row_Ports[row], (uint16_t)_Row_Pins[row], GPIO_PIN_RESET); //Disable row

  } //for (row)

}
