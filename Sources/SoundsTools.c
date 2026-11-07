#include "derivative.h"
#include "SoundsTools.h"
#include "SoundData.h"
#include "Ports.h"
#include "Lumiere.h"


byte  l_Volumen;
byte *l_SoundDataPt = 0;


byte GetVolumen(void);
void SetVolumen(byte Volumen);
byte VolumenUp(void);
byte VolumenDw(void);

void SoundInt(void);
void SoundCycle(byte *SoundDataPt);

///////////////////////////////////////////////////////////////////////////////////////
byte GetVolumen(void){
  return l_Volumen;
};

void SetVolumen(byte Volumen){
  l_Volumen = Volumen;
};

byte VolumenUp(void){
  if(l_Volumen < 10){
    l_Volumen++;
    return 1;
  }
  return 0;
};

byte VolumenDw(void){
  if(l_Volumen > 0){
    l_Volumen--;
    return 1;
  }
  return 0;
};

/////////////////////// SOUND ////////////////////////////////////////////////////////

void SoundMenuAd(void){
//  l_SoundDataPt = &AvMenu_Snd;
  SoundCycle(&AvMenu_Snd); 
}
void SoundMenuBk(void){
//  l_SoundDataPt = &RetMenu_Snd;
  SoundCycle(&RetMenu_Snd); 
}
void SoundUp(void){
//  l_SoundDataPt = &Subir_Snd;
  SoundCycle(&Subir_Snd); 
}
void SoundDw(void){
//  l_SoundDataPt = &Bajar_Snd;
  SoundCycle(&Bajar_Snd); 
}
void SoundSelOk(void){
//  l_SoundDataPt = &SelOk_Snd;
  SoundCycle(&SelOk_Snd); 
}
void SoundSelNok(void){
//  l_SoundDataPt = &SelNok_Snd;
  SoundCycle(&SelNok_Snd); 
}
void SoundStart(void){
//  l_SoundDataPt = &Start_Snd;
  SoundCycle(&Start_Snd); 
}
void SoundStop(void){
//  l_SoundDataPt = &Stop_Snd;
  SoundCycle(&Stop_Snd); 
}
void SoundPause(void){
//  l_SoundDataPt = &Pause_Snd;
  SoundCycle(&Pause_Snd); 
}

void SoundWlcm(void){
//  l_SoundDataPt = &Welcome_Snd;
  SoundCycle(&Welcome_Snd); 
};

void SoundWrite(void){
//  l_SoundDataPt = &SveMemo_Snd;
  SoundCycle(&SveMemo_Snd); 
};

void SoundDelete(void){
//  l_SoundDataPt = &DelMemo_Snd;
  SoundCycle(&DelMemo_Snd); 
};

void SoundMinTop(void){
//  l_SoundDataPt = &Minute_Snd;
  SoundCycle(&Minute_Snd); 
};

void SoundTempStop(void){
//  l_SoundDataPt = &TempStop_Snd;
  SoundCycle(&TempStop_Snd); 
};



void SoundCycle(byte *SoundDataPt){
  unsigned int  SoundLenMs    = 0;
  byte          SoundVol      = 0;
  unsigned int  Volumen       = 10;
  unsigned int  SoundFDivCnt  = 0;  
  unsigned int  SoundP1       = 0;
  unsigned int  SoundP2       = 0;
  byte          FullWave      = 0;
  unsigned int  SoundFDiv     = 0;
  
  
  while(SoundDataPt[0] != 0xFF){
    
    SoundFDiv   = (15625 / SoundDataPt[0])/ 10;            // carga el primer tono
    SoundLenMs    = (int) SoundDataPt[1] * 1562; // carga la duracion
    SoundVol      = SoundDataPt[2];                  // carga volumen del tono
    Volumen       = (l_Volumen * SoundVol)/10;
    
    if(Volumen > 5){
      FullWave = 1;
    }
    else{
      FullWave = 0;
    }
    SoundP1 =            (SoundFDiv *  Volumen )  / 20; 
    SoundP2 = SoundP1 + ((SoundFDiv *  (10 - Volumen)) / 10) ; 

    SoundDataPt++;
    SoundDataPt++;
    SoundDataPt++;
    g_SoundInt = 0;

    while(SoundLenMs != 0){                   // termino con la longitud?
      while(g_SoundInt == 0);
      g_SoundInt = 0;
      SoundLenMs--;
      if(SoundFDivCnt < SoundP1){
        SOUND_1      = 0;
        SOUND_2      = 1;
        SoundFDivCnt++;
      }
      else if(SoundFDivCnt < SoundP2){
        SOUND_1      = 0;
        SOUND_2      = 0;
        SoundFDivCnt++;
      }
      else{
        if(FullWave == 0){
          SOUND_1    = 0;
          SOUND_2    = 1;
        }
        else{
          SOUND_1    = 1;
          SOUND_2    = 0;
        }
        SoundFDivCnt++;
      }

      if(SoundFDivCnt == SoundFDiv){
        SoundFDivCnt = 0;
      }  
    } 
  }
  SOUND_1       = 0;
  SOUND_2       = 0;
  return;
}



/*
void Sound(unsigned int Frec,unsigned int LenMs,byte Vol){
  unsigned int aux;
  unsigned int aux1;
  unsigned int Time;
  unsigned int LoopsxCiclo;  // 100 es  1968hz => 196800/frec      (9840 x q hago el doble de ciclos de espera)
  unsigned int SoundP1;
  unsigned int SoundP2;
  
  Time   = (LenMs * (Frec / 20))/50;
  LoopsxCiclo = 280000/Frec;

  SoundP1 =            (LoopsxCiclo * Vol        ) / 20; 
  SoundP2 = SoundP1 +  (LoopsxCiclo * (10 - Vol) ) / 10; 


  for(aux1=0;aux1<Time;aux1++){ 
    SOUND_1 = 0;
    SOUND_2 = 1;
    for(aux=0;aux<SoundP1    ;aux++){
    }
    SOUND_1 = 0;
    SOUND_2 = 0;
    for(     ;aux<SoundP2    ;aux++){
    }
    SOUND_1 = 1;
    SOUND_2 = 0;
    for(     ;aux<LoopsxCiclo;aux++){
    }

  }
  SOUND_1 = 0;
  SOUND_2 = 0;
 
}
*/

/*
void SoundCycle(byte *SoundDataPt){
  unsigned int  SoundLenMs    = 0;
  byte          SoundVol      = 0;
  unsigned int  Volumen       = 10;
  unsigned int  SoundFDivCnt  = 0;  
  unsigned int  SoundP1       = 0;
  unsigned int  SoundP2       = 0;
  byte          FullWave      = 0;
  unsigned int  SoundFDiv     = 0;
  
  
  while(SoundDataPt[0] != 0xFF){
    
//    SoundFDiv   = (15625 / SoundDataPt[0])/ 10;            // carga el primer tono
    SoundFDiv   = (1000 / SoundDataPt[0])/ 10;            // carga el primer tono
//    SoundLenMs    = (int) SoundDataPt[1] * 1562; // carga la duracion
    SoundLenMs    = (int) SoundDataPt[1] * 100; // carga la duracion
    SoundVol      = SoundDataPt[2];                  // carga volumen del tono
    Volumen       = (l_Volumen * SoundVol)/10;
    
    if(Volumen > 5){
      FullWave = 1;
    }
    else{
      FullWave = 0;
    }
    SoundP1 =            (SoundFDiv *  Volumen )  / 20; 
    SoundP2 = SoundP1 + ((SoundFDiv *  (10 - Volumen)) / 10) ; 

    SoundDataPt++;
    SoundDataPt++;
    SoundDataPt++;
    g_SoundInt = 0;

    while(SoundLenMs != 0){                   // termino con la longitud?
      while(g_SoundInt == 0);
      g_SoundInt = 0;
      SoundLenMs--;
      if(SoundFDivCnt < SoundP1){
        SOUND_1      = 0;
        SOUND_2      = 1;
        SoundFDivCnt++;
      }
      else if(SoundFDivCnt < SoundP2){
        SOUND_1      = 0;
        SOUND_2      = 0;
        SoundFDivCnt++;
      }
      else{
        if(FullWave == 0){
          SOUND_1    = 0;
          SOUND_2    = 1;
        }
        else{
          SOUND_1    = 1;
          SOUND_2    = 0;
        }
        SoundFDivCnt++;
      }

      if(SoundFDivCnt == SoundFDiv){
        SoundFDivCnt = 0;
      }  
    } 
  }
  SOUND_1       = 0;
  SOUND_2       = 0;
  return;
}

*/


