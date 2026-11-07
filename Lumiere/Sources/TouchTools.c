#include "derivative.h"
#include "TchScr.h"
#include "TouchTools.h"
#include "DisplayTools.h"

void ResetRgn(void);
byte TchChk(byte *TchX,byte *TchY);
void ConvTchCoord(unsigned int TchScr,byte *TchX,byte *TchY);
byte ChkRgn(char *PantallaActiva,unsigned char TchX,unsigned char TchY,unsigned char RgnX1,unsigned char RgnY1,unsigned char RgnX2,unsigned char RgnY2,unsigned char TchRgn);


byte          g_TouchDebounce  =   0;
unsigned int  g_MinTchY        =  69;
unsigned int  g_MinTchX        =  43;
unsigned int  g_MaxTchY        = 140;
unsigned int  g_MaxTchX        = 199;

unsigned char l_TchRgnAnt      =   0;
unsigned char l_RgnAnX1        = 0xFF;
unsigned char l_RgnAnY1        = 0xFF;
unsigned char l_RgnAnX2        = 0xFF;
unsigned char l_RgnAnY2        = 0xFF;

/*
byte l_TchStatus               = TCH_READY;


byte TchGetTouch(){

  if(l_TchStatus == TCH_READY){
  	X_PORT_AD  = 0;  //  X AD off
  	Y_PORT_AD  = 0;  //  Y AD off
  	
  	SetPortOut(PORT_X1,0);
	SetPortIn(PORT_Y2,0);
	SetPortIn(PORT_X2,0);
	SetPortIn(PORT_Y1,1);
    
    if(Y1_PORT == 1){
      return NO_TOUCH;
    }
    
    if(CheckADs != AD_IN_USE){
      return NOT_READY;
    }
    
    CheckADs = AD_IN_USE;    
    ADCCFG     = 0x71;  // High speed 0, clock/8 11,Long sample 1, 8 bits 00, Bus clk/2 01 
  	Y_PORT_AD  = 0;  //  Y AD off
    
  	SetPortOut(PORT_Y1,1);
  	SetPortOut(PORT_X2,0);
	SetPortIn(PORT_Y2,0);

  	X_PORT_AD  = 1;  //  X AD on
	
    
    
  	
  }
  else if(l_TchStatus == TCH_READY){
  	
  }

	
}

void SetPortOut(byte Port ,byte SetOut){
  if(Port == PORT_Y1){
    Y1_PORT_DR  = 1;  // Port out
    Y1_PORT_PU  = 0;  // Port pull up off
    Y1_PORT_SE  = 0   // Port Slew rate off
    Y1_PORT_DS  = 0   // Port Drive Strength off
    Y1_PORT     = SetOut;
  }
  else if(Port == PORT_X1){
    X1_PORT_DR  = 1;  // Port out
    X1_PORT_PU  = 0;  // Port pull up off
    X1_PORT_SE  = 0   // Port Slew rate off
    X1_PORT_DS  = 0   // Port Drive Strength off
    X1_PORT     = SetOut;
  }
  else if(Port == PORT_X2){
    X2_PORT_DR  = 1;  // Port out
    X2_PORT_PU  = 0;  // Port pull up off
    X2_PORT_SE  = 0   // Port Slew rate off
    X2_PORT_DS  = 0   // Port Drive Strength off
    X2_PORT     = SetOut;
  }
  else if(Port == PORT_Y2){
    Y2_PORT_DR  = 1;  // Port out
    Y2_PORT_PU  = 0;  // Port pull up off
    Y2_PORT_DS  = 0   // Port Slew rate off
    Y2_PORT_DS  = 0   // Port Drive Strength off
    Y2_PORT_DS  = SetOut;
  }
}

void SetPortIn(byte Port ,byte SetPU){
  if(Port == PORT_Y1){
    Y1_PORT_DR  = 0;      // Port out
    Y1_PORT_PU  = SetPU;  // Port pull up off
    Y1_PORT_SE  = 0       // Port Slew rate off
    Y1_PORT_DS  = 0       // Port Drive Strength off
  }
  else if(Port == PORT_X1){
    X1_PORT_DR  = 0;      // Port out
    X1_PORT_PU  = SetPU;  // Port pull up off
    X1_PORT_SE  = 0       // Port Slew rate off
    X1_PORT_DS  = 0       // Port Drive Strength off
  }
  else if(Port == PORT_X2){
    X2_PORT_DR  = 0;      // Port out
    X2_PORT_PU  = SetPU;  // Port pull up off
    X2_PORT_SE  = 0       // Port Slew rate off
    X2_PORT_DS  = 0       // Port Drive Strength off
  }
  else if(Port == PORT_Y2){
    Y2_PORT_DR  = 0;      // Port out
    Y2_PORT_PU  = SetPU;  // Port pull up off
    Y2_PORT_DS  = 0       // Port Slew rate off
    Y2_PORT_DS  = 0       // Port Drive Strength off
  }
}

*/


byte TchChk(byte *TchX,byte *TchY){
  int        TchScr;
  static int TchScrAnt = 0xFFFF;

  *TchX = 255;	
  *TchY = 255;	


  if(g_TouchDebounce > 0){
    return NOT_READY;
  }

  TchScr = TS_GetTouch();
  if(TchScr == 0xFFFF){
    return NOT_READY;
  }

  g_TouchDebounce = 2;

  if(TchScr == TchScrAnt ){
    if(TchScr == 0xFFFE){
      return NO_TOUCH;
    }
    else{
      ConvTchCoord(TchScr,TchX,TchY);
      return TOUCH_OK;
    }
  }
  else{
    TchScrAnt = TchScr; 
  }

  return NOT_READY;
}


void ResetRgn(){
  l_TchRgnAnt = 0;
  l_RgnAnX1   = 0xFF;
  l_RgnAnY1   = 0xFF;
  l_RgnAnX2   = 0xFF;
  l_RgnAnY2   = 0xFF;
}

void ConvTchCoord(unsigned int TchScr,byte *TchX,byte *TchY){
  unsigned int  Xd;
  unsigned int  Yd;
  unsigned int  Xoff;
  unsigned int  Yoff;
  
  unsigned int  RelX;
  unsigned int  RelY;
  
  if((g_MaxTchX - g_MinTchX) < 100 || (g_MaxTchY - g_MinTchY) < 55){
    g_MinTchY =  69;
    g_MinTchX =  43;
    g_MaxTchY = 140;
    g_MaxTchX = 199;
  }
  
  RelX = 16256/(g_MaxTchX - g_MinTchX);
  RelY =  8064/(g_MaxTchY - g_MinTchY);
  
  Xd  = (unsigned int) (TchScr / 256); 
  Yd  = (unsigned int) (TchScr & 255);
  
  if(Xd > g_MinTchX)	
    if(Xd < g_MaxTchX)	
      Xoff  = (Xd - g_MinTchX);
    else
      Xoff  = (g_MaxTchX - g_MinTchX);
  else
    Xoff  = 0;
  
  if(Yd > g_MinTchY)
    if(Yd < g_MaxTchY)
      Yoff  = (Yd - g_MinTchY);
    else
      Yoff  = (g_MaxTchY - g_MinTchY);
  else
    Yoff  = 0;


  *TchX = 127 - (Xoff * RelX)/128;
  *TchY = 63 -  (Yoff * RelY)/128;

  return;
};


byte ChkRgn(char *PantallaActiva,unsigned char TchX,unsigned char TchY,unsigned char RgnX1,unsigned char RgnY1,unsigned char RgnX2,unsigned char RgnY2,unsigned char TchRgn){
	
  if(TchX >= RgnX1 && TchX <= RgnX2 && TchY >= RgnY1 && TchY <= RgnY2){
  
    if(TchRgn != l_TchRgnAnt){
      if(l_TchRgnAnt != 0 && l_TchRgnAnt < 200){  // Si la region anterior era activa la vuelve a color normal salvo que la region sea menor a 200 
        RePaintRgn(PantallaActiva,l_RgnAnX1,l_RgnAnY1,l_RgnAnX2,l_RgnAnY2,0);
      }
      if(TchRgn != 0 && TchRgn < 200){  // Region activa, marca el boton
        RePaintRgn(PantallaActiva,RgnX1,RgnY1,RgnX2,RgnY2,1);
        l_RgnAnX1    = RgnX1;
        l_RgnAnY1    = RgnY1;
        l_RgnAnX2    = RgnX2;
        l_RgnAnY2    = RgnY2;
      }
      l_TchRgnAnt = TchRgn;
    }
    return RGN_OK;
  }
  return NOT_RGN;
};




void RgnOff(char *PantallaActiva){
  if(l_TchRgnAnt != 0 && l_TchRgnAnt < 200){  // Si la region anterior era activa la vuelve a color normal salvo que la region sea menor a 200 
    RePaintRgn((char *) PantallaActiva,l_RgnAnX1,l_RgnAnY1,l_RgnAnX2,l_RgnAnY2,0);
  }
  l_TchRgnAnt = 0;    // No esta tocando el touch
};

