#include <string.h>
#include "6kro_report.h"
#include "Mailboxes.h"

static hid_6kro_report_t _Report;


void KRO6_AddModifier(Key_t* Key) {
  _Report.datas.Modifiers |= ModValue(Key);
}



void KRO6_AddKey(Key_t* Key) {
  if(KRO6_IsFull()) {
    return;
  }

  //Is the key meaningful?
  if(0x00 == GetCode(Key)) {
    return;
  }

  uint8_t i = 0;
  //Check if the key was previously set in the report
  for(i=0; i<_Report._keycount; i++) {
    if(GetCode(Key) == _Report.datas.Keycode[i]) {
      //key is already in the report
      return;
    }
  }

  _Report.datas.Keycode[_Report._keycount] = GetCode(Key);
  _Report._keycount++;
}



bool KRO6_IsFull(void) {
  return (_Report._keycount >= KEYCOUNT_6KRO);
}

bool KRO6_IsEmpty(void) {
  return ((0 == _Report.datas.Modifiers) && (0 == _Report._keycount));
}


void KRO6_Reset(void) {
  memset(&_Report, 0x00, sizeof(hid_6kro_report_t));
}



void KRO6_Send(void) {
  //Sends report to USBTask through mailbox
  // https://developer.mbed.org/handbook/CMSIS-RTOS#mail-queue

  hid_6kro_report_t* ptr = (hid_6kro_report_t*) osMailAlloc(KRO6_Mailbox, osWaitForever);
  if(NULL != ptr) {
    //Pool allocation successful
    memcpy(ptr, &_Report, sizeof(hid_6kro_report_t));
    osMailPut(KRO6_Mailbox, ptr);
  }
  else {
    return;
  }
}
