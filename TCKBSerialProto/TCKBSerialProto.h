#ifndef TCKB_SERIAL_PROTO_H
#define TCKB_SERIAL_PROTO_H

#include <stdbool.h>
#include "TCKB_Common_Constants.h"

#if defined(_PROJECT_LEDS_CONTROLLER)
  #include "stm32l1xx_hal.h"
#elif defined(_PROJECT_KEYBOARD_CONTROLLER)
  #include "stm32l0xx_hal.h"
#endif

#include "cmsis_os.h"
#include "FreeRTOSConfig.h"

osThreadId CommTaskHandle;
extern SPI_HandleTypeDef hspi1;
#define SPI_HANDLER hspi1

//These are the event code for the slave SPI_CS edge interrupt
#define EVENT_SPI_CS_DOWN 0x00000001
#define EVENT_SPI_CS_UP   0x00000002


//These are the frame identifiers
typedef enum {
  SET_LED_MODE = 0,
  SET_PRESSED_KEYS_6KRO,
  SET_PRESSED_KEYS_NKRO,

  _FRAME_TYPE_END_OF_ENUM
} SP_Frame_Type_e;


//Definition of the frame header
typedef struct {
  SP_Frame_Type_e type;
  uint8_t         len;
} __attribute__((packed)) SP_Frame_Header_t;




//Select mode ------------------------------------------------
//Definition of actions
typedef enum {
  GF_NO_ACTION = 0,   //0 is reserved for no actions

  GF_BRIGHTNESS_UP,   //Increase brightness             (trigger: BL_CTRL + BL_BRIGHT_UP)
  GF_BRIGHTNESS_DOWN, //Decrease brightness             (trigger: BL_CTRL + BL_BRIGHT_DOWN)
  GF_SPEED_UP,        //Increase animation speed        (trigger: BL_CTRL + BL_SPEED_UP)
  GF_SPEED_DOWN,      //Decrease animation speed        (trigger: BL_CTRL + BL_SPEED_DOWN)

  GF_OFF,             //Full Off                        (trigger: BL_CTRL + BL_FULL_OFF)
  GF_ON,              //Full On without animation       (trigger: BL_CTRL + BL_FULL_ON)
  GF_NEXT,            //Change animation mode           (trigger: BL_CTRL + BL_ANIM_NEXT)

  GF_RECORD_ON,       //Start custom pattern recording  (trigger: BL_CTRL + REC_ON)
  GF_RECORD_END,      //Save custom pattern             (trigger: FN)

  GF_SAVE_AS_IDLE,    //Save current mode as idle mode  (trigger: BL_CTRL + BL_ANIM_SAVE_AS_IDLE)
  _LED_ACTION_END_OF_ENUM,
} Led_Action_e;


typedef struct {
  SP_Frame_Header_t head;
  Led_Action_e      action;
  uint8_t checksum;
} __attribute__((packed)) Set_Led_Mode_t;




//6KRO key pressed frame -------------------------------------
typedef struct {
  uint8_t k0    :1;
  uint8_t k1    :1;
  uint8_t k2    :1;
  uint8_t k3    :1;
  uint8_t k4    :1;
  uint8_t k5    :1;
  uint8_t _free :2;
} __attribute__((packed)) K_New_Press_t;


typedef struct {
  uint8_t row  :3;
  uint8_t col   :5;
} __attribute__((packed)) K_Position_t;


typedef struct {
  SP_Frame_Header_t   head;
  struct {
    uint8_t           keycount;
    K_Position_t      position[COMMTASK_PRESSED_6KRO_KEYCOUNT]; //6 keys + 8 modifiers
  } __attribute__((packed)) datas;
  uint8_t             checksum;
} __attribute__((packed)) Pressed_Keys_6kro_t;




//Protocol definition ----------------------------------------
typedef union {
  SP_Frame_Header_t   Header;
  Set_Led_Mode_t      Set_Led_Mode;
  Pressed_Keys_6kro_t  Pressed_Keys_6kro;
} __attribute__((packed)) SP_Messages_u;


extern uint8_t SP_Buffer[sizeof(SP_Messages_u)];



//Master interface ---------------------------------------------
void SP_Master_Init(void);
void SP_Send_Pressed_Keys_6kro(Pressed_Keys_6kro_t* Ptr);
void SP_Send_Set_Led_Mode(Led_Action_e Action);

//Slave interface
void SP_Slave_Init(void);
void SP_Listen_Frame(void);
bool SP_Verify_Frame(void);

#endif //TCKB_SERIAL_PROTO_H
