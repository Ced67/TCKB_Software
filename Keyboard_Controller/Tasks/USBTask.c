#include "KeyboardControllerTasksCommon.h"
#include "HID_Report.h"
#include "Mailboxes.h"
#include "Leds.h"


static hid_6kro_report_t _6KRO_ReportToSend;

void StartUSBTask(void const * argument) {
  osEvent result;

  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  while(1){
    if(USBD_STATE_SUSPENDED == hUsbDeviceFS.dev_state) {
      //If host system is halted, we should inform the led task
      osSignalSet(LedTaskHandle, 0x1);
      osDelay(100);
    }
    else {
      //Get reports from mailboxes and send it to usb driver
      switch(Report_GetMode()) {
      case MODE_6KRO:
        {
          result = osMailGet(KRO6_Mailbox, 100);
          if(osEventMail == result.status) {
            hid_6kro_report_t* ptr = (hid_6kro_report_t*)result.value.p;
            memcpy(&_6KRO_ReportToSend, ptr, sizeof(hid_6kro_report_t));
            USBD_HID_SendReport (&hUsbDeviceFS, (uint8_t*)&(_6KRO_ReportToSend.datas), sizeof(hid_6kro_report_datas_t));
            osMailFree(KRO6_Mailbox, ptr);
          }
        };
        break;

      case MODE_NKRO:
        //TODO: Send NKRO Report
        break;
      }
    }
  }
}
