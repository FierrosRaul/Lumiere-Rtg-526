#include "derivative.h"
#include "Ports.h"
#include "Keyboard.h"

byte Key_Debounce;
byte Key_Repeat_Time;


void KeyboardInit(void);
byte KeyboardChk(void);
void KeyboardInt(void);



//////////////////////// KEYBOARD  /////////////////////////////////////////////////////////////
void KeyboardInit(void){
    Key_Debounce = 0;
    Key_Repeat_Time = KEY_RPT_OFF;
}



byte KeyboardChk(){
  static byte OldKey = KEY_VOID;

    if(Key_Debounce != 0){
       return KEY_VOID;
    }
   
//////////////////////////////////     OLD KEY = KEY_WAIT_UNTOUCH   ////////////////////////////////////////////////////////// 
    if(OldKey == KEY_WAIT_UNTOUCH ){
      if(PORT_KEY_BACK && PORT_KEY_STOP && PORT_KEY_PAUSE && PORT_KEY_START && PORT_KEY_DOWN && PORT_KEY_UP){
        Key_Debounce = 2; 
        OldKey = KEY_VOID;
      }
      Key_Repeat_Time = KEY_RPT_OFF;
      return KEY_VOID;
    }
    
//////////////////////////////////     OLD KEY = KEY_VOID   ////////////////////////////////////////////////////////// 
    else if(OldKey == KEY_VOID ){
      if(PORT_KEY_BACK  == 0){    //in    55 back
        Key_Debounce = 2;
        OldKey = KEY_BACK;
      }
      else if(PORT_KEY_STOP  == 0){    //in    60 stop
        Key_Debounce = 2;
        OldKey = KEY_STOP;       
      }
      else if(PORT_KEY_PAUSE == 0){    //in    61 pause
        Key_Debounce = 2;
        OldKey = KEY_PAUSE;       
      }
      else if(PORT_KEY_START == 0){    //in    62 start
        Key_Debounce = 2;
        OldKey = KEY_START;       
      }
      else if(PORT_KEY_DOWN  == 0){    //in    26 abajo
        Key_Debounce = 2;
        OldKey = KEY_DOWN;       
      }
      else if(PORT_KEY_UP    == 0){    //in    27 arriba
        Key_Debounce = 2;
        OldKey = KEY_UP;       
      }
      Key_Repeat_Time = KEY_RPT_OFF;
      return KEY_VOID;
    }
//////////////////////////////////     OLD KEY = VALID KEY   ////////////////////////////////////////////////////////// 
    else{
      if(PORT_KEY_BACK  == 0 &&  OldKey == KEY_BACK){
        Key_Repeat_Time = KEY_RPT_OFF;
        OldKey = KEY_WAIT_UNTOUCH;       
        return KEY_BACK;
      }
      else if(PORT_KEY_STOP  == 0 &&  OldKey == KEY_STOP){    //in    60 stop
        Key_Repeat_Time = KEY_RPT_OFF;
        OldKey = KEY_WAIT_UNTOUCH;       
        return KEY_STOP;
      }
      else if(PORT_KEY_PAUSE == 0 &&  OldKey == KEY_PAUSE){    //in    61 pause
        Key_Repeat_Time = KEY_RPT_OFF;
        OldKey = KEY_WAIT_UNTOUCH;       
        return KEY_PAUSE;
      }
      else if(PORT_KEY_START == 0 &&  OldKey == KEY_START){    //in    62 start
        Key_Repeat_Time = KEY_RPT_OFF;
        OldKey = KEY_WAIT_UNTOUCH;       
        return KEY_START;
      }
      else if(PORT_KEY_DOWN  == 0 &&  OldKey == KEY_DOWN){    //in    26 abajo
        if(Key_Repeat_Time == KEY_RPT_OFF){
          Key_Repeat_Time = KEY_RPT_STR;
          return KEY_DOWN;
        }
        else if(Key_Repeat_Time == KEY_RPT_GO){
          Key_Repeat_Time = KEY_RPT_STR;
          return KEY_DOWN_RPT;
        }
      }
      else if(PORT_KEY_UP    == 0 &&  OldKey == KEY_UP){    //in    27 arriba
        if(Key_Repeat_Time == KEY_RPT_OFF){
          Key_Repeat_Time = KEY_RPT_STR;
          return KEY_UP;
        }
        else if(Key_Repeat_Time == KEY_RPT_GO){
          Key_Repeat_Time = KEY_RPT_STR;
          return KEY_UP_RPT;
        }
      }
      else{
        Key_Repeat_Time = KEY_RPT_OFF;
        OldKey = KEY_WAIT_UNTOUCH;
        return KEY_VOID;       
      }
    }
    return KEY_VOID;       
}



void KeyboardInt(){      // ~ 1 milisegundos
  static unsigned int  Counter         = 0;
  static unsigned int  Counter2        = 0;

//////// Keyboard control //////////////////////  
  if(Key_Debounce == 2){
    Counter = 0;
    Key_Debounce = 1;
  }
  else if(Key_Debounce == 1){
    if(Counter < 18){
      Counter++;
    }
    else{
      Key_Debounce = 0;
      Counter = 0;
    }
  }

  if(Key_Repeat_Time == KEY_RPT_STR){
    Counter2 = 0;
    Key_Repeat_Time = KEY_RPT_WT;
  }

  else if(Key_Repeat_Time == KEY_RPT_WT){
    if(Counter2 < 100){
      Counter2++;
    }
    else{
      Key_Repeat_Time = KEY_RPT_GO;
    }
  }
}