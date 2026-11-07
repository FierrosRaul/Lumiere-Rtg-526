#include "derivative.h"



     /* Flash access function must be allocated in RAM */
#pragma MESSAGE DISABLE C1805
#define Page_Erase   PGM[21] = 0x40; temp = (( byte (*)( word ))( PGM ))
#define Program_Byte PGM[21] = 0x20; temp = (( byte (*)( word, byte ))( PGM ))
 
/* Array of opcode instructions of the Erase/Program function in the HCS08 family */
volatile unsigned char PGM[64]  = 
{  
        0x87,0xC6,0x18,0x25,0xA5,0x10,0x27,0x08,0xC6,0x18,0x25,0xAA,0x10,0xC7,0x18,0x25,
        0x9E,0xE6,0x01,0xF7,0xA6,0x20,0xC7,0x18,0x26,0x45,0x18,0x25,0xF6,0xAA,0x80,0xF7,
        0x9D,0x9D,0x9D,0x9D,0x45,0x18,0x25,0xF6,0xF7,0xF6,0xA5,0x30,0x27,0x04,0xA6,0xFF,
        0x20,0x07,0xA6,0xFF,0xC7,0x18,0x00,0xC6,0x18,0x25,0xA5,0x40,0x27,0xF4,0x8A,0x81
};


byte FlashErase(word Dest){
  byte temp;
  byte Result = 1;
 
  /* In this routine is not allowed access to flash */
//  Cpu_DisableInt();
  _asm SEI;
 
  temp = Page_Erase(Dest);
  if(temp == 0xFF){
    /* Signal Error */
    Result = 0;
  }
 
//  Cpu_EnableInt();
   _asm CLI;
 /* Signal Ok */
  return( Result );
}
/*-----------------------------------------------------------------------------
** Function Name : FlashWrite
** Description   : Write a block of data in internal Flash
** Input         : Address, Source and length 
** Output        : Result of operations
** Notes         : 
-----------------------------------------------------------------------------*/
byte FlashWrite(word Address,byte *Source,byte Len){
  byte Index, temp;
  byte Result = 1;
 
  /* In this routine is not allowed access to flash */
//  Cpu_DisableInt();
  _asm SEI;

 
  for(Index = 0;Index < Len;Index ++){
//    temp = Program_Byte(Address+Index,(byte) *(Source+Index));
    temp = Program_Byte(Address+Index,(byte) *Source);
    Source +=1;
 
    /* Execute Program routine */
    if(temp == 0xFF){
      /* Signal Error */
      Result = 0;
      break;
    }
  }
 
//  Cpu_EnableInt();
  _asm CLI;
 
  /* Signal Ok */
  return(Result);
}


byte FlashWriteByte(word Address,byte *Source){
  byte temp;
  byte Result = 1;
 
  /* In this routine is not allowed access to flash */
//  Cpu_DisableInt();
  _asm SEI;

  temp = Program_Byte(Address,(byte) *Source);
    /* Execute Program routine */
  if(temp == 0xFF){
    /* Signal Error */
    Result = 0;
  }
 
//  Cpu_EnableInt();
  _asm CLI;
 
  /* Signal Ok */
  return(Result);
}

byte FlashWriteInt(word Address,byte *Source){
  byte temp;
  byte Result = 1;
 
  /* In this routine is not allowed access to flash */

  _asm SEI;  //  Cpu_DisableInt();

  temp = Program_Byte(Address,(byte) *Source);  //  Execute Program routine 
  if(temp == 0xFF){    // Signal Error 
    _asm CLI;
    return(0);
  }

  Source +=1;
  temp = Program_Byte(Address+1,(byte) *Source); // Execute Program routine 
  if(temp == 0xFF){    // Signal Error 
    _asm CLI;
    return(0);
  }

//  Cpu_EnableInt();
  _asm CLI;
  /* Signal Ok */
  return(1);
}

