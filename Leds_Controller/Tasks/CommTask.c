#include "TCKBSerialProto.h"
#include "TCKBLedMatrix.h"
#include "Mailboxes.h"
#include <string.h> //memcpy()

void StartCommTask (void const * argument) {
  osEvent ev;

  SP_Slave_Init(); //Prepare the slave serial protocol

  while(1) {
    //Wait for an spi event coming from SPI_CS signal
    ev = osSignalWait((EVENT_SPI_CS_DOWN | EVENT_SPI_CS_UP), osWaitForever);

    //A signal is received:
    if(osEventSignal == ev.status) {
      switch(ev.value.signals) {
        case EVENT_SPI_CS_DOWN:
          //Configure the spi to listen frame
          SP_Listen_Frame();
          break;

        case EVENT_SPI_CS_UP:
          //Process the received frame
          if(true == SP_Verify_Frame()) {
            //Frame is correct, we can mail it to GraphicTask()
            SP_Messages_u* ptr = (SP_Messages_u*) osMailAlloc(SERIAL_Mailbox, 0);
            if(NULL != ptr) {
              //Pool allocation successful
              memcpy(ptr, SP_Buffer, sizeof(SP_Buffer));
              osMailPut(SERIAL_Mailbox, ptr);
            }
          }
          break;

        default:
          break;
      }
    }
  }
}
