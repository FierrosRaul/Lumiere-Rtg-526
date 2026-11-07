#ifndef _DALLAS_H
#define _DALLAS_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif

#define TMP_VOID         0
#define TMP_START_CONV   1
#define TMP_WAIT_CONV    2
#define TMP_READY_CONV   3


//int DS_Crp_CheckSensor(void);          // indica si esta presente el sensor      0= Ok             -1 = not present
//int DS_Crp_CheckTemp(void);            //  Pone al sensor a calcular temperatura 0= Ok             -1 = not present
int DS_Crp_ReadTemp(void);              //  Obtiene la temperatura                0 >= temp *10 C   -1 = not present

//int DS_Fac_CheckSensor(void);          // indica si esta presente el sensor      0= Ok             -1 = not present
//int DS_Fac_CheckTemp(void);            //  Pone al sensor a calcular temperatura 0= Ok             -1 = not present
int DS_Fac_ReadTemp(void);              //  Obtiene la temperatura                0 >= temp *10 C   -1 = not present

extern unsigned char DS_TempStatus;
extern byte          DS_DallasData;

extern byte          Rx1Wire;
extern byte          Tx1Wire;
extern byte          Temp_msb;
extern byte          Temp_lsb;
extern byte          Temp_msbBf;
extern byte          Temp_lsbBf;
extern byte          TrgH;
extern byte          TrgL;
extern byte          CONFIG_1820;
extern byte          Reserved;
extern byte          CountRemain;
extern byte          CountxC;
extern byte          CRC_SPad;

#ifdef __cplusplus
  }
#endif

#endif /* _DALLAS_H */
