#ifndef _DISPLAYTOOLS_H
#define _DISPLAYTOOLS_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif

extern void FillRgn(char *PantBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy,unsigned char Inv);
extern void RePaintRgn(char *PantBuff,char StrPx,char StrPy,char EndPx,char EndPy,char Inv);
extern void InsertRgn(char *PantBuff,char *RegnBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy);
extern void InsertInvRgn(char *PantBuff,char *RegnBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy,byte Inv);
extern void PrintCharValue(unsigned char Value,unsigned char Line,unsigned char Row);
extern void PrintChar(char ChData,char Line,char Row);
extern void PrintCharStr(char *ChData,char Line,char Row);
extern void PrintNum24x13(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY);
extern void PrintNum7x5Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv);
extern void PrintCharValue7x5Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
extern void PrintBlankValue7x5Inv(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
extern void PrintIntValue7x5Inv(char *PantBuff,unsigned int Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
extern void PrintNum9x5(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY);
extern void PrintNum9x5Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv);
extern void PrintCharValue9x5Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
extern void PrintCharValue9x5(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt);
extern void PrintIntValue9x5(char *PantBuff,int Value,unsigned char PosX,unsigned char PosY,char Dgt);
extern void PrintBlankValue9x5(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt);
extern void PrintNum30x17(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY);
extern void PrintIntValue30x17(char *PantBuff,int Value,unsigned char PosX,unsigned char PosY,char Dgt);
extern void PrintCharValue24x13(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt);
extern void PrintBlankValue24x13(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt); 
extern void PrintCharValue21x32Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
extern void PrintBlankValue21x32Inv(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
extern void PrintNum21x32Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv);


#define BKLIGHT_MIN   10
#define DIMMER_DELAY  4

extern void SetBrillo(byte Brillo);
extern void SetStandBy(byte StandBy);
extern void DimmerBklight(void);       // INTERRUPT FUNCTION
extern void BkLigth(void);             // INTERRUPT FUNCTION

extern byte g_Brillo;
extern byte g_Contraste;
extern byte g_BrilloDimmer;


#ifdef __cplusplus
  }
#endif


#endif