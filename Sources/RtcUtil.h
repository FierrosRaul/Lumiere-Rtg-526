#ifndef _RTCUTIL_H
#define _RTCUTIL_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif

#include "derivative.h" /* include peripheral declarations */


#define RTC_HOUR  1
#define RTC_MIN   2
#define RTC_DAY   3
#define RTC_MONTH 4
#define RTC_YEAR  5


//extern unsigned char g_RTC_Sec;
//extern unsigned char g_RTC_Min;
//extern unsigned char g_RTC_Hour;
//extern unsigned char g_RTC_Day;
//extern unsigned char g_RTC_Month;
//extern unsigned int  g_RTC_Year;

extern void GetRTC(unsigned int *RTC_Year,byte *RTC_Month,byte *RTC_Day,byte *RTC_Hour,byte *RTC_Min,byte *RTC_Sec);
extern void SetRTC(unsigned int RTC_Year,byte RTC_Month,byte RTC_Day,byte RTC_Hour,byte RTC_Min,byte RTC_Sec);


extern byte SetTimeUp(byte SetSel);
extern byte SetTimeDw(byte SetSel);


extern void RTC_Interrupt(void);
extern void SetInactivity(byte Secs);
extern void ResetInactivity(void);
extern byte CheckInactivity(void);



#ifdef __cplusplus
  }
#endif


#endif