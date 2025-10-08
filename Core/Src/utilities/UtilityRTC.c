///////////////////////////////////////////////////////////////////////////////
//ARQUIVO:    UtilityRTC
//AUTOR:      Fábio Almeida
//CRIADO:     03/01/2024
//OBSERVAÇÕES:
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "global.h"
/*==============================================================================
WRITE RTC
==============================================================================*/
void writeRTC(uint8_t hour, uint8_t minute, uint8_t second,
		uint8_t day, uint8_t weekDay, uint8_t month, uint8_t year) {

	if(hour > 23) {
		hour = 23;
	}
	if(minute > 59) {
		minute = 59;
	}
	if(second > 59) {
		second = 59;
	}
	if(day > 31 || day == 0) {
		day = 1;
	}
	if(month > 12 && month == 0) {
		month = 1;
	}

	sDate.Date = day;
	sDate.WeekDay = weekDay;
	sDate.Month = month;
	sDate.Year = year;

	sTime.Hours = hour;
	sTime.Minutes = minute;
	sTime.Seconds = second;

	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}
/*==============================================================================
READ RTC
==============================================================================*/
void readRTC() {
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}
/*==============================================================================
FIM DO ARQUIVO
==============================================================================*/
