#ifndef _PORTS_H
#define _PORTS_H

#ifdef __cplusplus
  extern "C" { /* our assembly functions have C calling convention */
#endif


#define  SOUND_1        PTFD_PTFD4
#define  SOUND_2        PTFD_PTFD5
#define  COOLER_FAN     PTFD_PTFD2
                                  // contraste  pata 15    PTED_PTED2  TPM1CH0


#define  BK_LIGHT       PTFD_PTFD0        ; 
#define  BK_LIGHT_PWM   TPM1C2V      //PWM  4  registro del pwm del backlight
#define  CONTR_PWM      TPM1C3V

//   Leds RGB
#define  LED_Red        TPM1C5V    // PWM   7
#define  LED_Green      TPM1C4V    // PWM   6
#define  LED_Blue       TPM1C3V    // PWM   5

//  Display
#define  DSP_RST        PTAD_PTAD0
#define  DSP_CS2        PTAD_PTAD1
#define  DSP_CS1        PTAD_PTAD2
#define  DSP_DI         PTAD_PTAD3
#define  DSP_RW         PTAD_PTAD4
#define  DSP_E          PTAD_PTAD5           

//  Touch Screen
#define  TS_PORT1       PTBD_PTBD4
#define  TS_PORT2       PTBD_PTBD5
#define  TS_PORT3       PTBD_PTBD6
#define  TS_PORT4       PTBD_PTBD7

//  Dallas
#define  DLS_CRP        PTED_PTED6
#define  DLS_FAC        PTED_PTED7


#ifdef __cplusplus
  }
#endif


#endif