#include "derivative.h" /* include peripheral declarations */
#include "RtcUtil.h"

byte         l_RTC_Sec  = 255;
byte         l_RTC_Min  = 255;
byte         l_RTC_Hour = 255;
byte         l_RTC_Day  = 255;
byte         l_RTC_Month= 255;
unsigned int l_RTC_Year = 65535;
byte         l_InactivityCountDown = 0;
byte         l_Secs     = 0;



void GetRTC(unsigned int *RTC_Year,byte *RTC_Month,byte *RTC_Day,byte *RTC_Hour,byte *RTC_Min,byte *RTC_Sec){
 *RTC_Year  = l_RTC_Year;
 *RTC_Month = l_RTC_Month;
 *RTC_Day   = l_RTC_Day;
 *RTC_Hour  = l_RTC_Hour;
 *RTC_Min   = l_RTC_Min;
 *RTC_Sec   = l_RTC_Sec;
};


void SetRTC(unsigned int RTC_Year,byte RTC_Month,byte RTC_Day,byte RTC_Hour,byte RTC_Min,byte RTC_Sec){
 l_RTC_Year  = RTC_Year;
 l_RTC_Month = RTC_Month;
 l_RTC_Day   = RTC_Day;
 l_RTC_Hour  = RTC_Hour;
 l_RTC_Min   = RTC_Min;
 l_RTC_Sec   = RTC_Sec;
};



byte SetTimeUp(byte SetSel){
  if(SetSel == 1){
    if(l_RTC_Hour == 23)
      l_RTC_Hour = 0;
    else
      l_RTC_Hour++;
    return 1;
  }
  else if(SetSel == 2){
    if(l_RTC_Min == 59)
      l_RTC_Min  = 0;
    else
      l_RTC_Min++;
    return 1;
  }
  else if(SetSel == 3){
    if(l_RTC_Month == 1 | l_RTC_Month == 3 | l_RTC_Month == 5 | l_RTC_Month == 7 | l_RTC_Month == 8 | l_RTC_Month == 10 | l_RTC_Month == 12 ){
      if(l_RTC_Day == 31)
        l_RTC_Day = 1;
      else
        l_RTC_Day++;
      return 1;
    }
    else if(l_RTC_Month == 2){
      if(l_RTC_Day > 28 || l_RTC_Day == 28 && !(l_RTC_Year & 0x0004))
        l_RTC_Day = 1;
      else
        l_RTC_Day++;
      return 1;
    }
    else{
      if(l_RTC_Day == 30)
        l_RTC_Day = 1;
      else
        l_RTC_Day++;
      return 1;
    }
          
  }
  else if(SetSel == 4){
    if(l_RTC_Month==12){
      l_RTC_Month=1;
    }
    else{
      l_RTC_Month++;
    }
    return 1;
  }
  else if(SetSel == 5){
    if(l_RTC_Year< 9999){
      l_RTC_Year++;
    }
    else{
      l_RTC_Year =2019;
    }
    return 1;
  }
  return 0;
}

byte SetTimeDw(byte SetSel){
  if(SetSel == 1){
    if(l_RTC_Hour == 0)
      l_RTC_Hour = 23;
    else
      l_RTC_Hour--;  
    return 1;
  }
  else if(SetSel == 2){
    if(l_RTC_Min == 0)
      l_RTC_Min  = 59;
    else
      l_RTC_Min--;
    return 1;
  }
  else if(SetSel == 3){
    if(l_RTC_Month == 1 | l_RTC_Month == 3 | l_RTC_Month == 5 | l_RTC_Month == 7 | l_RTC_Month == 8 | l_RTC_Month == 10 | l_RTC_Month == 12 ){
      if(l_RTC_Day == 1)
        l_RTC_Day = 31;
      else
        l_RTC_Day--;
      return 1;
    }
    else if(l_RTC_Month == 2){
      if(l_RTC_Day == 1){
        if(l_RTC_Year & 0x0004)
          l_RTC_Day = 29;
        else
          l_RTC_Day = 28;
      }
      else
        l_RTC_Day--;
      return 1;
      
    }
    else{
      if(l_RTC_Day == 1)
          l_RTC_Day = 30;
        else
          l_RTC_Day--;
      return 1;
    }
          
  }
  else if(SetSel == 4){
    if(l_RTC_Month == 1){
      l_RTC_Month = 12;
    }
    else{
      l_RTC_Month--;
    }
    return 1;
  }
  else if(SetSel == 5){
    if(l_RTC_Year > 2019){
      l_RTC_Year--;
      return 1;
    }
    else{
      return 0;
    }
  }
  return 0;
}





void RTC_Interrupt(){     // 1 vez por segundo
  if(l_RTC_Sec < 59)
    l_RTC_Sec++;
  else{
    l_RTC_Sec = 0;
    if(l_RTC_Min < 59)
      l_RTC_Min++;
    else{
      l_RTC_Min = 0;
      if(l_RTC_Hour < 23)
        l_RTC_Hour++;
      else{
        l_RTC_Hour = 0;
      }
    }
  }

  if(l_InactivityCountDown > 0){
    l_InactivityCountDown--;
  }
}


void SetInactivity(byte Secs){
    l_Secs = Secs;
    l_InactivityCountDown = Secs;
}


void ResetInactivity(void){
    l_InactivityCountDown = l_Secs;
}

void ForceInactivity(void){
    l_InactivityCountDown = 0;
}


byte CheckInactivity(void){
    if(l_InactivityCountDown == 0)
      return 1;
    return 0;
}

