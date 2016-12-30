#include "Mailboxes.h"


#ifdef _PROJECT_KEYBOARD_CONTROLLER

osMailQId KRO6_Mailbox;
void TCKB_Init_KRO6_Mailbox(void) {
  //6KRO
  osMailQDef(KRO6_Mailbox, MAILBOX_6KRO_COUNT, hid_6kro_report_t);
  KRO6_Mailbox = osMailCreate(osMailQ(KRO6_Mailbox), NULL);
}

#endif


osMailQId SERIAL_Mailbox;

void TCKB_Init_SERIAL_Mailbox(void) {
  //Link to Commtask
  osMailQDef(SERIAL_Mailbox, MAILBOX_SERIAL_MESSAGES, SP_Messages_u);
  SERIAL_Mailbox = osMailCreate(osMailQ(SERIAL_Mailbox), NULL);
}

