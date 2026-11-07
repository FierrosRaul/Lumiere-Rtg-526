#include "derivative.h"
#include "Lumiere.h"
#include "MCUinit.h"
#include "DisplayTools.h"
#include "ScreenData.h"
#include "Ports.h"
#include "RtcUtil.h"
#include "SoundsTools.h"
#include "FlashTool.h"
#include "FlashData.h"
#include "TchScr.h"
#include "TouchTools.h"
#include "Dallas.h"


#define   VOID_CMD           0
#define   STOP_CMD           1
#define   PAUSE_CMD          2
#define   START_CMD          3 
#define   HIDDEN_CMD         4
#define   ENTER_CMD          5
#define   DELETE_CMD         6
#define   NUMBER_CMD         7
#define   SEL1_CMD           8
#define   SEL2_CMD           9
#define   SEL3_CMD           10
#define   SEL4_CMD           11
#define   SEL5_CMD           12
#define   UP_CMD             13 
#define   DW_CMD             14 
#define   LEVEL_UP_CMD       15 
#define   LEVEL_DW_CMD       16 
#define   RESET_TM_CMD       17 
#define   TRIM_CMD           18 
#define   AMBAR_RED_PORC_CMD 19
#define   WHITE_PORC_CMD     20 




/////////////////////////////////// VARIABLES LOCALES ////////////////////////

byte         l_Function   = LED_WHITE;  // Funcion del 1800 Electroporador/MDA
byte         l_Power      = 0;        // Potencia del EP
unsigned int l_PwmSet     = 0;
byte         l_TimeRun    = 0;        // tiempo restante de la corrida
int          l_Temp       = 0;

byte         l_PrcMin     = 0;        // parcializador del total de minutos de uso del equipo
unsigned int l_PrcHrs     = 0;        // parcializador del total de horas de uso del equipo
byte         l_TotMin     = 0;        // total de minutos de uso del equipo
unsigned int l_TotHrs     = 0;        // total de horas de uso del equipo


byte         l_TmOffEv    = EV_OFF;   // indicado de fin de corrida
byte         l_Status     = ST_STOP;  // indica si el equipo esta corriendo
byte         l_TopSignal  = EV_OFF;   // indicador de top de señal de que paso un minuto
byte         l_TempAlarm  = 0;

unsigned int l_BlinkCount     = 0;

byte         l_CoolerFan  = 0;

byte         l_LastPow;
byte         l_LastTm;
byte         l_LastFunc;

unsigned int l_AmbRedPorc = 70;
unsigned int l_WhitePorc  = 40;


/////////////////////////////////// VARIABLES GLOBALES ////////////////////////

byte         g_Trim           = 100;
byte         g_RtcInt         = 0; 
byte         g_SoundInt       = 0;
byte         g_PantAct        = 0;

//////////////////////////////////////////////////////////////////////////////


void Main(void);
void NavMenu(void);

char TimeScr(void);
char WlcmScr(void);
char MnuScr(void);
char SelectScr(void);
char AjustesScr(void);
char ChkCtrlScr(void);
char WorkScr(void);
char SettingsScr(void);
char ChkPswScr(void);
char CalibTouchScr(void);

void IntWorkTimer();
void BlinkCounter(void);
void RtcIntWork(void);

void ChkTch(void);

byte TimeDw(byte RptCntr,byte *Time);
byte TimeUp(byte RptCntr,byte *Time,byte Limit);

byte PwrDw(byte RptCntr,byte *Power);
byte PwrUp(byte RptCntr,byte *Power,byte Limit);

byte PswDw(byte RptCntr,unsigned int *PswData);
byte PswUp(byte RptCntr,unsigned int *PswData,unsigned int Limit);

void FlashSaveData(void);
void FlashRestoreData(void);

void ConvTchCoord(unsigned int TchScr,unsigned char *TchX,unsigned char *TchY);
byte SetCalibTch(int TchScrMin,int TchScrMax);

void SetColorMsg(char *PantallaActiva,byte ActOn);

byte DallasCRC(byte *buffer);
void VersionPrint(char *PantBuff);


 void Main(){
	  COOLER_FAN  = 0;
	  l_CoolerFan = 0;
    FlashRestoreData();
    SetDispIntBC();
    DspScreenInit();   // Inicializo display
    NavMenu();       // Navegacion del 1800
}


///////////////////////////////  Pantallas      /////////////////////////////////////////

void NavMenu(){
  char NavStatus = PANT_WLCM;
  
  g_PantAct = PANT_VOID;
  
  while(1){
    if(NavStatus == PANT_TIME){
      NavStatus = TimeScr();
    }
    else if(NavStatus == PANT_WLCM){
      NavStatus = WlcmScr();
    }
    else if(NavStatus == PANT_MENU){
      NavStatus = MnuScr();
    }
    else if(NavStatus == PANT_SEL){
      NavStatus = SelectScr();
    }

    else if(NavStatus == PANT_ADJS){
      NavStatus = AjustesScr();
    }
    else if(NavStatus == PANT_CHKCTRL){
      NavStatus = ChkCtrlScr();
    }
    else if(NavStatus == PANT_WORK){
      NavStatus = WorkScr();
    }
    else if(NavStatus == PANT_SETTINGS){
      NavStatus = SettingsScr();
    }
    else if(NavStatus == PANT_PASSWORD){
      NavStatus = ChkPswScr();
    }
    else if(NavStatus == PANT_CALIBTOUCH){
      NavStatus = CalibTouchScr();
    }
    else{
      NavStatus = WlcmScr();
    }
  }
}



char TimeScr(){
  char *PantallaActiva;
  byte Rc;
  
  byte GoBack             = PANT_VOID;
  byte GoAhead            = PANT_VOID;

  unsigned char TchX      = 0xFF;
  unsigned char TchY      = 0xFF;

  byte          SetSel    = 0;
  byte          SetSelAnt = 0;
  byte          BlinkOn   = 1;
  byte          CmdButton = VOID_CMD;

  byte          RTC_Sec;
  byte          RTC_Min;
  byte          RTC_Hour;
  byte          RTC_Day;
  byte          RTC_Month;
  unsigned int  RTC_Year;

  byte          RTC_MinAnt  = 255;
  byte          RTC_HourAnt = 255;
  byte          RTC_DayAnt  = 255;
  byte          RTC_MonthAnt= 255;
  unsigned int  RTC_YearAnt = 65535;

  g_PantAct = PANT_TIME;

  PantallaActiva = &Time_Sc;
  DspPantalla((int) PantallaActiva);
  SetInactivity(30);
  ResetRgn();

  for(;;) {
    RtcIntWork();  

    GetRTC(&RTC_Year,&RTC_Month,&RTC_Day,&RTC_Hour,&RTC_Min,&RTC_Sec);

    if(RTC_Sec & 1){
      RePaintRgn((char *) PantallaActiva,64,32,67,46,0);
    }
    else{
      FillRgn(PantallaActiva,64,32,67,46,0);
    }

    if(CheckInactivity()){
      SetStandBy(1);
    }
    else{
      SetStandBy(0);
    }
    
    if(RTC_Hour  != RTC_HourAnt){
      PrintCharValue21x32Inv(PantallaActiva,RTC_Hour  ,13,20,2,0);
      RTC_HourAnt = RTC_Hour;
    };
    if(RTC_Min   != RTC_MinAnt){
      PrintCharValue21x32Inv(PantallaActiva,RTC_Min   ,76,20,2,0);
      RTC_MinAnt = RTC_Min;
    };
    if(RTC_Day   != RTC_DayAnt){
      PrintCharValue7x5Inv(PantallaActiva,RTC_Day  ,52,4,2,1);
      RTC_DayAnt = RTC_Day;
    };
    if(RTC_Month != RTC_MonthAnt){
      PrintCharValue7x5Inv(PantallaActiva,RTC_Month,72,4,2,1);
      RTC_MonthAnt = RTC_Month;
    };
    if(RTC_Year  != RTC_YearAnt){
      PrintIntValue7x5Inv (PantallaActiva,RTC_Year ,91,4,4,1);
      RTC_YearAnt = RTC_Year;
    };

    if(SetSel != SetSelAnt){
      if(SetSelAnt == 1){
        PrintCharValue21x32Inv(PantallaActiva,RTC_Hour  ,13,20,2,0);
      }
      else if(SetSelAnt == 2){ 
        PrintCharValue21x32Inv(PantallaActiva,RTC_Min   ,76,20,2,0);
      }
      else if(SetSelAnt == 3){
        PrintCharValue7x5Inv(PantallaActiva,RTC_Day  ,52,4,2,1);
      }
      else if(SetSelAnt == 4){
        PrintCharValue7x5Inv(PantallaActiva,RTC_Month,72,4,2,1);
      }
      else if(SetSelAnt == 5){
        PrintIntValue7x5Inv (PantallaActiva,RTC_Year ,91,4,4,1);
      }
      SetSelAnt = SetSel;
    }

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      ResetInactivity();

      if(SetSel != 0 && ChkRgn(PantallaActiva,TchX,TchY,0,57,15,63,1) == RGN_OK){  // Boton -
        if(SetTimeDw(SetSel)==1){
          SoundDw();
        }
        else{
          SoundSelNok();
        }
      }
      else if(SetSel != 0 && ChkRgn(PantallaActiva,TchX,TchY,112,57,127,63,3) == RGN_OK){  // Boton +
        if(SetTimeUp(SetSel)==1){
          SoundUp();
        }
        else{
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,44,57,89,63,2) == RGN_OK){  // Noto button
        CmdButton = ENTER_CMD;
      }
      else if(SetSel != 0){
        if(ChkRgn(PantallaActiva,TchX,TchY,13,20, 60,52,240) == RGN_OK){  // Selecciono hora
          CmdButton = SEL1_CMD;
        }
        else if(ChkRgn(PantallaActiva,TchX,TchY,76,20,122,52,241) == RGN_OK){ // Selecciono minutos
          CmdButton = SEL2_CMD;
        }
        else if(ChkRgn(PantallaActiva,TchX,TchY,52, 4, 66,12,245) == RGN_OK){ // Selecciono dia
          CmdButton = SEL3_CMD;
        }
        else if(ChkRgn(PantallaActiva,TchX,TchY,72, 4, 86,12,246) == RGN_OK){ // Selecciono mes
          CmdButton = SEL4_CMD;
        }
        else if(ChkRgn(PantallaActiva,TchX,TchY,91, 4,119,12,247) == RGN_OK){ // Selecciono año
          CmdButton = SEL5_CMD;
        }
      }
      else{                       // esta tocando fuera de region
        GoAhead =  PANT_WLCM;
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
    
      if(CmdButton == ENTER_CMD){
        CmdButton = VOID_CMD;
        if(SetSel != 0){
          SetSel = 0;
          FlashSaveData();
          SoundWrite();
        }
        else{
          SetSel = 1;
          SoundUp();
        }
      }
      else if(CmdButton == SEL1_CMD){
        CmdButton = VOID_CMD;
      	if(SetSel != 0){
          SetSel = 1;
          SoundUp();
      	}
      }
      else if(CmdButton == SEL2_CMD){
        CmdButton = VOID_CMD;
      	if(SetSel != 0){
          SetSel = 2;
          SoundUp();
      	}
      }
      else if(CmdButton == SEL3_CMD){
        CmdButton = VOID_CMD;
      	if(SetSel != 0){
          SetSel = 3;
          SoundUp();
      	}
      }
      else if(CmdButton == SEL4_CMD){
        CmdButton = VOID_CMD;
      	if(SetSel != 0){
          SetSel = 4;
          SoundUp();
      	}
      }
      else if(CmdButton == SEL5_CMD){
        CmdButton = VOID_CMD;
      	if(SetSel != 0){
          SetSel = 5;
          SoundUp();
      	}
      }
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
    }

    if(l_BlinkCount & 512){
      if(BlinkOn == 0){
        if(SetSel  == 1){
          PrintCharValue21x32Inv(PantallaActiva,RTC_Hour  ,13,20,2,0);
        }
        else if(SetSel  == 2){
          PrintCharValue21x32Inv(PantallaActiva,RTC_Min   ,76,20,2,0);
        }
        else if(SetSel  == 3){
          PrintCharValue7x5Inv(PantallaActiva,RTC_Day  ,52,4,2,1);
        }
        else if(SetSel  == 4){
          PrintCharValue7x5Inv(PantallaActiva,RTC_Month,72,4,2,1);
        }
        else if(SetSel  == 5){
          PrintIntValue7x5Inv (PantallaActiva,RTC_Year ,91,4,4,1);
        }
        BlinkOn = 1;
      }
      else{
        if(RTC_Hour  != RTC_HourAnt &&  SetSel  == 1){
          PrintCharValue21x32Inv(PantallaActiva,RTC_Hour  ,13,20,2,0);
          RTC_HourAnt = RTC_Hour;
        };
        if(RTC_Min   != RTC_MinAnt  &&  SetSel  == 2){
          PrintCharValue21x32Inv(PantallaActiva,RTC_Min   ,76,20,2,0);
          RTC_MinAnt = RTC_Min;
        };
        if(RTC_Day   != RTC_DayAnt  &&  SetSel  == 3){
          PrintCharValue7x5Inv(PantallaActiva,RTC_Day  ,52,4,2,1);
          RTC_DayAnt = RTC_Day;
        };
        if(RTC_Month != RTC_MonthAnt && SetSel  == 4){
          PrintCharValue7x5Inv(PantallaActiva,RTC_Month,72,4,2,1);
          RTC_MonthAnt = RTC_Month;
        };
        if(RTC_Year  != RTC_YearAnt &&  SetSel  == 5){
          PrintIntValue7x5Inv (PantallaActiva,RTC_Year ,91,4,4,1);
          RTC_YearAnt = RTC_Year;
        };
      }
    }
    else{
      if(BlinkOn == 1){
        if(SetSel  == 1){
          PrintBlankValue21x32Inv(PantallaActiva,13,20,2,0);
        }
        else if(SetSel  == 2){
          PrintBlankValue21x32Inv(PantallaActiva,76,20,2,0);
        }
        else if(SetSel  == 3){
          PrintBlankValue7x5Inv(PantallaActiva,52,4,2,1);
        }
        else if(SetSel  == 4){
          PrintBlankValue7x5Inv(PantallaActiva,72,4,2,1);
        }
        else if(SetSel  == 5){
          PrintBlankValue7x5Inv(PantallaActiva,91,4,4,1);  
        }
        BlinkOn = 0;
      }
    }
  }  
}




char WlcmScr(){
  byte Key;
  char *PantallaActiva;
  byte Rc;
  
  byte GoBack  = PANT_VOID;
  byte GoAhead = PANT_VOID;

  unsigned char TchX     = 0xFF;
  unsigned char TchY     = 0xFF;

  PantallaActiva    = &Welcome_Sc;
	l_Function = LED_WHITE;
  l_Status   = ST_STOP;

  DspPantalla((int) PantallaActiva);

  SetInactivity(120);

  if(g_PantAct == PANT_VOID)
	SoundWlcm();

  g_PantAct = PANT_WLCM;

  ResetRgn();

  for(;;) {
    RtcIntWork();  

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      ResetInactivity();
      if(ChkRgn(PantallaActiva,TchX,TchY,49,53,78,63,1) == RGN_OK){
        GoBack = PANT_TIME;
      }
      else{
        GoAhead =  PANT_MENU;
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
    }

    // Chequeo timeout de inactividad
    if(CheckInactivity()){
      SoundMenuBk();
      return PANT_TIME;
    }
  }
}


char MnuScr(){
  byte GoBack  = PANT_VOID;
  byte GoAhead = PANT_VOID;
  byte Rc;

  unsigned char TchX     = 0xFF;
  unsigned char TchY     = 0xFF;
  char *PantallaActiva;

  g_PantAct = PANT_MENU;
  PantallaActiva = &Menu_Sc;

  ResetRgn();
  DspPantalla((int) PantallaActiva);

  for(;;) {
    RtcIntWork();  

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      if(ChkRgn(PantallaActiva,TchX,TchY,5,7,122,17,1) == RGN_OK){
        GoAhead = PANT_SEL;    //                                                   // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,5,29,122,39,2) == RGN_OK){
        GoAhead = PANT_ADJS;    // ajustes Brillo contraste volumen                                            // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,45,53,122,63,3) == RGN_OK){
        GoAhead = PANT_CHKCTRL;    // check control                                            // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,5,53,33,63,4) == RGN_OK){
        GoBack  = PANT_WLCM;    // welcome                                          // esta tocando en la zona activa
      }
      else{                       // esta tocando fuera de region
        ResetInactivity();
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
    }
  }  
}




char SelectScr(){
  byte GoBack  = PANT_VOID;
  byte GoAhead = PANT_VOID;
  byte Rc;

  unsigned char TchX     = 0xFF;
  unsigned char TchY     = 0xFF;

  char *PantallaActiva;

  g_PantAct = PANT_SEL;
  PantallaActiva = &SelMode_Sc;

  ResetRgn();
  DspPantalla((int) PantallaActiva);

  for(;;) {
    RtcIntWork();  

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      if(ChkRgn(PantallaActiva,TchX,TchY,1,14,40,27,1) == RGN_OK){
				l_Function  = LED_RED;
        GoAhead     = PANT_WORK;    //                                                   // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,87,14,126,27,2) == RGN_OK){
				l_Function  = LED_BLUE;
        GoAhead     = PANT_WORK;    //                                                   // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,40,14,83,27,4) == RGN_OK){
				l_Function  = LED_GREEN;
        GoAhead     = PANT_WORK;    //                                                   // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,66,34,105,47,4) == RGN_OK){
				l_Function  = LED_AMBAR;
				GoAhead     = PANT_WORK;    //                                                   // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,22,34,61,47,4) == RGN_OK){
				l_Function  = LED_WHITE;
        GoAhead     = PANT_WORK;    //                                                   // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,6,56,126,63,5) == RGN_OK){
        GoAhead     = PANT_MENU;    //                                                   // esta tocando en la zona activa
      }
      else{                       // esta tocando fuera de region
        ResetInactivity();
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
    }
  }  
}



char AjustesScr(void){
  byte GoBack  = PANT_VOID;
  byte GoAhead = PANT_VOID;
  byte Rc;

  unsigned char TchX     = 0xFF;
  unsigned char TchY     = 0xFF;

  char *PantallaActiva;

  unsigned int  aux;
  byte BlinkOn = 0;


  g_PantAct = PANT_ADJS;
  PantallaActiva = &BrCntAud_Sc;

  
  DspPantalla((int) PantallaActiva);
  FillRgn(PantallaActiva,38 + (GetBrillo()    * 7), 3,108,13,0);
  FillRgn(PantallaActiva,38 + (GetContraste() * 7),21,108,31,0);
  FillRgn(PantallaActiva,38 + (GetVolumen()   * 7),39,108,49,0);

  ResetRgn();
  for(;;) {
    RtcIntWork();  

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      if(     ChkRgn(PantallaActiva,TchX,TchY, 16, 0, 35,16,6) == RGN_OK){
        if(BrilloDw()){
          FillRgn(PantallaActiva,38 + (GetBrillo() * 7), 3,108,13,0);
          RePaintRgn(PantallaActiva,38,3,38 + (GetBrillo() * 7),13,0);
          SoundUp();
        }
        else{
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 16,18, 35,34,4) == RGN_OK){
        if(ContrasteDw()){
          FillRgn(PantallaActiva,38 + (GetContraste() * 7),21,108,31,0);
          RePaintRgn(PantallaActiva,38,21,38 + (GetContraste() * 7),31,0);
          SoundUp();
        }
        else{
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 16,36, 35,52,2) == RGN_OK){
        if(VolumenDw()){
          FillRgn(PantallaActiva,38 + (GetVolumen()   * 7),39,108,49,0);
          RePaintRgn(PantallaActiva,38,39,38 + (GetVolumen()    * 7),49,0);
          SoundUp();
        }
        else{
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,111, 0,127,16,5) == RGN_OK){
        if(BrilloUp()){
          FillRgn(PantallaActiva,38 + (GetBrillo() * 7), 3,108,13,0);
          RePaintRgn(PantallaActiva,38,3,38 + (GetBrillo() * 7),13,0);
          SoundUp();
        }
        else{
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,111,18,127,34,3) == RGN_OK){
        if(ContrasteUp()){
          FillRgn(PantallaActiva,38 + (GetContraste() * 7),21,108,31,0);
          RePaintRgn(PantallaActiva,38,21,38 + (GetContraste() * 7),31,0);
          SoundUp();
        }
        else{
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,111,36,127,52,1) == RGN_OK){
        if(VolumenUp()){
          FillRgn(PantallaActiva,38 + (GetVolumen()   * 7),39,108,49,0);
          RePaintRgn(PantallaActiva,38,39,38 + (GetVolumen()    * 7),49,0);
          SoundUp();
        }
        else{
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,5,53,33,63,7) == RGN_OK){
        GoBack  = PANT_MENU;    // check control                                            // esta tocando en la zona activa
      }
      else{                       // esta tocando fuera de region
        ResetInactivity();
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        FlashSaveData();
        SoundWrite();
        return GoBack;
      }
    }
  }
}


char WorkScr(void){
  byte GoBack  = PANT_VOID;
  byte GoAhead = PANT_VOID;
  byte Rc;
  unsigned char TchX     = 0xFF;
  unsigned char TchY     = 0xFF;
  

  char *PantallaActiva;

  byte          CmdButton = VOID_CMD;
  byte          RptCntr    = 0;
  byte          AdValue    = 0;
  byte          WaitAdTime = 0;
  byte          CoolerFanMsgAnt = 3;
  int           TempAnt    = -1;
 
  g_PantAct = PANT_WORK;
  PantallaActiva = &Work_Sc;
  DspPantalla((int) PantallaActiva);

  l_TopSignal = EV_OFF;
  l_TmOffEv   = EV_OFF;
  l_Status    = ST_STOP;

  SetColorMsg(PantallaActiva,0);

  // mostrar tiempo y potencia
  PrintCharValue24x13(PantallaActiva,l_TimeRun,12,23,2);
  PrintCharValue24x13(PantallaActiva,l_Power,71,23,3);

  RePaintRgn(PantallaActiva, 1,54,29,63,1); // Stop
  RePaintRgn(PantallaActiva,46,54,77,63,0); // Pause
  RePaintRgn(PantallaActiva,92,54,126,63,0); // Start

  PrintIntValue7x5Inv(PantallaActiva,0,111,4,2,0);
    
  for(;;) {
    RtcIntWork(); 
    
	  if(l_CoolerFan == 1 && CoolerFanMsgAnt != 1 && l_TempAlarm == 0){
	    InsertInvRgn(PantallaActiva,&VentOn_Msg,VENT_X1,VENT_Y1,VENT_X2,VENT_Y2,0);
	    CoolerFanMsgAnt = 1;
	  }
	  else if(l_CoolerFan == 0 && CoolerFanMsgAnt != 0 && l_TempAlarm == 0){
	    InsertInvRgn(PantallaActiva,&VentOff_Msg,VENT_X1,VENT_Y1,VENT_X2,VENT_Y2,0);
	    CoolerFanMsgAnt = 0;
	  }
	  else if(l_TempAlarm == 1 && CoolerFanMsgAnt != 2){
	    InsertInvRgn(PantallaActiva,&VentAlrm_Msg,VENT_X1,VENT_Y1,VENT_X2,VENT_Y2,1);
	    CoolerFanMsgAnt = 2;
	  }
	  

    if(l_Temp != TempAnt){
      TempAnt = l_Temp;
      
      if(l_Temp > 2) 
        PrintIntValue7x5Inv(PantallaActiva,l_Temp/10,111,4,2,0);
      else
        PrintIntValue7x5Inv(PantallaActiva,0,111,4,2,0);
      
    }

	  if(l_TopSignal == EV_ON){
      l_TopSignal = EV_OFF;
      PrintCharValue24x13(PantallaActiva,l_TimeRun,12,23,2);
    }


		if(l_TempAlarm == 1 && l_Status != ST_STOP){
      l_TmOffEv    = EV_OFF;
      l_Status     = ST_STOP;  
		  RePaintRgn(PantallaActiva, 1,54,29,63,1); // Stop
		  RePaintRgn(PantallaActiva,46,54,77,63,0); // Pause
		  RePaintRgn(PantallaActiva,92,54,126,63,0); // Start
      SoundStop();
		}



    if(l_TmOffEv == EV_ON){
      l_TmOffEv = EV_OFF;
		  RePaintRgn(PantallaActiva, 1,54,29,63,1); // Stop
		  RePaintRgn(PantallaActiva,46,54,77,63,0); // Pause
		  RePaintRgn(PantallaActiva,92,54,126,63,0); // Start
			SoundStop();      
    }

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      if(     ChkRgn(PantallaActiva,TchX,TchY, 1,50, 29,63,204) == RGN_OK){  // STOP
        CmdButton = STOP_CMD;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 46,50,77,63,203) == RGN_OK){  // Pause
        CmdButton = PAUSE_CMD;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 92,50,126,63,202) == RGN_OK){  // Start
        CmdButton = START_CMD;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,12,23,53,35,254) == RGN_OK){  // Time +  
        if(l_Status != ST_RUN){
          if(TimeUp(RptCntr,&l_TimeRun,20)){
            RptCntr++;
            PrintCharValue24x13(PantallaActiva,l_TimeRun ,12,23,2);
            SoundDw();
          } 
          else{
            RptCntr = 0;
            SoundSelNok();
          }
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,12,36,53,47,253) == RGN_OK){ // Time -
        if(l_Status != ST_RUN){
          if(TimeDw(RptCntr,&l_TimeRun)){
            RptCntr++;
            PrintCharValue24x13(PantallaActiva,l_TimeRun ,12,23,2);
            SoundDw();
          } 
          else{
            RptCntr = 0;
            SoundSelNok();
          }
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 71, 23,127, 35,252) == RGN_OK){  // Power +
        if(PwrUp(RptCntr,&l_Power,100)){
          RptCntr++;
          PrintCharValue24x13(PantallaActiva,l_Power,71,23,3);
          SoundDw();
        } 
        else{
          RptCntr = 0;
          SoundSelNok();
        }
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,71,36,127, 47,251) == RGN_OK){  // Power -
        if(PwrDw(RptCntr,&l_Power)){
          RptCntr++;
          PrintCharValue24x13(PantallaActiva,l_Power,71,23,3);
          SoundDw();
        } 
        else{
          RptCntr = 0;
          SoundSelNok();
        }
      }
      else{                       // esta tocando fuera de region
        ResetInactivity();
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
      RptCntr = 0;
    
      if(CmdButton == STOP_CMD){
        CmdButton = VOID_CMD;
        l_TmOffEv = EV_OFF;
        l_Status  = ST_STOP;  // indica si el equipo esta corriendo
        GoBack = PANT_SEL;
      }
      else if(CmdButton == PAUSE_CMD){
        CmdButton = VOID_CMD;
        if(l_Status == ST_RUN){
          l_TmOffEv = EV_OFF;
          l_Status  = ST_PAUSE;  // indica si el equipo esta corriendo
				  RePaintRgn(PantallaActiva, 1,54,29,63,0); // Stop
				  RePaintRgn(PantallaActiva,46,54,77,63,1); // Pause
				  RePaintRgn(PantallaActiva,92,54,126,63,0); // Start
          SoundPause();
        }
      	
      }
      else if(CmdButton == START_CMD  && l_TempAlarm == 0){
        CmdButton = VOID_CMD;
        if(l_Status != ST_RUN){
          if(l_TimeRun > 0 && l_Power > 0){
				  	RePaintRgn(PantallaActiva, 1,54,29,63,0); // Stop
				  	RePaintRgn(PantallaActiva,46,54,77,63,0); // Pause
		  			RePaintRgn(PantallaActiva,92,54,126,63,1); // Start
            SoundStart();
            l_TopSignal = EV_OFF;
            l_TmOffEv   = EV_OFF;
            l_Status    = ST_RUN;  // indica si el equipo esta corriendo
          } 
          else{
            SoundSelNok();
          }
        }
      }

      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
    }
  }
};



void SetColorMsg(char *PantallaActiva,byte ActOn){
  if(l_Function == LED_WHITE){
    InsertInvRgn(PantallaActiva,&White_Msg,MODE_X1,MODE_Y1,MODE_X2,MODE_Y2,ActOn);
  }
  else if(l_Function == LED_RED  ){
    InsertInvRgn(PantallaActiva,&Red_Msg  ,MODE_X1,MODE_Y1,MODE_X2,MODE_Y2,ActOn);
  }
  else if(l_Function == LED_GREEN){
    InsertInvRgn(PantallaActiva,&Green_Msg,MODE_X1,MODE_Y1,MODE_X2,MODE_Y2,ActOn);
  }
  else if(l_Function == LED_BLUE ){
    InsertInvRgn(PantallaActiva,&Blue_Msg ,MODE_X1,MODE_Y1,MODE_X2,MODE_Y2,ActOn);
  }
  else if(l_Function == LED_AMBAR){
    InsertInvRgn(PantallaActiva,&Ambar_Msg,MODE_X1,MODE_Y1,MODE_X2,MODE_Y2,ActOn);
  }
}



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


char ChkCtrlScr(void){
  byte GoBack  = PANT_VOID;
  byte GoAhead = PANT_VOID;
  byte Rc;

  unsigned char TchX     = 0xFF;
  unsigned char TchY     = 0xFF;

  char *PantallaActiva;
  
  PantallaActiva = &ChkCtrl_Sc;

  g_PantAct = PANT_CHKCTRL;
  DspPantalla((int) PantallaActiva);

//  VersionPrint(PantallaActiva);

 
  PrintIntValue7x5Inv(PantallaActiva,l_PrcHrs,81,28,4,0);
  PrintCharValue7x5Inv(PantallaActiva,l_PrcMin,107,28,2,0);

  PrintIntValue7x5Inv(PantallaActiva,l_TotHrs,81,15,4,0);
  PrintCharValue7x5Inv(PantallaActiva,l_TotMin,107,15,2,0);


  ResetRgn();

  for(;;) {
    RtcIntWork();  

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      if(ChkRgn(PantallaActiva,TchX,TchY,5,39,122,49,1) == RGN_OK){
        GoAhead = PANT_PASSWORD;    // Reset hs parciales (password)                         // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,5,53,33,63,2) == RGN_OK){
        GoBack  = PANT_MENU;    //  vuelve a menu                                           // esta tocando en la zona activa
      }
      else{                       // esta tocando fuera de region
        ResetInactivity();
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
    }
  }  
}




///////////////////////////////////////////////////////////////////////////////////



char ChkPswScr(void){
  byte GoBack              = PANT_VOID;
  byte GoAhead             = PANT_VOID;
  byte GoSilent            = PANT_VOID;
  unsigned char TchX       = 0xFF;
  unsigned char TchY       = 0xFF;
  unsigned int Clave       = 0;
  byte         CmdButton   = VOID_CMD;
  byte         NumberValue = 0;
  byte         NumPos      = 0;
  byte Rc;
  char *PantallaActiva;

  g_PantAct = PANT_PASSWORD; 
  PantallaActiva = &Password_Sc;

  ResetRgn();
  DspPantalla((int) PantallaActiva);

  for(;;) {
    RtcIntWork();  

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      if(ChkRgn(PantallaActiva,TchX,TchY,0,0,15,15,1) == RGN_OK){    // Boton oculto 
		CmdButton   = HIDDEN_CMD;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,87,45,122,59,16) == RGN_OK){  // Enter
		CmdButton   = ENTER_CMD;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,87,22,122,36,15) == RGN_OK){  // Delete
		CmdButton   = DELETE_CMD;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,  0,40,12,61,1) == RGN_OK){  //  1
		CmdButton   = NUMBER_CMD;
        NumberValue = 1;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 17,40,29,61,1) == RGN_OK){  //  2
		CmdButton   = NUMBER_CMD;
        NumberValue = 2;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 34,40,46,61,1) == RGN_OK){  //  3
		CmdButton   = NUMBER_CMD;
        NumberValue = 3;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 51,40,63,61,1) == RGN_OK){  //  4
		CmdButton   = NUMBER_CMD;
        NumberValue = 4;
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY, 68,40,80,61,1) == RGN_OK){  //  5
		CmdButton   = NUMBER_CMD;
        NumberValue = 5;
      }
      else{                       // esta tocando fuera de region
        ResetInactivity();
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
      if(CmdButton == ENTER_CMD){
        CmdButton   = VOID_CMD;
        if(Clave == 12521){
          l_PrcHrs = 0;
          l_PrcMin = 0;
          FlashSaveData();
          SoundWrite();
          GoSilent = PANT_CHKCTRL;                                          
        }
        else{
          SoundSelNok();
          GoSilent = PANT_CHKCTRL;                                          
        }
      }
      else if(CmdButton == DELETE_CMD){
        CmdButton   = VOID_CMD;
        NumPos      = 0;
        Clave       = 0;
        RePaintRgn(PantallaActiva,8,23,78,35,0);
        SoundSelOk();
      }
      else if(CmdButton == HIDDEN_CMD){
        CmdButton   = VOID_CMD;
        if(Clave == 12521){
          GoAhead = PANT_SETTINGS;                                          
        }
        else{
          GoBack = PANT_CHKCTRL;                                          
        }
      	
      }
      else if(CmdButton == NUMBER_CMD){
        CmdButton   = VOID_CMD;
        if(NumPos < 5){
          Clave    = Clave * 10 + NumberValue;
          PrintCharValue9x5Inv(PantallaActiva,NumberValue,21+7*NumPos,26,1,0);
          NumPos++;
          SoundSelOk();
        }
        else{
          SoundSelNok();
        }
      }
      if(GoSilent != PANT_VOID){
        return GoSilent;
      }
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
    }
  } 
};

char SettingsScr(){
  byte GoBack  = PANT_VOID;
  byte GoAhead = PANT_VOID;
  byte GoSilent= PANT_VOID;
  
  byte Rc;
  byte CmdButton = VOID_CMD;

  unsigned char TchX     = 0xFF;
  unsigned char TchY     = 0xFF;

  char *PantallaActiva;

  byte  EdValueMax;
  byte  EdValueMin;
  byte  SetSel = 0;

  g_PantAct = PANT_SETTINGS;
  PantallaActiva = &Settings_Sc;

  ResetRgn();
  DspPantalla((int) PantallaActiva);

  for(;;) {
    RtcIntWork();  

    Rc = TchChk(&TchX,&TchY);
    if(Rc == TOUCH_OK){
      ResetInactivity();

      if(ChkRgn(PantallaActiva,TchX,TchY,1,46,32,62,1) == RGN_OK){   // Back
        GoBack   = PANT_CHKCTRL;    //  vuelve a menu                                           // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,96,46,126,62,1) == RGN_OK){   // Touch
        GoAhead  = PANT_CALIBTOUCH;                                                            // esta tocando en la zona activa
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,47,29,78,45,8) == RGN_OK){  // OK
	    	GoSilent = PANT_CHKCTRL;
      }
      else if(SetSel !=0 && ChkRgn(PantallaActiva,TchX,TchY,76,8,88,18,2) == RGN_OK){  //  +
				CmdButton = UP_CMD; 
	 	 	}
      else if(SetSel !=0 && ChkRgn(PantallaActiva,TchX,TchY,38,8,50,18,3) == RGN_OK){  //  -
				CmdButton = DW_CMD; 
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,1,1,32,17,4) == RGN_OK){  //  Ambar red porcentage
				CmdButton = AMBAR_RED_PORC_CMD; 
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,1,24,32,40,5) == RGN_OK){  // White compose porcentage
      	CmdButton = WHITE_PORC_CMD; 
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,96,1,126,17,6) == RGN_OK){  // Reset time
				CmdButton = RESET_TM_CMD; 
      }
      else if(ChkRgn(PantallaActiva,TchX,TchY,96,24,126,40,7) == RGN_OK){  // TRIM
				CmdButton = TRIM_CMD; 
      }
    }
    else if(Rc == NO_TOUCH){      // no esta tocando el touch
      RgnOff(PantallaActiva);
    
      if(GoAhead != PANT_VOID){
        SoundMenuAd();
        return GoAhead;
      }
      if(GoBack != PANT_VOID){
        SoundMenuBk();
        return GoBack;
      }
      if(GoSilent != PANT_VOID){
        FlashSaveData();
        SoundWrite();
        return GoSilent;
      }
      if(CmdButton == UP_CMD){
      	CmdButton = VOID_CMD;
        if(SetSel == 1){
          if(l_AmbRedPorc < EdValueMax){
		        l_AmbRedPorc++;
	  	      PrintCharValue9x5(PantallaActiva,l_AmbRedPorc,55,8,3);  
          	SoundUp();
          }
          else{
            SoundSelNok();
          }
        }
        else if(SetSel == 2){
          if(l_WhitePorc < EdValueMax){
		        l_WhitePorc++;
	  	      PrintCharValue9x5(PantallaActiva,l_WhitePorc,55,8,3);  
          	SoundUp();
          }
          else{
            SoundSelNok();
          }
        }
        else if(SetSel == 3){
          if(g_Trim < EdValueMax){
		        g_Trim++;
	  	      PrintCharValue9x5(PantallaActiva,g_Trim,55,8,3);  
          	SoundUp();
          }
          else{
            SoundSelNok();
          }
        }
      }
      else if(CmdButton == DW_CMD){
        CmdButton = VOID_CMD;
        if(SetSel == 1){
          if(l_AmbRedPorc > EdValueMin){
	    	    l_AmbRedPorc--;
	      	  PrintCharValue9x5(PantallaActiva,l_AmbRedPorc,55,8,3);  
          	SoundUp();
          }
          else{
            SoundSelNok();
          }
        }
        else if(SetSel == 2){
          if(l_WhitePorc > EdValueMin){
	    	    l_WhitePorc--;
	      	  PrintCharValue9x5(PantallaActiva,l_WhitePorc,55,8,3);  
          	SoundUp();
          }
          else{
            SoundSelNok();
          }
        }
        else if(SetSel == 3){
          if(g_Trim > EdValueMin){
	    	    g_Trim--;
	      	  PrintCharValue9x5(PantallaActiva,g_Trim,55,8,3);  
          	SoundUp();
          }
          else{
            SoundSelNok();
          }
        }
      } 
      else if(CmdButton == AMBAR_RED_PORC_CMD){
        CmdButton = VOID_CMD;
        SetSel = 1;
        EdValueMax = 100;
        EdValueMin = 0;
        PrintCharValue9x5(PantallaActiva,l_AmbRedPorc,55,8,3);  
       	SoundUp();
      } 
      else if(CmdButton == WHITE_PORC_CMD){
        CmdButton = VOID_CMD;
        SetSel = 2;
        EdValueMax = 100;
        EdValueMin = 0;
        PrintCharValue9x5(PantallaActiva,l_WhitePorc,55,8,3);  
       	SoundUp();
      } 
      else if(CmdButton == TRIM_CMD){
        CmdButton = VOID_CMD;
        SetSel = 3;
        EdValueMax = 150;
        EdValueMin = 50;
        PrintCharValue9x5(PantallaActiva,g_Trim,55,8,3);  
       	SoundUp();
      } 
      else if(CmdButton == RESET_TM_CMD){
        CmdButton = VOID_CMD;
        l_PrcMin = 0;        // parcializador del total de minutos de uso del equipo
        l_PrcHrs = 0;        // parcializador del total de horas de uso del equipo
        l_TotMin = 0;        // total de minutos de uso del equipo
        l_TotHrs = 0;        // total de horas de uso del equipo
        FillRgn(PantallaActiva,51,8,75,18,0);
       	SoundUp();
      } 
    }
  } 
};





byte PswUp(byte RptCntr,unsigned int *PswData,unsigned int Limit){

  if(*PswData >= Limit)
    return 0;
  
  if(RptCntr > 10){
    if(*PswData+100 <=  Limit){
      *PswData = *PswData + 100;
      return 1;
    }
    else{
      *PswData = Limit;
      return 1;
    }
  } 
  else if(RptCntr > 5){
    if(*PswData+10 <= Limit){
      *PswData = *PswData + 10;
      return 1;
    }
    else{
      *PswData = Limit;
      return 1;
    }
  } 
  else if(RptCntr > 2){
    if(*PswData+5 <= Limit){
      *PswData = *PswData + 5;
      return 1;
    }
    else{
      *PswData = Limit;
      return 1;
    }
  }
  else{
    if(*PswData < Limit){
      *PswData = *PswData  + 1;
      return 1;
    }
  }
}


byte PswDw(byte RptCntr,unsigned int *PswData){
  if(*PswData == 0)
    return 0;



  if(RptCntr > 10){
    if(*PswData >= 100){
      *PswData = *PswData - 100;
      return 1;
    }
    else{
      *PswData = 0;
      return 1;
    }
  } 
  else if(RptCntr > 5){
    if(*PswData >= 10){
      *PswData= *PswData - 10;
      return 1;
    }
    else{
      *PswData = 0;
      return 1;
    } 
  } 
  else if(RptCntr >2){
    if(*PswData >= 5){
      *PswData= *PswData - 5;
      return 1;
    }
    else{
      *PswData = 0;
      return 1;
    } 
  }
  else{
    if(*PswData > 0){
      *PswData= *PswData - 1;
      return 1;
    }
  }
}



byte TimeUp(byte RptCntr,byte *Time,byte Limit){

  if(*Time >= Limit)
    return 0;
  
  if(RptCntr > 2){
    if(*Time+5 <= Limit){
      *Time = *Time + 5;
      return 1;
    }
    else{
      *Time = Limit;
      return 1;
    }
  }
  else{
    if(*Time < Limit){
      *Time = *Time  + 1;
      return 1;
    }
  }
};


byte TimeDw(byte RptCntr,byte *Time){

  if(*Time == 0)
    return 0;

  if(RptCntr >2){
    if(*Time-5 >= 0){
      *Time= *Time - 5;
      return 1;
    }
    else{
      *Time = 0;
      return 1;
    } 
  }
  else{
    if(*Time > 0){
      *Time= *Time - 1;
      return 1;
    }
  }

}


byte PwrUp(byte RptCntr,byte *Power,byte Limit){
  byte Delta;
  
  if(*Power >= Limit)
    return 0;
  
  if(*Power == 0){
	  *Power = 25;
	  return 1;
  }

  if(*Power+5 <= Limit){
	  *Power = *Power + 5;
	  return 1;
  }
  return 0;
};



byte PwrDw(byte RptCntr,byte *Power){

  if(*Power == 0)
    return 0;

  if(*Power == 25){
    *Power= 0;
    return 1;
  }
 
  if(*Power-5 >= 0){
	  *Power= *Power - 5;
  	return 1;
  }
  
  return 0;
};




// ************* INTERRUPT WORK **************************************
void RtcIntWork(){
  static   unsigned int  SecScaler     = 0;
  static   unsigned int  TempWaitCounter = 0;

  if(g_RtcInt == 0)
    return;
  
  g_RtcInt = 0;

  if(SecScaler == 0){
    RTC_Interrupt();
    IntWorkTimer();  // 1 vez por segundo
    SecScaler = 1150 + (g_Trim-100)*30;          // 64us * 15625 = 1 seg
  }
  else{
    SecScaler--;
  }

  if(l_Status != ST_RUN || l_Power == 0){
	  LED_Red   = 0;
 		LED_Green = 0;
  	LED_Blue  = 0;
  }
  else if(l_Function == LED_WHITE){
	  LED_Red   = (((TPM1MOD * l_WhitePorc )/100) * l_Power) / 100;
	  LED_Green = (((TPM1MOD * l_WhitePorc )/100) * l_Power) / 100;
	  LED_Blue  = (((TPM1MOD * l_WhitePorc )/100) * l_Power) / 100;
  }
  else if(l_Function == LED_RED  ){
	  LED_Red   = (TPM1MOD * l_Power)/100;
	  LED_Green = 0;
	  LED_Blue  = 0;
  }
  else if(l_Function == LED_GREEN){
	  LED_Red   = 0;
	  LED_Green = (TPM1MOD * l_Power)/100;
	  LED_Blue  = 0;
  }
  else if(l_Function == LED_BLUE ){
	  LED_Red   = 0;
	  LED_Green = 0;
	  LED_Blue  = (TPM1MOD * l_Power)/100;
  }
  else if(l_Function == LED_AMBAR){
	  LED_Red   = (((TPM1MOD *      l_AmbRedPorc )/100) * l_Power) / 100; 
	  LED_Green = (((TPM1MOD * (100-l_AmbRedPorc))/100) * l_Power) / 100;
	  LED_Blue  = 0;
  }
  
  ///////////  Debounce del touch ///////////////////////////////////////////////
  if(g_TouchDebounce>0)
    g_TouchDebounce--;
  //////////////////////////////////////////////////////////////////////////////
  if(DS_TempStatus == TMP_WAIT_CONV){
    if(TempWaitCounter > 0){
      TempWaitCounter--;
    }
    else{
      DS_TempStatus = TMP_READY_CONV;
    }
  }
  else if(DS_TempStatus == TMP_START_CONV){
    DS_TempStatus = TMP_WAIT_CONV;
    TempWaitCounter = 1200;
  }



  
  BlinkCounter();
  
  SetDispIntBC();
  
//  SoundInt();
  
}




void IntWorkTimer(){  // 1 vez por segundo
  static byte TimeWait = 59;
  
  if(l_Status == ST_RUN){
    if(l_TimeRun > 0){  
      if(TimeWait == 0){
        l_TimeRun--;
        l_TopSignal = EV_ON;
        TimeWait = 59;
          
        l_PrcMin++;
        if(l_PrcMin == 60){
          l_PrcMin = 0;
          l_PrcHrs++;
        }

        l_TotMin++;
        if(l_TotMin == 60){
          l_TotMin = 0;
          l_TotHrs++;
        }
        FlashSaveData();
      }
      else
        TimeWait--;
    }
    else{
      l_TmOffEv = EV_ON;
      l_Status  = ST_STOP;
    }
  }
  
  l_Temp = ReadTemp();
  if(l_Temp > 350 && l_CoolerFan == 0){
	  COOLER_FAN  = 1;
	  l_CoolerFan = 1;
  }
  else if(l_Temp < 300 && l_CoolerFan == 1){
	  COOLER_FAN  = 0;
    l_CoolerFan = 0;
  }

  if(l_Temp > 600){
  	l_TempAlarm= 1;
  }
  else if(l_Temp < 500){
  	l_TempAlarm= 0;
  }
  
  
}

void BlinkCounter(void){
  l_BlinkCount++;
}


void FlashSaveData(){
  byte          RTC_Sec;
  byte          RTC_Min;
  byte          RTC_Hour;
  byte          RTC_Day;
  byte          RTC_Month;
  unsigned int  RTC_Year;
  byte          Brillo;
  byte          Contraste;
  byte          Volumen;
  
  Brillo    = GetBrillo();
  Contraste = GetContraste();
  Volumen   = GetVolumen();
  
  GetRTC(&RTC_Year,&RTC_Month,&RTC_Day,&RTC_Hour,&RTC_Min,&RTC_Sec);
 
  
  FlashErase((unsigned int) &MemoConfData);
                                                                          
  FlashWriteByte((unsigned int) &MemoConfData +  0, &g_Trim   );
  FlashWriteByte((unsigned int) &MemoConfData +  1, &Brillo   );
  FlashWriteByte((unsigned int) &MemoConfData +  2, &Contraste);
  FlashWriteByte((unsigned int) &MemoConfData +  3, &Volumen  );
  
  FlashWriteByte((unsigned int) &MemoConfData +  4, &g_MinTchY); 
  FlashWriteByte((unsigned int) &MemoConfData +  5, &g_MinTchX); 
  FlashWriteByte((unsigned int) &MemoConfData +  6, &g_MaxTchY);
  FlashWriteByte((unsigned int) &MemoConfData +  7, &g_MaxTchX);
  
  FlashWriteByte((unsigned int) &MemoConfData +  8, &l_PrcMin);
  FlashWriteInt((unsigned int) &MemoConfData  +  9, &l_PrcHrs );
  FlashWriteByte((unsigned int) &MemoConfData + 11, &l_TotMin);
  FlashWriteInt((unsigned int) &MemoConfData  + 12, &l_TotHrs );
  
  FlashWriteByte((unsigned int) &MemoConfData + 14, &RTC_Sec  );
  FlashWriteByte((unsigned int) &MemoConfData + 15, &RTC_Min  );
  FlashWriteByte((unsigned int) &MemoConfData + 16, &RTC_Hour );
  FlashWriteByte((unsigned int) &MemoConfData + 17, &RTC_Day  );
  FlashWriteByte((unsigned int) &MemoConfData + 18, &RTC_Month);
  FlashWriteInt((unsigned int)  &MemoConfData + 19, &RTC_Year );

  FlashWriteInt((unsigned int)  &MemoConfData + 21, &l_AmbRedPorc);
  FlashWriteInt((unsigned int)  &MemoConfData + 23, &l_WhitePorc );
};

void FlashRestoreData(){
  byte          RTC_Sec;
  byte          RTC_Min;
  byte          RTC_Hour;
  byte          RTC_Day;
  byte          RTC_Month;
  unsigned int  RTC_Year;
  byte          Brillo;
  byte          Contraste;
  byte          Volumen;

  g_Trim        = *(&MemoConfData + 0);    //   70  -  130
  Brillo        = *(&MemoConfData + 1);  // 7;
  Contraste     = *(&MemoConfData + 2);  // 6;
  Volumen       = *(&MemoConfData + 3);  // 5;
  g_MinTchY     = *(&MemoConfData + 4); 
  g_MinTchX     = *(&MemoConfData + 5); 
  g_MaxTchY     = *(&MemoConfData + 6);
  g_MaxTchX     = *(&MemoConfData + 7);

  
  l_PrcMin  = *(&MemoConfData + 8);
  memcpy(&l_PrcHrs,(&MemoConfData + 9),sizeof(int));
  l_TotMin      = *(&MemoConfData + 11);
  memcpy(&l_TotHrs,(&MemoConfData + 12),sizeof(int));
  
  RTC_Sec   =  *(&MemoConfData + 14);  // 1;
  RTC_Min   =  *(&MemoConfData + 15);  // 1;
  RTC_Hour  =  *(&MemoConfData + 16);  // 1;
  RTC_Day   =  *(&MemoConfData + 17);  // 1;
  RTC_Month =  *(&MemoConfData + 18);  // 1;
  memcpy(&RTC_Year,(&MemoConfData + 19),sizeof(int));

  memcpy(&l_AmbRedPorc,(&MemoConfData + 21),sizeof(int));
  memcpy(&l_WhitePorc ,(&MemoConfData + 23),sizeof(int));
  
  SetBrillo(Brillo);
  SetContraste(Contraste);
  SetVolumen(Volumen);
  SetRTC(RTC_Year,RTC_Month,RTC_Day,RTC_Hour,RTC_Min,RTC_Sec);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  char CalibTouchScr(){
  char *PantallaActiva;

  byte Key;
  int TchScr;
  int TchScrMin;
  int TchScrMax;
  int TchScrAnt = 0xFFFF;

  unsigned char Times;

  g_PantAct = PANT_CALIBTOUCH;

  PantallaActiva = &Calib_Sc;
  SetInactivity(120);
  DspPantalla((int) PantallaActiva);

  FillRgn(PantallaActiva,0,0,15,15,0);

  TchScrAnt = 0xFFFF;
  Times = 0;
  while(1){
    TchScr = TS_GetTouch();
    if(TchScr == 0xFFFE){
      TchScrAnt = 0xFFFF;
      Times = 0;
    }
    else if(TchScr != 0xFFFF){
      if(TchScr == TchScrAnt){
        if(Times < 10){
          Times++;
        }
        else{
          break;
        }
      }
      else{
        TchScrAnt = TchScr;
        Times = 0;
      }
    }
  }
  TchScrMax = TchScr;
  SoundSelOk();

  FillRgn(PantallaActiva,100,50,127,63,0);

  Times = 0;
  while(1){
    TchScr = TS_GetTouch();
    if(TchScr != 0xFFFF){
      if(TchScr == 0xFFFE)
        Times++;
      else
       Times = 0;
    
      if(Times == 10)
        break;
    }
  }


  RePaintRgn(PantallaActiva,0,0,15,15,0);

  TchScrAnt = 0xFFFF;
  Times = 0;
  while(1){
    TchScr = TS_GetTouch();
    if(TchScr == 0xFFFE){
      TchScrAnt = 0xFFFF;
      Times = 0;
    }
    else if(TchScr != 0xFFFF){
      if(TchScr == TchScrAnt){
        if(Times < 10)
          Times++;
        else
          break;
      }
      else{
        TchScrAnt = TchScr;
        Times = 0;
      }
    }
  }
  TchScrMin = TchScr;
  

  if(SetCalibTch(TchScrMin,TchScrMax)){
    SoundWrite();
  }
  else{
    SoundSelNok();
  }
  return PANT_SETTINGS;
}  


byte SetCalibTch(int TchScrMin,int TchScrMax){
  unsigned char TchX1 = 0xFF;
  unsigned char TchY1 = 0xFF;
  unsigned char TchX2 = 0xFF;
  unsigned char TchY2 = 0xFF;

  TchY1 = (unsigned char)  TchScrMin & 255;
  TchX1 = (unsigned char) (TchScrMin / 256);
  TchY2 = (unsigned char)  TchScrMax & 255;
  TchX2 = (unsigned char) (TchScrMax / 256);
  
  
  if(TchX2 - TchX1 < 100)
    return 0;
  
  if(TchY2 - TchY1 < 55)
    return 0;
  
  g_MinTchX = TchX1 - 5;        
  g_MaxTchX = TchX2 + 5;        
  
  g_MinTchY = TchY1 - 3;
  g_MaxTchY = TchY2 + 3;

  FlashSaveData();
  return 1;
};  





  ///////////////////////////////////////////////////////////////////////////////////////

void VersionPrint(char *PantBuff){
  byte Release = 1;
  unsigned int Compilation = 1;

  PrintCharValue7x5Inv(PantBuff,Release    ,75,3,2,1);
  PrintIntValue7x5Inv(PantBuff ,Compilation,95,3,4,1);
}




int ReadTemp(void){
  int    TempAux     = 0;
  byte   CalcCRC;
  static int  Rc;
  static int  AntRc  = 1;
  
  TempAux = DS_Fac_ReadTemp();
  
  if(TempAux <= 0){
    Rc = 0;
    return Rc;
  }

  CalcCRC = DallasCRC(&DS_DallasData);
  if(CRC_SPad != CalcCRC){
    Rc = 0;
    return Rc;
  }

  if(TempAux == 0x5500){
    Rc = 0;
    return Rc;
  } 

  Rc = (TempAux /256) * 10; 
  Rc = Rc +  (TempAux & 0x00FF);
  AntRc = Rc;
  return Rc;
}


const byte CrcTable [] = {   0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
                           157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
                            35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
                           190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
                            70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
                           219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
                           101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
                           248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
                           140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
                            17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
                           175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
                            50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
                           202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
                            87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
                           233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
                           116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
                         };


byte DallasCRC(byte *buffer){
  byte  crc8  = 0;
  byte  byteCount;

  for(byteCount = 0; byteCount < 8; byteCount++){
  	crc8 = CrcTable[crc8 ^ buffer[byteCount]];
  }

  if(crc8 != buffer[8]){
    return 0xFF;
  }
  return crc8;
}
