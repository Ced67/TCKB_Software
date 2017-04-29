#include "Utils.h"

extern osMutexId MtxCRC32Handle; //To protect access to hardware CRC32 engine
extern CRC_HandleTypeDef hcrc;

static uint8_t STM32_UUID_BIN[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t STM32_UUID_STR[]   = "000000000000000000000000";


////////////////////////////////////////////////////////////////////////
// https://opensource.apple.com/source/CommonCrypto/CommonCrypto-55010/LocalTests/XTSTest/hexString.c
const char byteMap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
const int byteMapLen = sizeof(byteMap);

// Utility function to convert nibbles (4 bit values) into a hex character representation */
static char nibbleToChar(uint8_t nibble)
{
  if(nibble < byteMapLen) return byteMap[nibble];
  return '*';
}
////////////////////////////////////////////////////////////////////////

static uint32_t _rnd = 0;

uint32_t Get_Random(void) {
  _rnd ^= HAL_GetTick();

  osMutexWait(MtxCRC32Handle, osWaitForever);
    _rnd = HAL_CRC_Calculate(&hcrc, &_rnd, 1);
  osMutexRelease(MtxCRC32Handle);

  return _rnd;
}

static void Init_Random(void) {
  ((uint8_t*)_rnd)[0] = STM32_UUID_BIN[8];
  ((uint8_t*)_rnd)[1] = STM32_UUID_BIN[9];
  ((uint8_t*)_rnd)[2] = STM32_UUID_BIN[10];
  ((uint8_t*)_rnd)[3] = STM32_UUID_BIN[11];
}



uint8_t* Get_CPUID_str(void) {
  return STM32_UUID_STR;
}

static void Init_UID(void) {
  //Reading binary CPUID
  #if defined(STM32L053xx) || defined(STM32L100xB) || defined(STM32L100x8)
    uint8_t* uid_base = (uint8_t*)UID_BASE;
    STM32_UUID_BIN[0]   = uid_base[0x00];
    STM32_UUID_BIN[1]   = uid_base[0x01];
    STM32_UUID_BIN[2]   = uid_base[0x02];
    STM32_UUID_BIN[3]   = uid_base[0x03];

    STM32_UUID_BIN[4]   = uid_base[0x04];
    STM32_UUID_BIN[5]   = uid_base[0x05];
    STM32_UUID_BIN[6]   = uid_base[0x06];
    STM32_UUID_BIN[7]   = uid_base[0x07];

    STM32_UUID_BIN[8]   = uid_base[0x14];
    STM32_UUID_BIN[9]   = uid_base[0x15];
    STM32_UUID_BIN[10]  = uid_base[0x16];
    STM32_UUID_BIN[11]  = uid_base[0x17];

  #else
    #error "UNDEFINED TARGET"
  #endif

  //Converting to string
  int i;
  for(i=0; i<sizeof(STM32_UUID_BIN); i++) {
    STM32_UUID_STR[i*2]   = nibbleToChar(STM32_UUID_BIN[i] >> 4);
    STM32_UUID_STR[i*2+1] = nibbleToChar(STM32_UUID_BIN[i] & 0x0f);
  }

  return;
}

void Utils_Init(void) {
  Init_UID();
  Init_Random();
}
