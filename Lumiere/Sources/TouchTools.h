#ifndef _TOUCHTOOLS_H
#define _TOUCHTOOLS_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif

#define  NOT_READY 0
#define  NO_TOUCH  1
#define  TOUCH_OK  2

#define  NOT_RGN   0
#define  RGN_OK    1

extern void ResetRgn(void);
extern byte TchChk(byte *TchX,byte *TchY);
extern byte ChkRgn(char *PantallaActiva,byte TchX,byte TchY,unsigned char RgnX1,unsigned char RgnY1,unsigned char RgnX2,unsigned char RgnY2,unsigned char TchRgn);
extern void ConvTchCoord(unsigned int TchScr,byte *TchX,byte *TchY);
extern void RgnOff(char *PantallaActiva);

extern byte          g_TouchDebounce;
extern unsigned int  g_MinTchY;
extern unsigned int  g_MinTchX;
extern unsigned int  g_MaxTchY;
extern unsigned int  g_MaxTchX;

#ifdef __cplusplus
  }
#endif

#endif /* _TOUCHTOOLS_H */
