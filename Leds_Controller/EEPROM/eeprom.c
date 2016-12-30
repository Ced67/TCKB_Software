#include "eeprom.h"
extern osMutexId MtxCRC32Handle; //To protect access to hardware CRC32 engine
extern CRC_HandleTypeDef hcrc;

#define EEP_base_addr         (0x08080000)
#define EEP_size              256 //(0x00001000)


#define EEP_CRC               (*((volatile uint32_t *)           (EEP_base_addr)))
#define EEP_Version           (*((volatile uint8_t *)            (EEP_base_addr + 0x04)))

#define EEP_BrightnessIndex   (*((volatile uint8_t *)            (EEP_base_addr + 0x08)))
#define EEP_AnimationSpeed    (*((volatile uint8_t *)            (EEP_base_addr + 0x0C)))
#define EEP_LedAnimationType  (*((volatile LedAnimationType_e *) (EEP_base_addr + 0x10)))
#define EEP_Flags             (*((volatile EEP_flags_t *)        (EEP_base_addr + 0x14)))


static uint32_t EEP_Calc_CRC(void) {
  uint32_t _crc = 0;
  osMutexWait(MtxCRC32Handle, osWaitForever);
    _crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)(EEP_base_addr +  4), (EEP_size/sizeof(uint32_t))-1);
  osMutexRelease(MtxCRC32Handle);

  return _crc;
}


static void _EEP_Start_Update(void) {
  //taskENTER_CRITICAL();
  HAL_FLASHEx_DATAEEPROM_Unlock();

  //Clean CRC
  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, (uint32_t)&EEP_CRC, 0);
}

static void _EEP_End_Update(void) {
  //Refresh CRC;
  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, (uint32_t)&EEP_CRC, EEP_Calc_CRC());

  HAL_FLASHEx_DATAEEPROM_Lock();
  //taskEXIT_CRITICAL();
}

static void EEP_Set_Default(void) {
  _EEP_Start_Update();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_Version,            TCKB_EEP_LEDS_STRUCT_VERSION);
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_BrightnessIndex,    (ANIMATION_BRIGHTNESS_STEPS - 1));

    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_AnimationSpeed,     (ANIMATION_SPEED_MIN + ANIMATION_SPEED_MAX)/2);
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, (uint32_t)&EEP_LedAnimationType,   0);
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_Flags,              0x01);
  _EEP_End_Update();
}


void EEP_Init(void) {
  //Check eeprom integrity
  if((TCKB_EEP_LEDS_STRUCT_VERSION != EEP_Version) || (EEP_Calc_CRC() != EEP_CRC)) {
    EEP_Set_Default();
  }

  return;
}




uint8_t Get_EEP_BrightnessIndex(void) {
  return EEP_BrightnessIndex;
}

void Set_EEP_BrightnessIndex(uint8_t val) {
  _EEP_Start_Update();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_BrightnessIndex, val);
  _EEP_End_Update();
}




uint8_t Get_EEP_AnimationSpeed(void) {
  return EEP_AnimationSpeed;
}

void Set_EEP_AnimationSpeed(uint8_t val) {
  _EEP_Start_Update();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_AnimationSpeed, val);
  _EEP_End_Update();
}




LedAnimationType_e Get_EEP_LedAnimationType(void) {
  return EEP_LedAnimationType;
}

void Set_EEP_LedAnimationType(LedAnimationType_e val) {
  EEP_flags_t tmp;
  tmp.All = EEP_Flags.All;

  tmp.FullON  = 0;
  tmp.FullOFF = 0;

  _EEP_Start_Update();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_Flags, tmp.All);
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, (uint32_t)&EEP_LedAnimationType, (uint32_t)val);
  _EEP_End_Update();
}




bool Get_EEP_Flags_FullON(void) {
  return EEP_Flags.FullON;
}

void Set_EEP_Flags_FullON(void) {
  EEP_flags_t tmp;
  tmp.All = EEP_Flags.All;

  tmp.FullON  = 1;
  tmp.FullOFF = 0;

  _EEP_Start_Update();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_Flags, tmp.All);
  _EEP_End_Update();
}




bool Get_EEP_Flags_FullOFF(void) {
  return EEP_Flags.FullOFF;
}

void Set_EEP_Flags_FullOFF(void) {
  EEP_flags_t tmp;
  tmp.All = EEP_Flags.All;

  tmp.FullON  = 0;
  tmp.FullOFF = 1;

  _EEP_Start_Update();
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, (uint32_t)&EEP_Flags, tmp.All);
  _EEP_End_Update();
}



