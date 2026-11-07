#ifndef _SOUNDSTOOLS_H
#define _SOUNDSTOOLS_H



extern void SoundMenuAd(void);
extern void SoundMenuBk(void);
extern void SoundUp(void);
extern void SoundDw(void);
extern void SoundSelOk(void);
extern void SoundSelNok(void);
extern void SoundStart(void);
extern void SoundStop(void);
extern void SoundPause(void);
extern void SoundWlcm(void);
extern void SoundWrite(void);
extern void SoundDelete(void);
extern void SoundMinTop(void);
extern void SoundTempStop(void);


extern byte GetVolumen(void);
extern void SetVolumen(byte Volumen);
extern byte VolumenUp(void);
extern byte VolumenDw(void);

extern void SoundInt(void);

#endif
