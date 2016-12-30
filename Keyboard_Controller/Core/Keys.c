#include "Keys.h"


#define DEBOUNCE_MASK 0x07 //3ms = three 1 shifted to the left (0x07 MAX == 3ms)


void ReadKey(GPIO_TypeDef* Col_Port,
             uint16_t Col_Pin,
             Key_t* Key) {

  uint8_t previous_debounce = Key->Debounce;

  //Prepare shift register for the next reading
  Key->Debounce = (Key->Debounce << 1);

  if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Col_Port, Col_Pin)) {
    Key->Debounce |= 0x01; //Add 1 in the shift register
  }
  else {
    Key->Debounce &= 0xFE; //Add 0 in the shift register
  }

  //Newpress is a punctual event, not a state
  //if(current debounce match mask)                      && (previous debounce unmatch mask)
  if( (DEBOUNCE_MASK == (DEBOUNCE_MASK & Key->Debounce)) && (DEBOUNCE_MASK != (DEBOUNCE_MASK & previous_debounce))) {
    Key->Newpress = true;
  }
  else {
    Key->Newpress = false;
  }

  //If the shift register match the mask, the key is debounced (no zeros)
  Key->Pressed = (DEBOUNCE_MASK == (DEBOUNCE_MASK & Key->Debounce));
}



bool IsPressed(Key_t* Key) {
  return Key->Pressed;
}

bool IsNewpress(Key_t* Key) {
  return Key->Newpress;
}

uint8_t GetCode(Key_t* Key) {
  return Key->Code;
}


uint8_t ModValue(Key_t* Key) {
  return Key->ModValue;
}


bool IsFn(Key_t* Key) {
  return Key->Fn;
}


bool IsBl(Key_t* Key) {
  return Key->Bl;
}


bool  IsMod(Key_t* Key) {
  return Key->Modifier;
}
