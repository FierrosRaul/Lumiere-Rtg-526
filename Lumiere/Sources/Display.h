#ifndef _DISPLAY_H
#define _DISPLAY_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif

void asm_main(void);
  /* interface to my assembly main function */
void ClrDsp(void);
void DspScreenInit(void);
void Wait(void);
void DspPantalla(int Pantalla);

void LcdWrtPg(char YPos,char XPage,char Data);
void LcdWriteData(char LcdData);
void LcdWriteCmd(char LcdCmd);


extern unsigned char *CharMap8x8;
/*
extern unsigned char *CharMapNum8x4;
extern unsigned char *CharMapNum7x5;
extern unsigned char *CharMapNum9x5;
extern unsigned char *CharMapNum24x13;
extern unsigned char *CharMapNum30x17;
extern unsigned char *CharMapNum21x32;
*/

#ifdef __cplusplus
  }
#endif

#endif /* _DISPLAY_H */
