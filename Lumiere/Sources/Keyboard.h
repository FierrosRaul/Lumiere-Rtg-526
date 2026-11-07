#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif

#define  KEY_VOID           0

// defines de cada aplicacion 1 x tecla + 1 x tecla a repeticion     
//#define  KEY_MEMO           1
#define  KEY_BACK           2
#define  KEY_STOP           3
#define  KEY_PAUSE          4     
#define  KEY_START          5
#define  KEY_DOWN           6
#define  KEY_UP             7
// Teclas a repeticion
#define  KEY_DOWN_RPT       8
#define  KEY_UP_RPT         9
// fin de defines de cada aplicacion    

#define  KEY_WAIT_UNTOUCH 255


#define KEY_RPT_OFF  3
#define KEY_RPT_STR  2
#define KEY_RPT_WT   1
#define KEY_RPT_GO   0


extern void KeyboardInit(void);
extern byte KeyboardChk(void);
extern void KeyboardInt(void);

#ifdef __cplusplus
  }
#endif


#endif