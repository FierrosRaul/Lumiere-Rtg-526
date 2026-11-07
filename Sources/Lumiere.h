#ifndef _ELECTROPORADOR_H
#define _ELECTROPORADOR_H


#define  KEY_VOID           0    
#define  KEY_MEMO           1
#define  KEY_BACK           2
#define  KEY_STOP           3
#define  KEY_PAUSE          4     
#define  KEY_START          5
#define  KEY_DOWN           6
#define  KEY_UP             7
#define  KEY_DOWN_RPT       8
#define  KEY_UP_RPT         9
#define  KEY_WAIT_UNTOUCH 255


#define KEY_RPT_OFF  3
#define KEY_RPT_STR  2
#define KEY_RPT_WT   1
#define KEY_RPT_GO   0

#define  PANT_VOID         0
#define  PANT_TIME         1
#define  PANT_WLCM         2
#define  PANT_MENU         3
#define  PANT_SEL          4
#define  PANT_ADJS         5
#define  PANT_CHKCTRL      6
#define  PANT_WORK         7
#define  PANT_SETTINGS     8
#define  PANT_PASSWORD     9
#define  PANT_CALIBTOUCH   10


//#define  WRKSTAT_INIT     0
//#define  WRKSTAT_STOP     1
//#define  WRKSTAT_PAUSE    2
//#define  WRKSTAT_RUN      3

#define MODE_FAC   0
#define MODE_CRP   1

#define LED_WHITE    0
#define LED_RED      1
#define LED_GREEN    2
#define LED_BLUE     3
#define LED_AMBAR    4


#define ST_STOP    0
#define ST_PAUSE   1
#define ST_RUN     2

#define EV_OFF     0
#define EV_ON      1


#define MODE_X1  2
#define MODE_Y1  5
#define MODE_X2  29
#define MODE_Y2  9

#define VENT_X1  34
#define VENT_Y1  5
#define VENT_X2  105
#define VENT_Y2  9


#define STATUS_X1  91
#define STATUS_Y1  5
#define STATUS_X2  120
#define STATUS_Y2  9


extern void Main(void);
extern void RtcIntWork(void);


extern byte         g_PantAct;

extern unsigned int l_Tmr_End;
extern byte         g_Trim;
extern byte         g_RtcInt; 
extern byte         g_SoundInt;
extern unsigned int l_PwmSet;
#endif /* __ELECTROPORADOR_H */


