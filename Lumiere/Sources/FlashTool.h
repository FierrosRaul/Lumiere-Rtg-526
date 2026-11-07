#ifndef _FLASHTOOLS_H
#define _FLASHTOOLS_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif


extern byte FlashErase( word Dest );
extern byte FlashWrite( word Address, byte *Source, byte Len) ;
extern byte FlashWriteInt(word Address,byte *Source);
extern byte FlashWriteByte(word Address,byte *Source);


#ifdef __cplusplus
  }
#endif

#endif /* _DISPLAY_H */
