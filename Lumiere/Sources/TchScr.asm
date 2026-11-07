
; Include derivative-specific definitions
       INCLUDE 'derivative.inc'

       XDEF TS_GetTouch
       
       XDEF AD_Used

       
;       XDEF TS_X_Pos
;       XDEF TS_Y_Pos
       
            
; variable/data section
MY_ZEROPAGE: SECTION  SHORT
TS_Status:	       DC.B	$00 ; 0 -> Init  / 1 -> check ADx / 2 -> Check ADy    
TS_X_Pos:		   DC.B	$00 
TS_Y_Pos:		   DC.B	$00 
AD_Used:           DC.B	$00


MyCode:     SECTION



 
TS_GetTouch:
  LDA     TS_Status              ;Chequea el estado de la funcion
  BNE     TS_Chk_S1
  bclr    5,APCTL1                ; AD5 off
  bclr    6,APCTL1                ; AD6 off

  bset    5,PTBDD                 ; Data direction out
;  bset    5,PTBPE                 ; Pull up on
  bclr    5,PTBPE                 ; Pull up off
  bclr    5,PTBSE                 ; Slew Rate off
  bclr    5,PTBDS
  bclr    5,PTBD                  ; Data 0V

  bclr    7,PTBDD                 ; Data direction in
;  bset    7,PTBPE                 ; Pull up on
  bclr    7,PTBPE                 ; Pull up off
  bclr    7,PTBSE                 ; Slew Rate off
  bclr    7,PTBDS

  bclr    4,PTBDD                 ; Data direction in
  bclr    4,PTBPE                 ; Pull up off
  bclr    4,PTBSE                 ; Slew Rate off
  bclr    4,PTBDS


  bclr    6,PTBDD                 ; Data direction in
  bset    6,PTBPE                 ; Pull up on
  bclr    6,PTBSE                 ; Slew Rate off
  bclr    6,PTBDS

  brclr   6,PTBD,TS_Tch_On 
  LDHX    #$FFFE
  RTS
 
TS_Tch_On:
; Dispara lectura de eje x
  lda     AD_Used
  beq     TS_Tch_On1
  LDHX    #$FFFF
  RTS

TS_Tch_On1:
  lda      #$01                  ; Aviso que uso el AD
  sta      AD_Used
  lda      #$71
  sta      ADCCFG
  
  bclr    6,APCTL1                ; AD6 off     

  bset    6,PTBDD                 ; Data direction out
  bclr    6,PTBPE                 ; Pull up off
  bclr    6,PTBSE                 ; Slew Rate off
  bclr    6,PTBDS
  bset    6,PTBD                  ; Data 5V
 
  bset    4,PTBDD                 ; Data direction out
  bclr    4,PTBPE                 ; Pull up off
  bclr    4,PTBSE                 ; Slew Rate off
  bclr    4,PTBDS
  bclr    4,PTBD                  ; Data 0V
  
  bclr    7,PTBDD                 ; Data direction in
  bclr    7,PTBPE                 ; Pull up off
  bclr    7,PTBSE                 ; Slew Rate off
  bclr    7,PTBDS
   
  bclr    5,PTBDD                 ; Data direction in
  bclr    5,PTBPE                 ; Pull up off
  bclr    5,PTBSE                 ; Slew Rate off
  bclr    5,PTBDS
  bset    5,APCTL1                ; AD5 on

  lda     #$05
  sta     ADCSC1                  ; Go on channel 5
 
  LDA     #$01
  STA     TS_Status
  LDHX    #$FFFF
  RTS

TS_Chk_S1:
  DECA
  BNE     TS_Chk_S2

  LDA     ADCSC1                 ; Estado 01 chequea si la lectura del AD termino, si no termino  devuelve $FFFF
  AND     #$80
  BNE     TS_Sv_X
  
  LDA     ADCSC2                 ; Estado 01 chequea si hay una lectura en progreso  devuelve $FFFF
  AND     #$80
  BNE     TS_InPrg_X

  LDA     #$00
  STA     TS_Status
  STA     AD_Used
TS_InPrg_X:
  LDHX    #$FFFF
  RTS


TS_Sv_X:     
; Salva el valor x
  LDA     ADCRL
  STA     TS_X_Pos                ; Almacena el valor en memoria
;   Estado 00 dispara una lectura del AD eje y y pasa a estado 02 devuelve $FFFF 
  bclr    5,APCTL1                ; AD5 off     
 
  bset    5,PTBDD                 ; Data direction out
  bclr    5,PTBPE                 ; Pull up off
  bclr    5,PTBSE                 ; Slew Rate off
  bclr    5,PTBDS
  bset    5,PTBD                  ; Data 5V

  bset    7,PTBDD                 ; Data direction out
  bclr    7,PTBPE                 ; Pull up off
  bclr    7,PTBSE                 ; Slew Rate off
  bclr    7,PTBDS
  bclr    7,PTBD                  ; Data 0V
  
  bclr    4,PTBDD                 ; Data direction in
  bclr    4,PTBPE                 ; Pull up off
  bclr    4,PTBSE                 ; Slew Rate off
  bclr    4,PTBDS
    
  bclr    6,PTBDD                 ; Data direction in
  bclr    6,PTBPE                 ; Pull up off
  bclr    6,PTBSE                 ; Slew Rate off
  bclr    6,PTBDS
  bset    6,APCTL1                ; AD6 on

  lda     #$06
  sta     ADCSC1                  ; Go on channel 6
  
  LDA     #$02
  STA     TS_Status              ; pasa a estado 02 
  LDHX    #$FFFF                 ; devuelve $FFFF 
  RTS

TS_Chk_S2
  LDA     ADCSC1
  AND     #$80
  BNE     TS_Sv_Y 
 
  LDA     ADCSC2                 ; Estado 01 chequea si hay una lectura en progreso  devuelve $FFFF
  AND     #$80
  BNE     TS_InPrg_Y

  LDA     #$00
  STA     TS_Status
  STA     AD_Used
TS_InPrg_Y:
  LDHX    #$FFFF
  RTS
  

TS_Sv_Y: 
  LDA  ADCRL
  STA  TS_Y_Pos
  LDA  #$00
  STA  TS_Status
  STA  AD_Used
  LDHX TS_X_Pos
  RTS


;////////////////////////////////////////////////////////////////