
; Include derivative-specific definitions
       INCLUDE 'derivative.inc'
       
       XDEF DS_Fac_ReadTemp
       XDEF DS_Crp_ReadTemp
       XDEF DS_TempStatus
       XDEF DS_DallasData

       XDEF Rx1Wire
       XDEF Tx1Wire
       XDEF Temp_msb
       XDEF Temp_lsb
       XDEF Temp_msbBf
       XDEF Temp_lsbBf
       XDEF TrgH
       XDEF TrgL
       XDEF CONFIG_1820
       XDEF Reserved
       XDEF CountRemain
       XDEF CountxC
       XDEF CRC_SPad

MY_ZEROPAGE: SECTION  SHORT
;
; variable/data section
;
DS_TempStatus:	DC.B	$00

Rx1Wire:		    DC.B	$00
Tx1Wire:		    DC.B	$00

DS_DallasData:
Temp_lsbBf:	    DC.B	$DA
Temp_msbBf:	    DC.B	$01
TrgH:   		    DC.B	$4B
TrgL:		        DC.B	$46
CONFIG_1820:    DC.B	$7F
Reserved:       DC.B	$FF
CountRemain:    DC.B	$0C
CountxC: 		    DC.B	$10
CRC_SPad:		    DC.B	$F1
ROM_data:       DC.B $00,$00,$00,$00,$00,$00,$00,$00

Temp_msb:		    DC.B	$00
Temp_lsb: 	    DC.B	$00


MyCode:     SECTION


DS_Fac_CheckTemp1:                      ; Estaba el sensor y estado = void, disparo lectura
        jsr     DS_FAC_RESET
        lda     #$CC                    ; Skip ROM
        jsr     OUT_FAC_1_wire          ; Send it
        lda     #$44                    ; ConvertTemp
        jsr     OUT_FAC_1_wire          ; Send it
        lda     #$01                    ; pongo estado de inicio de espera de conversion
        sta     DS_TempStatus           ; TMP_START_CONV  ver como poner el define
        ldhx    Temp_msb
        rts
 
; Parasite power !!!               
DS_Fac_ReadTemp:
        lda     DS_TempStatus           ; ver como poner el define
        beq     DS_Fac_VoidStat         ; Si el estado es Void disparo lectura
        cmp     #$03                    ; TMP_READY_CONV 
        beq     DS_Fac_Ready            ; Si termino la conversion voy a leer y devolver valor
        ldhx    Temp_msb                ; Pongo el anterior valor
        rts

DS_Fac_VoidStat:                        ; hago reset y chequeo si esta el sensor 
        jsr     DS_FAC_RESET
        bcs     DS_Fac_CheckTemp1       ; esta el sensor disparo la lectura
        lda     #$00                    ; No esta el sensor, ponfo estado void e indico error
        sta     DS_TempStatus           ; TMP_VOID ver como poner el define
        ldhx    #$FFFF
        rts

DS_Fac_Ready:                           ; Termino la espera de conversion
        jsr     DS_FAC_RESET            ; hago reset y chequeo si esta el sensor
        bcs     DS_Fac_ReadTemp1        ;    
        lda     #$00                    ; No esta el sensor, ponfo estado void e indico error
        sta     DS_TempStatus           ; 
        ldhx    #$FFFF                  ; no sensor ret -1
        rts

DS_Fac_ReadTemp1:                       ; esta el sensor y termino el tiempo de conversion, leo data
        lda     #$CC                    ; Skip ROM
        jsr     OUT_FAC_1_wire          ; Send it

        lda     #$BE                    ; Read scratchpad command
        jsr     OUT_FAC_1_wire          ; Send it
        
        jsr     IN_FAC_1_wire           ; 0 Get first temperature byte
        lda     Rx1Wire
        sta     Temp_lsb                ; save it
        sta     Temp_lsbBf              ; save it
        
        jsr     IN_FAC_1_wire           ; 1 Get second temperature byte
        lda     Rx1Wire
        sta     Temp_msb                ; save it
        sta     Temp_msbBf              ; save it
        
        jsr     IN_FAC_1_wire               ; 2 Get trigger high
        lda     Rx1Wire
        sta     TrgH                      ;save it
        
        jsr     IN_FAC_1_wire               ; 3 Get trigger low
        lda     Rx1Wire
        sta     TrgL                      ;save it
        
        jsr     IN_FAC_1_wire               ; 4 Get the DS18b20 config
        lda     Rx1Wire
        sta     CONFIG_1820
        
        jsr     IN_FAC_1_wire               ; 5 reserved
        lda     Rx1Wire
        sta     Reserved
        
        jsr     IN_FAC_1_wire               ; 6 Get CountRemain
        lda     Rx1Wire
        sta     CountRemain

        jsr     IN_FAC_1_wire               ; 7 Get  CountxC
        lda     Rx1Wire
        sta     CountxC
        
        jsr     IN_FAC_1_wire               ; 8 Get the CRC for the scratchpad
        lda     Rx1Wire
        sta     CRC_SPad
        
        lda     Temp_lsb                ; 
        ldx     #4

DL_Fac_ShTemp:
        lsla    
        rol     Temp_msb                   ;shift integer into msb
        decx
        bne     DL_Fac_ShTemp
 
        
        lda     Temp_lsb                ; Guardo en el acumulador el valor bajo
        clr     Temp_lsb           ; no estaba en 0 lo borro para ir acumulando, no lo hice arriba x q jodia con el branch
        and     #$0F
        beq     DL_Fac_EndSetDec       ; 0 -> 0

        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ; 1 -> 1
        deca
        beq     DL_Fac_EndSetDec       ; 2 -> 1

        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ; 3 -> 2
        deca
        beq     DL_Fac_EndSetDec       ; 4 -> 2

        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ; 5 -> 3
        
        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ; 6 -> 4
        deca
        beq     DL_Fac_EndSetDec       ; 7 -> 4

        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ; 8 -> 5
        
        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ; 9 -> 6
        deca
        beq     DL_Fac_EndSetDec       ;10 -> 6

        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ;11 -> 7
        deca
        beq     DL_Fac_EndSetDec       ;12 -> 7

        inc     Temp_lsb
        deca
        beq     DL_Fac_EndSetDec       ;13 -> 8

        inc     Temp_lsb           ;+13 -> 9
        
DL_Fac_EndSetDec:
        jsr     DS_FAC_RESET
        lda     #$CC                    ; Skip ROM
        jsr     OUT_FAC_1_wire          ; Send it
        lda     #$44                    ; ConvertTemp
        jsr     OUT_FAC_1_wire          ; Send it
        lda     #$01                    ; pongo estado de inicio de espera de conversion
        sta     DS_TempStatus           ; TMP_START_CONV  ver como poner el define
        ldhx    Temp_msb
        rts


;//////////////////////////////////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////////////////////////////////
DS_Crp_CheckTemp1:                      ; Estaba el sensor y estado = void, disparo lectura
        jsr     DS_CRP_RESET
        lda     #$CC                    ; Skip ROM
        jsr     OUT_CRP_1_wire          ; Send it
        lda     #$44                    ; ConvertTemp
        jsr     OUT_CRP_1_wire          ; Send it
        lda     #$01                    ; pongo estado de inicio de espera de conversion
        sta     DS_TempStatus           ; TMP_START_CONV  ver como poner el define
        ldhx    Temp_msb
        rts

DS_Crp_ReadTemp:
        lda     DS_TempStatus           ; ver como poner el define
        beq     DS_Crp_VoidStat         ; Si el estado es Void disparo lectura
        cmp     #$03                    ; TMP_READY_CONV 
        beq     DS_Crp_Ready            ; Si termino la conversion voy a leer y devolver valor
        ldhx    Temp_msb                ; Pongo el anterior valor
        rts

DS_Crp_VoidStat:
        jsr     DS_CRP_RESET
        bcs     DS_Crp_CheckTemp1       ; esta el sensor disparo la lectura
        lda     #$00                    ; No esta el sensor, ponfo estado void e indico error
        sta     DS_TempStatus           ; TMP_VOID ver como poner el define
        ldhx    #$FFFF
        rts

DS_Crp_Ready:
        jsr     DS_CRP_RESET            ; hago reset y chequeo si esta el sensor
        bcs     DS_Crp_ReadTemp1        ;    
        lda     #$00                    ; No esta el sensor, ponfo estado void e indico error
        sta     DS_TempStatus           ; 
        ldhx    #$FFFF                  ; no sensor ret -1
        rts

DS_Crp_ReadTemp1:
        lda     #$CC                    ;Skip ROM
        jsr     OUT_CRP_1_wire              ;Send it

        lda     #$BE                    ;Read scratchpad command
        jsr     OUT_CRP_1_wire              ;Send it
        
        jsr     IN_CRP_1_wire               ; 0 Get first temperature byte
        lda     Rx1Wire
        sta     Temp_lsb                   ;save it
        sta     Temp_lsbBf                 ;save it
        
        jsr     IN_CRP_1_wire               ; 1 Get second temperature byte
        lda     Rx1Wire
        sta     Temp_msb                   ;save it
        sta     Temp_msbBf                 ;save it
        
        jsr     IN_CRP_1_wire               ; 2 Get trigger high
        lda     Rx1Wire
        sta     TrgH                      ;save it
        
        jsr     IN_CRP_1_wire               ; 3 Get trigger low
        lda     Rx1Wire
        sta     TrgL                      ;save it
        
        jsr     IN_CRP_1_wire               ; 4 Get the DS18b20 config
        lda     Rx1Wire
        sta     CONFIG_1820
        
        jsr     IN_CRP_1_wire               ; 5 reserved
        lda     Rx1Wire
        sta     Reserved
        
        jsr     IN_CRP_1_wire               ; 6 Get CountRemain
        lda     Rx1Wire
        sta     CountRemain

        jsr     IN_CRP_1_wire               ; 7 Get  CountxC
        lda     Rx1Wire
        sta     CountxC
        
        jsr     IN_CRP_1_wire               ; 8 Get the CRC for the scratchpad
        lda     Rx1Wire
        sta     CRC_SPad
       
        lda     Temp_lsb                ; 
        ldx     #4
        
        
        
DL_Crp_ShTemp:
        lsla    
        rol     Temp_msb                   ;shift integer into msb
        decx
        bne     DL_Crp_ShTemp
 
        
        lda     Temp_lsb                ; Guardo en el acumulador el valor bajo
        clr     Temp_lsb           ; no estaba en 0 lo borro para ir acumulando, no lo hice arriba x q jodia con el branch
        and     #$0F
        beq     DL_Crp_EndSetDec       ; 0 -> 0

        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ; 1 -> 1
        deca
        beq     DL_Crp_EndSetDec       ; 2 -> 1

        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ; 3 -> 2
        deca
        beq     DL_Crp_EndSetDec       ; 4 -> 2

        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ; 5 -> 3
        
        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ; 6 -> 4
        deca
        beq     DL_Crp_EndSetDec       ; 7 -> 4

        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ; 8 -> 5
        
        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ; 9 -> 6
        deca
        beq     DL_Crp_EndSetDec       ;10 -> 6

        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ;11 -> 7
        deca
        beq     DL_Crp_EndSetDec       ;12 -> 7

        inc     Temp_lsb
        deca
        beq     DL_Crp_EndSetDec       ;13 -> 8

        inc     Temp_lsb           ;+13 -> 9
        
DL_Crp_EndSetDec:        
        jsr     DS_CRP_RESET
        lda     #$CC                    ; Skip ROM
        jsr     OUT_CRP_1_wire          ; Send it
        lda     #$44                    ; ConvertTemp
        jsr     OUT_CRP_1_wire          ; Send it
        lda     #$01                    ; pongo estado de inicio de espera de conversion
        sta     DS_TempStatus           ; TMP_START_CONV  ver como poner el define
        ldhx    Temp_msb
        rts





  
   




************************************
*  Reset and detect Presence pulse *
************************************
DS_FAC_RESET:
        sei 
        bclr    7,PTED                  ;be sure bit is set to low
        bset    7,PTEDD                 ;Start of the pulse
        jsr     wait480usec             ;Wait a while
        bclr    7,PTEDD                  ;End the pulse
        jsr     wait60usec              ;Wait for resistor pull up
        brset   7,PTED,DS_FAC_RESET_NS
        jsr     wait480usec
        cli
        sec
        bset    7,PTED                  ;be sure bit is set to 1
        bset    7,PTEDD                 ;Set to charge sensor 
        rts
DS_FAC_RESET_NS
        jsr     wait480usec
        cli
        clc
        bset    7,PTED                  ;be sure bit is set to 1
        bset    7,PTEDD                 ;Set to charge sensor 
        rts



DS_CRP_RESET:
        sei 
        bclr    6,PTED                  ;be sure bit is set to low
        bset    6,PTEDD                  ;End the pulse
        jsr     wait480usec             ;Wait a while
        bclr    6,PTEDD                  ;End the pulse
        jsr     wait60usec              ;Wait for resistor pull up
        brset   6,PTED,DS_CRP_RESET_NS
        jsr     wait480usec
        cli
        sec
        bset    6,PTED                  ;be sure bit is set to 1
        bset    6,PTEDD                 ;Set to charge sensor 
        rts
DS_CRP_RESET_NS
        jsr     wait480usec
        cli
        clc
        bset    6,PTED                  ;be sure bit is set to 1
        bset    6,PTEDD                 ;Set to charge sensor 
        rts
        

*************************
* MASTER READ SLOT      *
*************************
IN_CRP_1_wire:
        lda     #8                      ;bit count
IN_CRP_1_wire1:
        psha   
        bsr     InBit_CRP_1_wire
        lda     Rx1Wire             ;get current data byte
        rora                            ;shift carry into it
        sta     Rx1Wire             ;save it
        pula                    
        deca
        bne     IN_CRP_1_wire1
        rts
        
InBit_CRP_1_wire:
        sei
        bclr    6,PTED
        bset    6,PTEDD                 ;Start the pulse - drive low
        brn     *
        bclr    6,PTEDD                 ;End the pulse - pull to high
        jsr     wait15usec
        brset   6,PTED,*+3              ;set carry 
        jsr     wait15usec
        jsr     wait15usec
        cli
        bset    6,PTED                  ;be sure bit is set to 1
        bset    6,PTEDD                 ;Set to charge sensor 
        rts



IN_FAC_1_wire:
        lda     #8                      ;bit count
IN_FAC_1_wire1:
        psha   
        bsr     InBit_FAC_1_wire
        lda     Rx1Wire             ;get current data byte
        rora                            ;shift carry into it
        sta     Rx1Wire             ;save it
        pula                    
        deca
        bne     IN_FAC_1_wire1
        rts
        
InBit_FAC_1_wire:
        sei
        bclr    7,PTED
        bset    7,PTEDD                 ;Start the pulse - drive low
        brn     *
        bclr    7,PTEDD                 ;End the pulse - pull to high
        jsr     wait15usec
        brset   7,PTED,*+3              ;set carry 
        jsr     wait15usec
        jsr     wait15usec
        cli
        bset    7,PTED                  ;be sure bit is set to 1
        bset    7,PTEDD                 ;Set to charge sensor 
        rts


******************************************
*  Transmit a byte via one-wire system
*  Enter with a byte in Reg A
******************************************
OUT_CRP_1_wire:
        sta     Tx1Wire
        lda     #8                      ;bit count
OUT_CRP_1_wire1:
        psha
        lda     Tx1Wire             ;Get the data byte
        lsra                            ;Shift low bit into carry
        sta     Tx1Wire             ;save
        bcs     OUT_CRP_1_wire2             ;branch if sending a '1'
*************************
* MASTER WRITE "0" SLOT *
*************************
        sei
        bclr    6,PTED                  ;be sure bit is set to low
        bset    6,PTEDD                 ;Start the pulse
        jsr     wait60usec              ;data line held low for 60 usec
        bclr    6,PTEDD                 ;End the pulse
        bra     OUT_CRP_1_wire3
*************************        
* MASTER WRITE "1" SLOT *
*************************
OUT_CRP_1_wire2:
        sei
        bclr    6,PTED                  ;be sure bit is set to low
        bset    6,PTEDD                 ;Start the pulse
        brn     *     
        bclr    6,PTEDD                 ;End the pulse
        jsr     wait60usec              ;Wait for resistor pull up
**************************
* FINISH MASTER R/W SLOT * 
**************************
OUT_CRP_1_wire3:
        cli
        bset    6,PTED                  ;be sure bit is set to 1
        bset    6,PTEDD                 ;Set to charge sensor 
        pula
        deca                            ;decrement the bit count
        bne     OUT_CRP_1_wire1             ;loop if not finished
        rts




OUT_FAC_1_wire:
        sta     Tx1Wire
        lda     #8                      ;bit count
OUT_FAC_1_wire1:
        psha
        lda     Tx1Wire             ;Get the data byte
        lsra                            ;Shift low bit into carry
        sta     Tx1Wire             ;save
        bcs     OUT_FAC_1_wire2             ;branch if sending a '1'
*************************
* MASTER WRITE "0" SLOT *
*************************
        sei
        bclr    7,PTED                  ;be sure bit is set to low
        bset    7,PTEDD                 ;Start the pulse
        jsr     wait60usec              ;data line held low for 60 usec
        bclr    7,PTEDD                 ;End the pulse
        bra     OUT_FAC_1_wire3
*************************        
* MASTER WRITE "1" SLOT *
*************************
OUT_FAC_1_wire2:
        sei
        bclr    7,PTED                  ;be sure bit is set to low
        bset    7,PTEDD                 ;Start the pulse
        brn     *     
        bclr    7,PTEDD                 ;End the pulse
        jsr     wait60usec              ;Wait for resistor pull up
**************************
* FINISH MASTER R/W SLOT * 
**************************
OUT_FAC_1_wire3:
        cli
        bset    7,PTED                  ;be sure bit is set to 1
        bset    7,PTEDD                 ;Set to charge sensor 
        pula
        deca                            ;decrement the bit count
        bne     OUT_FAC_1_wire1             ;loop if not finished
        rts




**********************************************************************
;MicroSec01:      fdb     {8}
;MicroSec07:      fdb     {56}
;MicroSec60:      fdb     {480}
*
*       Delay routine   ;The carry bit is not affected by this routine
DELAY   decx                    ; ( 1 cycle )
        nop                     ; ( 1 cycle )   
        nop                     ; ( 1 cycle )
        nop                     ; ( 1 cycle )
        nop                     ; ( 1 cycle )
        nop                     ; ( 1 cycle )
        nop                     ; ( 1 cycle )     
        bne      DELAY          ; ( 3 cycles ) - 10 cycles in loop
        rts                     ; ( 4 cycles )
;
wait60usec:                     ;Wait for 60 micro seconds (24+(Xreg*10))*cycle time
        tpa                     ; ( 1 cycles )
        psha                    ; ( 2 cycles )
        pshx                    ; ( 2 cycles )
        ldx     #38             ; #46             ; ( 2 cycles )
        bsr     DELAY           ; ( 4 cycles )
        pulx                    ; ( 2 cycles )
        pula                    ; ( 2 cycles )
        tap                     ; ( 1 cycles )
        rts                     ; ( 4 cycles )
;


wait15usec:                     ;Wait for 15 micro seconds (24+(Xreg*10))*cycle time
        tpa                     ; ( 1 cycles )
        psha                    ; ( 2 cycles )
        pshx                    ; ( 2 cycles )
        ldx     #10            ; ( 2 cycles )
        bsr     DELAY           ; ( 4 cycles )
        pulx                    ; ( 2 cycles )
        pula                    ; ( 2 cycles )
        tap                     ; ( 1 cycles )
        rts                     ; ( 4 cycles )
;



wait480usec:                    ;Wait for 480 micro seconds
        bsr     wait60usec
        bsr     wait60usec
        bsr     wait60usec
        bsr     wait60usec
        bsr     wait60usec
        bsr     wait60usec
        bsr     wait60usec
        bsr     wait60usec    
        rts                     ; ( 4 cycles )
;

 END