#include "derivative.h" /* include peripheral declarations */
#include "Ports.h"
#include "DisplayTools.h"
#include "Display.h"
#include "lumiere.h"

////////////////////////  Variables Globales  /////////////////////////////////////////////////////////

byte g_Brillo    = 10;
byte g_Contraste = 10;
byte g_BrilloDimmer = 10;

////////////////////////  Variables Locales  /////////////////////////////////////////////////////////
byte l_StandBy =  0;

void FillRgn(char *PantBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy,unsigned char Inv);
void RePaintRgn(char *PantBuff,char StrPx,char StrPy,char EndPx,char EndPy,char Inv);
void InsertRgn(char *PantBuff,char *RegnBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy);
void InsertInvRgn(char *PantBuff,char *RegnBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy,byte Inv);


void PrintCharValue(unsigned char Value,unsigned char Line,unsigned char Row);
void PrintChar(char ChData,char Line,char Row);
void PrintCharStr(char *ChData,char Line,char Row);
void PrintNum24x13(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY);
void PrintNum7x5Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv);
void PrintCharValue7x5Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
void PrintIntValue7x5Inv(char *PantBuff,unsigned int Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
void PrintBlankValue7x5Inv(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
void PrintNum9x5(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY);
void PrintNum9x5Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv);
void PrintCharValue9x5Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
void PrintCharValue9x5(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt);
void PrintIntValue9x5(char *PantBuff,int Value,unsigned char PosX,unsigned char PosY,char Dgt);
void PrintBlankValue9x5(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt);
void PrintNum30x17(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY);
void PrintIntValue30x17(char *PantBuff,int Value,unsigned char PosX,unsigned char PosY,char Dgt);
void PrintCharValue24x13(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt);
void PrintBlankValue24x13(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt);
void PrintCharValue21x32Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
void PrintBlankValue21x32Inv(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv);
void PrintNum21x32Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetBrillo(byte Brillo){
  g_Brillo = Brillo;
}

void SetStandBy(byte StandBy){
  l_StandBy = StandBy;
}

//////////////////  DISPLAY   //////////////////////////////////////////////////////////
void FillRgn(char *PantBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy,unsigned char Inv){
  unsigned char Px;
  unsigned char aux;
  char *Buff;
  char Data;
  unsigned char StrLn;
  unsigned char EndLn;
  unsigned char StrLnRst;
  unsigned char EndLnRst;
  unsigned char Ly;
  unsigned char ByteXor;
  
  char IniLine[] = {0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80};
  char EndLine[] = {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};

  aux   = 127 - EndPx;
  EndPx = 127 - StrPx;
  StrPx = aux;
  
  aux   = 63  - EndPy;
  EndPy = 63  - StrPy;
  StrPy = aux;
      
  StrLn    = StrPy >>3;
  EndLn    = EndPy >>3;
  StrLnRst = StrPy & 7;
  EndLnRst = EndPy & 7;
  
  for(Ly=StrLn;Ly<=EndLn;Ly++){
     Buff= PantBuff + (StrPx + Ly * 128);
     for(Px=StrPx;Px<=EndPx;Px++){
      if(StrLn == EndLn){
        ByteXor = IniLine[StrLnRst] & EndLine[EndLnRst];
      }
      else if(Ly==StrLn){
        ByteXor = IniLine[StrLnRst];
      }
      else if(Ly==EndLn){
        ByteXor = EndLine[EndLnRst];
      }
      else{
        ByteXor = 0xFF;
      }
      
      if(Inv != 0)
        Data = *Buff | ByteXor;
      else
        Data = *Buff & ~ByteXor;
      
      
      LcdWrtPg(Px,Ly,Data);
      Buff++;
    }
  }
}


void RePaintRgn(char *PantBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy,unsigned char Inv){
  unsigned char Px;
  unsigned char aux;
  char *Buff;
  char Data;
  unsigned char StrLn;
  unsigned char EndLn;
  unsigned char StrLnRst;
  unsigned char EndLnRst;
  unsigned char Ly;
  unsigned char ByteXor;

    
  
  char IniLine[] = {0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80};
  char EndLine[] = {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};

  aux   = 127 - EndPx;
  EndPx = 127 - StrPx;
  StrPx = aux;
  
  aux   = 63  - EndPy;
  EndPy = 63  - StrPy;
  StrPy = aux;
      
  StrLn    = StrPy >>3;
  EndLn    = EndPy >>3;
  StrLnRst = StrPy & 7;
  EndLnRst = EndPy & 7;
  
  for(Ly=StrLn;Ly<=EndLn;Ly++){
     Buff= PantBuff + (StrPx + Ly * 128);
     for(Px=StrPx;Px<=EndPx;Px++){
      if(Inv != 0){
        if(StrLn == EndLn){
          ByteXor = IniLine[StrLnRst] & EndLine[EndLnRst];
        }
        else if(Ly==StrLn){
          ByteXor = IniLine[StrLnRst];
        }
        else if(Ly==EndLn){
          ByteXor = EndLine[EndLnRst];
        }
        else{
          ByteXor = 0xFF;
        }
        Data = *Buff ^ ByteXor;
      }
      else{
        Data = *Buff;
      }
      LcdWrtPg(Px,Ly,Data);
      Buff++;
    }
  }
}



void InsertRgn(char *PantBuff,char *RegnBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy){
  unsigned char Px;
  unsigned char aux;
  char *Buff;
  char *BuffRgn;
  unsigned char Data;
  unsigned char StrLn;
  unsigned char EndLn;
  unsigned char StrLnRst;
  unsigned char EndLnRst;
  unsigned char Ly;
  unsigned char ByteAnd;
  unsigned char RegnWidth;
  unsigned char RegnHeight;
  unsigned char RegnLy;
  
  char IniLine[] = {0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80};
  char EndLine[] = {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
  
  aux   = 127 - EndPx ;
  EndPx = 127 - StrPx;
  StrPx = aux;
  
  aux   = 63  - EndPy;
  EndPy = 63  - StrPy;
  StrPy = aux;
    
  StrLn    = StrPy >>3;
  EndLn    = EndPy >>3;
  StrLnRst = StrPy & 7;
  EndLnRst = EndPy & 7;
   
  RegnWidth  = EndPx - StrPx + 1; 
  RegnHeight = EndPy - StrPy + 1;
  
  for(Ly=StrLn;Ly<=EndLn;Ly++){
    RegnLy  = Ly - StrLn;
    Buff    = PantBuff + (StrPx + Ly * 128);
    BuffRgn = RegnBuff + (RegnLy * RegnWidth);
    
    for(Px = StrPx;Px <= EndPx;Px++){
      if(Ly == StrLn){
        if(StrPy != EndPy)
          Data = *BuffRgn << StrLnRst | *Buff & ~IniLine[StrLnRst];
        else
          Data = (*BuffRgn << StrLnRst & IniLine[StrLnRst] & EndLine[EndLnRst]) | (*Buff & ~IniLine[StrLnRst]) | (*Buff & ~EndLine[EndLnRst]) ;
      }
      else if(Ly == EndLn){
//        if(RegnHeight & 7)
          Data = ((*BuffRgn << StrLnRst | *(BuffRgn-RegnWidth) >> (8-StrLnRst)) & EndLine[EndLnRst]) | (*Buff & ~EndLine[EndLnRst]) ;
//        else
//          Data = *(BuffRgn-RegnWidth) >> (8-StrLnRst) | *Buff & ~EndLine[EndLnRst] ;
      }
      else{
        Data = *BuffRgn << StrLnRst | *(BuffRgn-RegnWidth) >> (8-StrLnRst);
      }
      LcdWrtPg(Px,Ly,Data);
      Buff++;
      BuffRgn++;
    }
  }
}

void InsertInvRgn(char *PantBuff,char *RegnBuff,unsigned char StrPx,unsigned char StrPy,unsigned char EndPx,unsigned char EndPy,byte Inv){
  unsigned char Px;
  unsigned char aux;
  char *Buff;
  char *BuffRgn;
  unsigned char Data;
  unsigned char StrLn;
  unsigned char EndLn;
  unsigned char StrLnRst;
  unsigned char EndLnRst;
  unsigned char Ly;
  unsigned char ByteAnd;
  unsigned char RegnWidth;
  unsigned char RegnHeight;
  unsigned char RegnLy;
  
  char BuffRawData;
  char BuffRawDataAnt;
  
  char IniLine[] = {0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80};
  char EndLine[] = {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
  
  aux   = 127 - EndPx ;
  EndPx = 127 - StrPx;
  StrPx = aux;
  
  aux   = 63  - EndPy;
  EndPy = 63  - StrPy;
  StrPy = aux;
    
  StrLn    = StrPy >>3;
  EndLn    = EndPy >>3;
  StrLnRst = StrPy & 7;
  EndLnRst = EndPy & 7;
   
  RegnWidth  = EndPx - StrPx + 1; 
  RegnHeight = EndPy - StrPy + 1;
  
  for(Ly=StrLn;Ly<=EndLn;Ly++){
    RegnLy  = Ly - StrLn;
    Buff    = PantBuff + (StrPx + Ly * 128);
    BuffRgn = RegnBuff + (RegnLy * RegnWidth);
    
    for(Px = StrPx;Px <= EndPx;Px++){

      if(Inv ==0){
        BuffRawData    = *BuffRgn;
        BuffRawDataAnt = *(BuffRgn-RegnWidth); 
      }
      else{
        BuffRawData    = *BuffRgn ^ 0xFF;
        BuffRawDataAnt = *(BuffRgn-RegnWidth) ^ 0xFF; 
      }

      if(Ly == StrLn){
        if(StrPy != EndPy)
          Data = BuffRawData << StrLnRst | *Buff & ~IniLine[StrLnRst];
        else
          Data = (BuffRawData << StrLnRst & IniLine[StrLnRst] & EndLine[EndLnRst]) | (*Buff & ~IniLine[StrLnRst]) | (*Buff & ~EndLine[EndLnRst]) ;
      }
      else if(Ly == EndLn){
//        if(RegnHeight & 7)
          Data = ((BuffRawData << StrLnRst | BuffRawDataAnt >> (8-StrLnRst)) & EndLine[EndLnRst]) | (*Buff & ~EndLine[EndLnRst]) ;
//        else
//          Data = BuffRawDataAnt >> (8-StrLnRst) | *Buff & ~EndLine[EndLnRst] ;
      }
      else{
        Data = BuffRawData << StrLnRst | BuffRawDataAnt >> (8-StrLnRst);
      }
      LcdWrtPg(Px,Ly,Data);
      Buff++;
      BuffRgn++;
    }
  }
}




void PrintChar(char ChData,char Line,char Row){
  char *Buff;
  char YPos;
  char XPos;
  char aux;
  
  if(Row>15 | Line>7)
    return;
  
  YPos = (15-Row) * 8;
  XPos = 7-Line;
  
  Buff = (char *) &CharMap8x8 + (ChData * 8);
  
  for(aux = 0;aux<8;aux++){
    LcdWrtPg(YPos+aux,XPos,*Buff);
    Buff++;
  }
}

void PrintCharValue(unsigned char Value,unsigned char Line,unsigned char Row){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;

	if(Cnt == 0){
		PrintChar(' ',Line,Row);
		if(Dec == 0){
			PrintChar(' ',Line,Row+1);
		}
		else{
			PrintChar('0'+Dec,Line,Row+1);
		}
	}
	else{
		PrintChar('0'+Cnt,Line,Row);
		PrintChar('0'+Dec,Line,Row+1);
	}
	PrintChar('0'+Value,Line,Row+2);
	

};

void PrintCharStr(char *ChData,char Line,char Row){
  char *Buff;
  char YPos;
  char XPos;
  char aux;
  char aux1;
  
  if(Row>15 | Line>7)
    return;
  
  while(*ChData !=0){
    YPos = (15-Row) * 8;
    XPos = 7-Line;
    Buff = (char *) &CharMap8x8 + (*ChData * 8);
    for(aux = 0;aux<8;aux++){
      LcdWrtPg(YPos+aux,XPos,*Buff);
      Buff++;
    }
    ChData++;
    Row++;
    if(Row>15 | Line>7)
      return;
  }
}

void PrintIntValue7x5Inv(char *PantBuff,unsigned int Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv){
	unsigned char DMil = 0;
	unsigned char Mil = 0;
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>9999;Value -=10000)
		DMil++;
	for(;Value>999;Value -=1000)
		Mil++;
	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;
		
  if(Dgt == 5){
  	PrintNum7x5Inv(PantBuff,Mil,PosX,PosY,Inv);
  	PrintNum7x5Inv(PantBuff,Mil,PosX+6,PosY,Inv);
  	PrintNum7x5Inv(PantBuff,Cnt,PosX+12,PosY,Inv);
	  PrintNum7x5Inv(PantBuff,Dec,PosX+18,PosY,Inv);
    PrintNum7x5Inv(PantBuff,Value,PosX+24,PosY,Inv);
  }
  else if(Dgt == 4){
  	PrintNum7x5Inv(PantBuff,Mil,PosX,PosY,Inv);
  	PrintNum7x5Inv(PantBuff,Cnt,PosX+6,PosY,Inv);
	  PrintNum7x5Inv(PantBuff,Dec,PosX+12,PosY,Inv);
    PrintNum7x5Inv(PantBuff,Value,PosX+18,PosY,Inv);
  }
  else if(Dgt == 3){
  	PrintNum7x5Inv(PantBuff,Cnt,PosX,PosY,Inv);
	  PrintNum7x5Inv(PantBuff,Dec,PosX+6,PosY,Inv);
    PrintNum7x5Inv(PantBuff,Value,PosX+12,PosY,Inv);
  }
  else{
	  PrintNum7x5Inv(PantBuff,Dec,PosX,PosY,Inv);
    PrintNum7x5Inv(PantBuff,Value,PosX+6,PosY,Inv);
  }
};


void PrintCharValue7x5Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;
		
  if(Dgt == 3){
  	PrintNum7x5Inv(PantBuff,Cnt,PosX,PosY,Inv);
	  PrintNum7x5Inv(PantBuff,Dec,PosX+6,PosY,Inv);
    PrintNum7x5Inv(PantBuff,Value,PosX+12,PosY,Inv);
  }
  else{
	  PrintNum7x5Inv(PantBuff,Dec,PosX,PosY,Inv);
    PrintNum7x5Inv(PantBuff,Value,PosX+6,PosY,Inv);
  }
};


void PrintCharValue9x5Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;
		
  if(Dgt == 3){
  	PrintNum9x5Inv(PantBuff,Cnt,PosX,PosY,Inv);
	  PrintNum9x5Inv(PantBuff,Dec,PosX+6,PosY,Inv);
    PrintNum9x5Inv(PantBuff,Value,PosX+12,PosY,Inv);
  }
  else if(Dgt == 2){
	  PrintNum9x5Inv(PantBuff,Dec,PosX,PosY,Inv);
    PrintNum9x5Inv(PantBuff,Value,PosX+6,PosY,Inv);
  }
  else{
    PrintNum9x5Inv(PantBuff,Value,PosX+6,PosY,Inv);
  }
};



void PrintIntValue9x5(char *PantBuff,int Value,unsigned char PosX,unsigned char PosY,char Dgt){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;
	unsigned char Uni = 0;

  if(Value <= 0){
    Cnt = 0;
	  Dec = 0;
	  Uni = 0;
  }
  else{
  	for(;Value>99;Value -=100)
	  	Cnt++;
	  for(;Value>9;Value -=10)
	  	Dec++;
	  Uni = (unsigned char) Value;
  }
		
  if(Dgt == 3){
  	PrintNum9x5(PantBuff,Cnt,PosX   ,PosY);         //  55
	  PrintNum9x5(PantBuff,Dec,PosX+6,PosY);      //  78
    PrintNum9x5(PantBuff,Uni,PosX+12,PosY);    //  101
  }
  else if(Dgt == 2){
	  PrintNum9x5(PantBuff,Dec,PosX   ,PosY);
    PrintNum9x5(PantBuff,Uni,PosX+6,PosY);
  }
  else{
    PrintNum9x5(PantBuff,Uni,PosX   ,PosY);
  }
  
};





void PrintCharValue9x5(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;
		
  if(Dgt == 3){
  	PrintNum9x5(PantBuff,Cnt,PosX,PosY);
	  PrintNum9x5(PantBuff,Dec,PosX+6,PosY);
    PrintNum9x5(PantBuff,Value,PosX+12,PosY);
  }
  else{
	  PrintNum9x5(PantBuff,Dec,PosX,PosY);
    PrintNum9x5(PantBuff,Value,PosX+6,PosY);
  }
};


void PrintBlankValue9x5(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt){
  PrintNum9x5(PantBuff,10,PosX,PosY);
	PrintNum9x5(PantBuff,10,PosX+6,PosY);
  if(Dgt == 3)
    PrintNum9x5(PantBuff,10,PosX+12,PosY);
};


void PrintNum9x5(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY){
  char *Buff;
  char aux;
  
  if(PosX > 123 | PosY >55 | Num >10)
    return;
                    
  Buff = (char *) &CharMapNum9x5 + (10*Num);
  InsertInvRgn(PantBuff,Buff,PosX,PosY,PosX+4,PosY+8,0);
}

void PrintNum9x5Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv){
  char *Buff;
  char aux;
  
  if(PosX > 123 | PosY >55 | Num >10)
    return;
                    
  Buff = (char *) &CharMapNum9x5 + (10*Num);
  InsertInvRgn(PantBuff,Buff,PosX,PosY,PosX+4,PosY+8,Inv);
}


void PrintBlankValue7x5Inv(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv){
  PrintNum7x5Inv(PantBuff,10,PosX,PosY,Inv);
	PrintNum7x5Inv(PantBuff,10,PosX+6,PosY,Inv);
  if(Dgt >= 3)
    PrintNum7x5Inv(PantBuff,10,PosX+12,PosY,Inv);
  if(Dgt == 4)
    PrintNum7x5Inv(PantBuff,10,PosX+18,PosY,Inv);

  
};

void PrintNum7x5Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv){
  char *Buff;
  char aux;
  
  if(PosX > 123 | PosY >57 | Num >10)
    return;
                    
  Buff = (char *) &CharMapNum7x5 + (5*Num);
  InsertInvRgn(PantBuff,Buff,PosX,PosY,PosX+4,PosY+6,Inv);
}




void PrintNum24x13(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY){
  char *Buff;
  char aux;
  
  if(PosX > 115 | PosY >40 | Num >10)
    return;
  
  Buff = (char *) &CharMapNum24x13 + (39*Num);
  InsertInvRgn(PantBuff,Buff,PosX,PosY,PosX+12,PosY+23,0);
}



void PrintCharValue24x13(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;
		
  if(Dgt == 3){
  	PrintNum24x13(PantBuff,Cnt,PosX,PosY);
	  PrintNum24x13(PantBuff,Dec,PosX+16,PosY);
    PrintNum24x13(PantBuff,Value,PosX+32,PosY);
  }
  else if(Dgt == 2){
	  PrintNum24x13(PantBuff,Dec,PosX,PosY);
    PrintNum24x13(PantBuff,Value,PosX+16,PosY);
  }
  else{
    PrintNum24x13(PantBuff,Value,PosX,PosY);
  }
  
};


void PrintBlankValue24x13(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt){
		
  if(Dgt == 3){
  	PrintNum24x13(PantBuff,10,PosX,PosY);
	  PrintNum24x13(PantBuff,10,PosX+16,PosY);
    PrintNum24x13(PantBuff,10,PosX+32,PosY);
  }
  else if(Dgt == 2){
	  PrintNum24x13(PantBuff,10,PosX,PosY);
    PrintNum24x13(PantBuff,10,PosX+16,PosY);
  }
  else{
    PrintNum24x13(PantBuff,10,PosX,PosY);
  }
  
};

void PrintIntValue30x17(char *PantBuff,int Value,unsigned char PosX,unsigned char PosY,char Dgt){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;
	unsigned char Uni = 0;

  if(Value <= 0){
    Cnt = 0;
	  Dec = 0;
	  Uni = 0;
  }
  else{
  	for(;Value>99;Value -=100)
	  	Cnt++;
	  for(;Value>9;Value -=10)
	  	Dec++;
	  Uni = (unsigned char) Value;
  }
		
  if(Dgt == 3){
  	PrintNum30x17(PantBuff,Cnt,PosX   ,PosY);         //  55
	  PrintNum30x17(PantBuff,Dec,PosX+23,PosY);      //  78
    PrintNum30x17(PantBuff,Uni,PosX+46,PosY);    //  101
  }
  else if(Dgt == 2){
	  PrintNum30x17(PantBuff,Dec,PosX   ,PosY);
    PrintNum30x17(PantBuff,Uni,PosX+23,PosY);
  }
  else{
    PrintNum30x17(PantBuff,Uni,PosX   ,PosY);
  }
  
};




void PrintCharValue30x17(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;
		
  if(Dgt == 3){
  	PrintNum30x17(PantBuff,Cnt,PosX,PosY);
	  PrintNum30x17(PantBuff,Dec,PosX+23,PosY);
    PrintNum30x17(PantBuff,Value,PosX+46,PosY);
  }
  else if(Dgt == 2){
	  PrintNum30x17(PantBuff,Dec,PosX,PosY);
    PrintNum30x17(PantBuff,Value,PosX+23,PosY);
  }
  else{
    PrintNum30x17(PantBuff,Value,PosX,PosY);
  }
  
};




void PrintNum30x17(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY){
  char *Buff;
  char aux;
  
  if(PosX > 111 | PosY >34 | Num >10)
    return;
                    
  Buff = (char *) &CharMapNum30x17 + (68*Num);
  InsertInvRgn(PantBuff,Buff,PosX,PosY,PosX+16,PosY+29,0);
}


void PrintBlankValue21x32Inv(char *PantBuff,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv){
  PrintNum21x32Inv(PantBuff,10,PosX,PosY,Inv);
	PrintNum21x32Inv(PantBuff,10,PosX+24,PosY,Inv);
  if(Dgt == 3)
    PrintNum21x32Inv(PantBuff,10,PosX+48,PosY,Inv);
};


void PrintCharValue21x32Inv(char *PantBuff,unsigned char Value,unsigned char PosX,unsigned char PosY,char Dgt,byte Inv){
	unsigned char Cnt = 0;
	unsigned char Dec = 0;

	for(;Value>99;Value -=100)
		Cnt++;
	for(;Value>9;Value -=10)
		Dec++;
		
  if(Dgt == 3){
  	PrintNum21x32Inv(PantBuff,Cnt,PosX,PosY,Inv);
	  PrintNum21x32Inv(PantBuff,Dec,PosX+24,PosY,Inv);
    PrintNum21x32Inv(PantBuff,Value,PosX+48,PosY,Inv);
  }
  else if(Dgt == 2){
	  PrintNum21x32Inv(PantBuff,Dec,PosX,PosY,Inv);
    PrintNum21x32Inv(PantBuff,Value,PosX+24,PosY,Inv);
  }
  else{
    PrintNum21x32Inv(PantBuff,Value,PosX,PosY,Inv);
  }
  
};



void PrintNum21x32Inv(char *PantBuff,unsigned char Num,unsigned char PosX,unsigned char PosY,byte Inv){
  char *Buff;
  char aux;
  
  if(PosX > 108 | PosY >34 | Num >11)
    return;
                    
  Buff = (char *) &CharMapNum21x32 + (84*Num);
  InsertInvRgn(PantBuff,Buff,PosX,PosY,PosX+20,PosY+31,Inv);
}





/////////////////////  INTERRUPT  ///////////////////////////////////////////////////////////////////

void DimmerBklight(void){
static unsigned int DimmerScaler    = 0;
static unsigned int BrilloDimmerAnt = 0;

  if(l_StandBy == 1){
    if(DimmerScaler == DIMMER_DELAY){
      DimmerScaler = 0;
      if(g_BrilloDimmer > BKLIGHT_MIN){
        g_BrilloDimmer -= 2;
      }
    }
    else{
      DimmerScaler++;
    }                
  } 
  else{
    g_BrilloDimmer = 10 + g_Brillo * 9;
  }
  
  if(BrilloDimmerAnt != g_BrilloDimmer ){
    BK_LIGHT_PWM    =  g_BrilloDimmer * l_Tmr_End / 100;
    BrilloDimmerAnt = g_BrilloDimmer; 
  }
}

void BkLigth(void){      // Seteo del PWM
   BK_LIGHT_PWM = g_BrilloDimmer;
}


