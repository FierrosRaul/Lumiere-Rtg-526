#ifndef _SCREENDATA_H
#define _SCREENDATA_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif

extern byte *Time_Sc;
extern byte *Welcome_Sc;
extern byte *Menu_Sc;
extern byte *SelMode_Sc;
extern byte *SelFacCrp_Sc;
extern byte *BrCntAud_Sc;
extern byte *ChkCtrl_Sc;
extern byte *Work_Sc;
extern byte *Settings_Sc;
extern byte *Password_Sc;
extern byte *Calib_Sc;
extern byte *CabezalDesc_Sc;
extern byte *SetFHT_Sc;
extern byte *Expo_Msg;

extern byte *Paused_Msg; 
extern byte *Active_Msg;
extern byte *Inactive_Msg;
extern byte *CrpMda_Msg;
extern byte *FacMda_Msg;
extern byte *Decam_Msg;

extern byte *CharMapNum7x5;
extern byte *CharMapNum8x4;
extern byte *CharMapNum9x5;
extern byte *CharMapNum24x13;
extern byte *CharMapNum30x17;
extern byte *CharMapNum21x32;

#ifdef __cplusplus

  }
#endif


#endif

