#include "HID_Report.h"

static Report_Mode_t _ReportMode = MODE_6KRO;

static uint32_t _last_send_tick = 0;

const uint32_t _Report_Interval[2] = {KRO6_REPORT_SEND_DELAY_Tk, NKRO_REPORT_SEND_DELAY_Tk};

static bool _Empty_Report_Sent = false;

void Report_SetMode(Report_Mode_t Mode) {
  _ReportMode = Mode;
}


Report_Mode_t Report_GetMode(void) {
  return _ReportMode;
}



void Report_Init(void) {
  KRO6_Reset();
  //TODO: NKRO_Reset();
  _ReportMode = MODE_6KRO;
  _last_send_tick = HAL_GetTick();
}



void Report_AddModifier(Key_t* Key) {
  switch(_ReportMode) {
  case MODE_6KRO:
    KRO6_AddModifier(Key);
    break;

  case MODE_NKRO:
    //TODO: NKRO_AddModifier(Key);
    break;
  }
}



void Report_AddKey(Key_t* Key) {
  switch(_ReportMode) {
  case MODE_6KRO:
    KRO6_AddKey(Key);
    break;

  case MODE_NKRO:
    //TODO: NKRO_AddKey(Key);
    break;
  }
}



bool Report_IsFull(void) {
  switch(_ReportMode) {
  case MODE_6KRO:
    return KRO6_IsFull();

  case MODE_NKRO:
    //TODO: return NKRO_IsFull;
    break;
  }

  return true; //To satisfy compiler and not to fill an unknown type (should not happend)
}



bool Report_IsEmpty(void) {
  switch(_ReportMode) {
  case MODE_6KRO:
    return KRO6_IsEmpty();

  case MODE_NKRO:
    //TODO: return NKRO_IsEmpty;
    break;
  }

  return true; //To satisfy compiler and not to fill an unknown type (should not happend)
}



void Report_Reset(void) {
  switch(_ReportMode) {
  case MODE_6KRO:
    KRO6_Reset();
    break;

  case MODE_NKRO:
    //TODO: NKRO_Reset();
    break;
  }
}



void Report_Send(void) {
  if((HAL_GetTick() - _last_send_tick) > _Report_Interval[_ReportMode]) {
    _last_send_tick = HAL_GetTick();

    //We send only one empty report or any data loader report.
    if((false == _Empty_Report_Sent) || (false == Report_IsEmpty())) {
      _Empty_Report_Sent = Report_IsEmpty();

      switch(_ReportMode) {
      case MODE_6KRO:
        KRO6_Send();
        break;

      case MODE_NKRO:
        //TODO: NKRO_Send();
        break;
      }
    }

    Report_Reset();
  }
}
