#ifndef _TCHSCR_H
#define _TCHSCR_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif




int    TS_GetTouch(void);
extern byte AD_Used;



#ifdef __cplusplus
  }
#endif

#endif /* _TCHSCR_H */
