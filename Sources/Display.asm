;*******************************************************************
;* This stationery serves as the framework for a user application. *
;* For a more comprehensive program that demonstrates the more     *
;* advanced functionality of this processor, please see the        *
;* demonstration applications, located in the examples             *
;* subdirectory of the "Freescale CodeWarrior for HC08" program    *
;* directory.                                                      *
;*******************************************************************




; export symbols
            XDEF ClrDsp
            XDEF Wait
            XDEF DspPantalla
            XDEF DspScreenInit
 
            
            XDEF LcdWrtPg
            XDEF LcdWriteData
            XDEF LcdWriteCmd
           

            
;Char            
            XDEF CharMap8x8
            XDEF CharMapNum8x4
            XDEF CharMapNum7x5
            XDEF CharMapNum9x5
            XDEF CharMapNum24x13
            XDEF CharMapNum30x17
            XDEF CharMapNum21x32



            ; Include derivative-specific definitions
            INCLUDE 'MC9S08JM60.inc'

; variable/data section
MY_ZEROPAGE: SECTION  SHORT

LCD_Set_Y_Addr:            equ   $40   
LCD_Set_X_Page:            equ   $B8   ; or (0 -  7)
LCD_Set_StartLine:         equ   $C0   ; or (0 - 63)

LCD_Dsp_On:                equ   $3F
LCD_Dsp_Off:               equ   $3E


Debounce_Value:            equ   $80   ; set del contador de pasadas para no leer teclas opr el ruido


;
; variable/data section
;
;            ORG    RAMStart         ; Insert your data definition here
Scr_Byte_CntrL:		DC.B	$C0
Scr_Byte_CntrH:		DC.B	$C0
LCD_Y_Pos:	    	DC.B	$00
LCD_X_Pos:	    	DC.B	$00          
LCD_PG_DATA:	    DC.B	$00           

LCD_X_Edg1:	    	DC.B	$00
LCD_Y_Edg1:	    	DC.B	$00
LCD_X_Edg2:	    	DC.B	$00
LCD_Y_Edg2:	    	DC.B	$00


WaitCntrL:	    	DC.B	$00
WaitCntrH:	    	DC.B	$00

KbdStatus:	    	DC.B	$00        ; estado del teclado
KbdStatusTemp:	 	DC.B	$00        ; estado temporal del teclado
KbdStatusChng:	 	DC.B	$00        ; estado de cambio del teclado
KeyDbcCntr:	    	DC.B	$00        ; Debounce counter
KbdKeyBuff:       DC.B	$00        ; Buffer de teclado

PwmPwValCntr:     DC.B	$00        ; Contador del PWM de baja frecuencia
PwmPwValSet:      DC.B	$00        ; Valor de ancho de pulso
PwmPwValSetIn:    DC.B	$00        ; Valor de ancho de pulso interno

TestValueData:    DC.B  $00        ; Valor para testear
CharRowCntr:      DC.B  $00        ; Contador de filas del generador de caracteres


; code section
MyCode:     SECTION
; this assembly routine is called the C/C++ application

Wait:
  		LDA #$FD                             ; 
      STA WaitCntrH                        ; 
      LDA #$FF                             ; 
     	STA WaitCntrL                        ; 
      JSR  Wait1ns
      RTS                     ; return to caller


;/////////////////////////////////////////////////////////////////////////

DspScreenInit:
; Inicializacion de señales del display
      BSET 0,PTAD        ; RST
      BCLR 5,PTAD        ; E            
      BCLR 2,PTAD        ; CS1
      BCLR 1,PTAD        ; CS2
      BCLR 3,PTAD        ; DI
      BCLR 4,PTAD        ; RW

      BSET 0,PTFD        ; BkLight

  		LDA #$FD                             ; 
      STA WaitCntrH                        ; 
      LDA #$FF                             ; 
     	STA WaitCntrL                        ; 
      JSR  Wait1ns


      JSR  LcdSelChp1     ; Sel Chip 1
      LDA  #LCD_Dsp_On    ; Display On!!
      JSR  LcdWriteCmd

      JSR  LcdSelChp2     ; Sel Chip 2
      LDA  #LCD_Dsp_On    ; Display On!!
      JSR  LcdWriteCmd

      JSR  LcdSelChp1     ; Sel Chip 1
      LDA  #LCD_Set_StartLine
      JSR  LcdWriteCmd

      JSR  LcdSelChp2     ; Sel Chip 2
      LDA  #LCD_Set_StartLine
      JSR  LcdWriteCmd

   		JSR  ClrDsp

  		LDA #$FD                             ; 
      STA WaitCntrH                        ; 
      LDA #$FF                             ; 
     	STA WaitCntrL                        ; 
      JSR  Wait1ns
      
      RTS



;/////////////////////////////////////////////////////////////////////////////////////////////////////////
;//////////////////////// DISPLAY ROUTINES ///////////////////////////////////////////////////////////////
;/////////////////////////////////////////////////////////////////////////////////////////////////////////
;     0 CSx +  Y address   +
; Y = 7  6  5  4  3  2  1  0  
;     0  0  0  0  0  + X addr +
; X = 7  6  5  4  3  2    1   0  
;
; HX indica el inicio del buffer de los datos de la pantalla
;



DspPantalla:
      JSR  LcdSelChp1          ; CS1
      LDA  #$00                 ; Pagina 0 
      STA  LCD_X_Pos
      LDA  #$00                           
      STA  LCD_Y_Pos

DspScr_Pos:
      LDA  LCD_X_Pos
      ORA  #LCD_Set_X_Page      ; Set X Page (0-7)

      JSR  LcdSelChp1          ; CS1
      JSR  LcdWriteCmd
      JSR  LcdSelChp2          ; CS2
      JSR  LcdWriteCmd
      
DspScr_PosY_Addr:     
      LDA  LCD_Y_Pos
      AND  #$3F                  ; 
      ORA  #LCD_Set_Y_Addr      ; Set Y address (0-63)
      JSR  LcdSelChp1          ; CS1
      JSR  LcdWriteCmd
      JSR  LcdSelChp2          ; CS2
      JSR  LcdWriteCmd
      
DspScrYLoop:            
      JSR  LcdSelChp1          ; CS1
      LDA  LCD_Y_Pos
      AND  #%01000000
      BEQ  DspScrWrite
      JSR  LcdSelChp2          ; CS2

DspScrWrite:
    	LDA  X
      JSR  LcdWriteData
			AIX  #$1

      LDA  LCD_Y_Pos
      INCA
      STA  LCD_Y_Pos
      
      LDA  LCD_Y_Pos
      AND  #$3F
      BNE  DspScrYLoop

      LDA  LCD_Y_Pos
      AND  #%01000000
      BEQ  DspScr_IncX
      
      JMP  DspScr_PosY_Addr

DspScr_IncX:                                                     
      LDA  #$00
      STA  LCD_Y_Pos

      LDA  LCD_X_Pos
      INCA
      STA  LCD_X_Pos
      AND  #%00001000
      BEQ  DspScr_Pos
  		RTS


DspGotoXY:
      LDA  LCD_X_Pos
      ORA  #LCD_Set_X_Page      ; Set X Page (0-7)

      JSR  LcdSelChp1          ; CS1
      JSR  LcdWriteCmd
      JSR  LcdSelChp2          ; CS2
      JSR  LcdWriteCmd

      JSR  LcdSelChp1          ; CS1
      LDA  LCD_Y_Pos
      AND  #%01000000
      BEQ  DspScrWrite
      JSR  LcdSelChp2          ; CS2

      RTS

DspChar:
      LDHX #CharMap8x8 
      LDA  #$00
      STA  CharRowCntr
 
DspChar_Loop:      
    	LDA  X
      JSR  LcdWriteData
			AIX  #$1
			
			LDA  CharRowCntr
      CMP  #$07
      BEQ  DspChar_EndRows 			
      INCA
      STA  CharRowCntr
			
  
      LDA  LCD_Y_Pos
      INCA
      STA  LCD_Y_Pos
      CMP  #$80
      BNE  DspChar_Loop 
      LDA  #$00
      STA  LCD_Y_Pos

      LDA  LCD_X_Pos
      INCA
      STA  LCD_X_Pos
      CMP  #$40      
      BNE  DspChar_Loop 
      LDA  #$00
      STA  LCD_Y_Pos
      JMP  DspChar_Loop

DspChar_EndRows:
      RTS


;////////////////////////////////////////////////////////////////////////////////////////




;/////////////////////////////////////////////////////////////////////////////////////////////////////////
;/////////////////////////////////////////////////////////////////////////////////////////////////////////
;/////////////////////////////////////////////////////////////////////////////////////////////////////////
;     0 CSx +  Y address   +
; Y = 7  6  5  4  3  2  1  0  
;     0  0  0  0  0  + X addr +
; X = 7  6  5  4  3  2    1   0  


ClrDsp:
      JSR  LcdSelChp1          ; CS1
      LDA  #$00                 ; Pagina 0 
      STA  LCD_X_Pos
      LDA  #$00                           
      LDA  LCD_Y_Pos

ClrDsp_Pos:
      LDA  LCD_X_Pos
      ORA  #LCD_Set_X_Page      ; Set X Page (0-7)

      JSR  LcdSelChp1          ; CS1
      JSR  LcdWriteCmd
      JSR  LcdSelChp2          ; CS2
      JSR  LcdWriteCmd
      
ClrDsp_PosY_Addr:     
      LDA  LCD_Y_Pos
      AND  #$3F                  ; 
      ORA  #LCD_Set_Y_Addr      ; Set Y address (0-63)
      JSR  LcdSelChp1          ; CS1
      JSR  LcdWriteCmd
      JSR  LcdSelChp2          ; CS2
      JSR  LcdWriteCmd
      
ClearDspYLoop:
      JSR  LcdSelChp1          ; CS1
      LDA  LCD_Y_Pos
      AND  #%01000000
      BEQ  ClearDspWrite
      JSR  LcdSelChp2          ; CS2

ClearDspWrite:            
      LDA  #$00
      JSR  LcdWriteData

      LDA  LCD_Y_Pos
      INCA
      STA  LCD_Y_Pos
      
      LDA  LCD_Y_Pos
      AND  #$3F
      BNE  ClearDspYLoop

      LDA  LCD_Y_Pos
      AND  #%01000000
      BEQ  ClrDsp_IncX
      
      JMP  ClrDsp_PosY_Addr

ClrDsp_IncX:                                                     
      LDA  #$00
      STA  LCD_Y_Pos

      LDA  LCD_X_Pos
      INCA
      STA  LCD_X_Pos
      AND  #%00001000
      BEQ  ClrDsp_Pos
  		RTS



;/////////////////////////////////////////////////////////////////////////////////////
;DspData:						      PTDD         		//out   42 43 48 49 50 51 52 53
;DspCtl_DI: 					equ   PTAD,PTAD_PTAD3	//out   31
;DspCtl_RW: 					equ   PTAD,PTAD_PTAD4	//out   32
;DspCtl_E:  					equ   PTAD,PTAD_PTAD5	//out   33
;DspCtl_CS1:					equ   PTAD,PTAD_PTAD2	//out   30
;DspCtl_CS2:					equ   PTAD,PTAD_PTAD1	//out   29
;DspCtl_Rst:					equ   PTAD,PTAD_PTAD0	//out   28
;/////////////////////////////////////////////////////////////////////////////////////

LcdSelChp1:
      BSET 2,PTAD        ; CS1
      BCLR 1,PTAD        ; CS2
			RTS

LcdSelChp2:
      BCLR 2,PTAD        ; CS1
      BSET 1,PTAD        ; CS2
			RTS

 

LcdWrtPg:             ;// Ver como recibir 3 parametros
;      STHX LCD_Y_Pos
      STA  LCD_PG_DATA 
      TXA
      STA  LCD_X_Pos
      LDA  3,SP
      STA  LCD_Y_Pos
      
      
LcdYPos:     
      LDA  LCD_Y_Pos
      AND  #$3F                  ; 
      ORA  #LCD_Set_Y_Addr      ; Set Y address (0-63)
      JSR  LcdSelChp1          ; CS1
      JSR  LcdWriteCmd
      JSR  LcdSelChp2          ; CS2
      JSR  LcdWriteCmd
    
      
LcdXPag:      
      LDA  LCD_X_Pos
      AND  #$07                  ; 
      ORA  #LCD_Set_X_Page      ; Set X Page (0-7)
      JSR  LcdSelChp1          ; CS1
      JSR  LcdWriteCmd
      JSR  LcdSelChp2          ; CS2
      JSR  LcdWriteCmd

      
      JSR  LcdSelChp1          ; CS1
      LDA  LCD_Y_Pos
      AND  #%01000000
      BEQ  LcdPutData
      JSR  LcdSelChp2          ; CS2
      
LcdPutData:
      LDA  LCD_PG_DATA      
      JSR  LcdWriteData
      RTS      


LcdWriteCmd:			         ;// escribe el comando que esta en A
      BCLR 3,PTAD  ;// DI
      BCLR 4,PTAD  ;// r/w
      STA  PTDD
      NOP
      NOP
      NOP
      NOP
      NOP
      BSET 5,PTAD  ;// E = 1
      NOP
      NOP
      NOP
      NOP
      NOP
      BCLR 5,PTAD  ;// E = 0
			RTS
			

LcdWriteData:			         ;// escribe el comando que esta en A
      BSET 3,PTAD  ;// DI
      BCLR 4,PTAD  ;// r/w
      STA  PTDD
      NOP
      NOP
      NOP
      NOP
      NOP
      BSET 5,PTAD  ;// E = 1
      NOP
      NOP
      NOP
      NOP
      NOP
      BCLR 5,PTAD  ;// E = 0
			RTS
 

;//////////////////////////////////////////////////////////////////////////////////////////////////

Wait1ns:  ;  34 + 28 * WailCtlH/L ciclos   Ciclos de clock
      PSHA                                 ; 2 
			
Wait1nsLoop1:                              ; 28 ciclos por WaitCtrlH/L
      LDA WaitCntrL                        ; 4 / 3 si es 8 bits la direccion
      BEQ Wait1nsDecH                      ; 3 
			DECA                                 ; 1
			STA WaitCntrL                        ; 4 / 3 si es 8 bits la direccion
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			NOP                                  ; 1
			JMP Wait1nsLoop1                     ; 4 / 3 si es 8 bits la direccion
			
Wait1nsDecH:
			DECA                                 ; 1
			STA WaitCntrL                        ; 4 / 3 si es 8 bits la direccion

      LDA WaitCntrH                        ; 4 / 3 si es 8 bits la direccion
      BEQ Wait1nsEnd                       ; 3

			DECA                                 ; 1
			STA WaitCntrH                        ; 4 / 3 si es 8 bits la direccion
    	JMP Wait1nsLoop1                     ; 4 / 3 si es 8 bits la direccion

Wait1nsEnd:
      PULA                                 ; 3
			RTS                                  ; 5




;**************************************************************
;   Screens foreground data
;**************************************************************



CharMap8x8:
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$F2,$00,$00; // 33
   DC.B    $00,$00,$00,$00,$E0,$00,$E0,$00; 
   DC.B    $00,$00,$28,$FE,$28,$FE,$28,$00; 
   DC.B    $00,$00,$48,$54,$FE,$54,$24,$00; 
   DC.B    $00,$00,$46,$26,$10,$C8,$C4,$00; 
   DC.B    $00,$00,$02,$44,$AA,$92,$6C,$00; 
   DC.B    $00,$00,$00,$00,$C0,$A0,$00,$00; 
   DC.B    $00,$00,$00,$00,$82,$44,$38,$00; 
   DC.B    $00,$00,$00,$00,$38,$44,$82,$00; 
   DC.B    $00,$00,$28,$10,$7C,$10,$28,$00; 
   DC.B    $00,$00,$10,$10,$7C,$10,$10,$00; 
   DC.B    $00,$00,$00,$00,$18,$14,$00,$00; 
   DC.B    $00,$10,$10,$10,$10,$10,$10,$00; 
   DC.B    $00,$00,$00,$00,$00,$0C,$0C,$00; 
   DC.B    $00,$00,$40,$20,$10,$08,$04,$00; 
   DC.B    $00,$00,$7C,$A2,$92,$8A,$7C,$00; 
   DC.B    $00,$00,$00,$02,$FE,$42,$00,$00; 
   DC.B    $00,$00,$62,$92,$8A,$86,$42,$00; 
   DC.B    $00,$00,$8C,$D2,$A2,$82,$84,$00; 
   DC.B    $00,$00,$08,$FE,$48,$28,$18,$00; 
   DC.B    $00,$9C,$A2,$A2,$A2,$E4,$00,$00; 
   DC.B    $00,$00,$0C,$92,$92,$52,$3C,$00; 
   DC.B    $00,$00,$C0,$A0,$90,$8E,$80,$00; 
   DC.B    $00,$00,$6C,$92,$92,$92,$6C,$00; 
   DC.B    $00,$00,$78,$94,$92,$92,$60,$00; 
   DC.B    $00,$00,$00,$00,$6C,$6C,$00,$00; 
   DC.B    $00,$00,$00,$00,$6C,$6A,$00,$00; 
   DC.B    $00,$00,$00,$82,$44,$28,$10,$00; 
   DC.B    $00,$00,$24,$24,$24,$24,$24,$00; 
   DC.B    $00,$00,$10,$28,$44,$82,$00,$00; 
   DC.B    $00,$00,$60,$90,$8A,$80,$40,$00; 
   DC.B    $00,$00,$7C,$82,$9E,$92,$4C,$00; 
   DC.B    $00,$00,$7E,$88,$88,$88,$7E,$00; 
   DC.B    $00,$00,$6C,$92,$92,$92,$FE,$00; 
   DC.B    $00,$00,$44,$82,$82,$82,$7C,$00; 
   DC.B    $00,$00,$38,$44,$82,$82,$FE,$00; 
   DC.B    $00,$00,$82,$92,$92,$92,$FE,$00; 
   DC.B    $00,$00,$80,$90,$90,$90,$FE,$00; 
   DC.B    $00,$00,$5C,$92,$92,$82,$7C,$00; 
   DC.B    $00,$00,$FE,$10,$10,$10,$FE,$00; 
   DC.B    $00,$00,$00,$82,$FE,$82,$00,$00; 
   DC.B    $00,$00,$80,$FC,$82,$02,$04,$00; 
   DC.B    $00,$00,$82,$44,$28,$10,$FE,$00; 
   DC.B    $00,$00,$02,$02,$02,$02,$FE,$00; 
   DC.B    $00,$00,$FE,$40,$30,$40,$FE,$00; 
   DC.B    $00,$00,$FE,$08,$10,$20,$FE,$00; 
   DC.B    $00,$00,$7C,$82,$82,$82,$7C,$00; 
   DC.B    $00,$00,$60,$90,$90,$90,$FE,$00; 
   DC.B    $00,$00,$00,$7A,$84,$8A,$82,$7C; 
   DC.B    $00,$00,$62,$94,$98,$90,$FE,$00; 
   DC.B    $00,$00,$8C,$92,$92,$92,$62,$00; 
   DC.B    $00,$00,$80,$80,$FE,$80,$80,$00; 
   DC.B    $00,$00,$FC,$02,$02,$02,$FC,$00; 
   DC.B    $00,$00,$F8,$04,$02,$04,$F8,$00; 
   DC.B    $00,$00,$FC,$02,$06,$02,$FC,$00; 
   DC.B    $00,$00,$C6,$28,$10,$28,$C6,$00; 
   DC.B    $00,$00,$E0,$10,$0E,$10,$E0,$00; 
   DC.B    $00,$00,$C2,$A2,$92,$8A,$86,$00; 
   DC.B    $00,$00,$00,$00,$82,$82,$FE,$00; 
   DC.B    $00,$00,$A8,$68,$3E,$68,$A8,$00; 
   DC.B    $00,$00,$00,$00,$FE,$82,$82,$00; 
   DC.B    $00,$00,$20,$40,$80,$40,$20,$00; 
   DC.B    $00,$00,$02,$02,$02,$02,$02,$00; 
   DC.B    $00,$00,$00,$00,$20,$40,$80,$00; 
   DC.B    $00,$00,$1E,$2A,$2A,$2A,$04,$00; 
   DC.B    $00,$00,$1C,$22,$22,$22,$FE,$00; 
   DC.B    $00,$00,$00,$22,$22,$22,$1C,$00; 
   DC.B    $00,$00,$FE,$12,$22,$22,$1C,$00; 
   DC.B    $00,$00,$18,$2A,$2A,$2A,$1C,$00; 
   DC.B    $00,$00,$40,$80,$88,$7E,$08,$00; 
   DC.B    $00,$00,$7C,$4A,$4A,$4A,$30,$00; 
   DC.B    $00,$00,$1E,$20,$20,$10,$FE,$00; 
   DC.B    $00,$00,$00,$02,$BE,$22,$00,$00; 
   DC.B    $00,$00,$00,$BC,$02,$02,$04,$00; 
   DC.B    $00,$00,$00,$22,$14,$08,$FE,$00; 
   DC.B    $00,$00,$00,$02,$FE,$82,$00,$00; 
   DC.B    $00,$00,$1E,$20,$18,$20,$3E,$00; 
   DC.B    $00,$00,$1E,$20,$20,$10,$3E,$00; 
   DC.B    $00,$00,$1C,$22,$22,$22,$1C,$00; 
   DC.B    $00,$00,$10,$28,$28,$28,$3E,$00; 
   DC.B    $00,$00,$3E,$18,$28,$28,$10,$00; 
   DC.B    $00,$00,$10,$20,$20,$10,$3E,$00; 
   DC.B    $00,$00,$04,$2A,$2A,$2A,$12,$00; 
   DC.B    $00,$00,$04,$02,$22,$FC,$20,$00; 
   DC.B    $00,$00,$3E,$04,$02,$02,$3C,$00; 
   DC.B    $00,$00,$38,$04,$02,$04,$38,$00; 
   DC.B    $00,$00,$78,$04,$08,$04,$78,$00; 
   DC.B    $00,$00,$44,$28,$10,$28,$44,$00; 
   DC.B    $00,$00,$7C,$12,$12,$12,$60,$00; 
   DC.B    $00,$00,$22,$32,$2A,$26,$22,$00; 
   DC.B    $00,$00,$00,$00,$82,$6C,$10,$00; 
   DC.B    $00,$00,$00,$00,$FE,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$10,$6C,$82,$00; 
   DC.B    $00,$00,$10,$38,$54,$10,$10,$00; 
   DC.B    $04,$04,$14,$14,$0C,$04,$04,$04; 
   DC.B    $04,$04,$14,$14,$0D,$05,$02,$00; 
   DC.B    $00,$54,$14,$0C,$04,$04,$04,$04; 
   DC.B    $04,$04,$54,$14,$0C,$04,$04,$04; 
   DC.B    $04,$04,$54,$14,$0D,$05,$02,$00; 
   DC.B    $0C,$54,$14,$0C,$04,$04,$04,$04; 
   DC.B    $04,$04,$0C,$54,$14,$0C,$04,$04; 
   DC.B    $0C,$54,$14,$0C,$04,$04,$18,$00; 
   DC.B    $4C,$14,$54,$0C,$04,$04,$04,$04; 
   DC.B    $04,$04,$4C,$14,$54,$0C,$04,$04; 
   DC.B    $04,$04,$5E,$15,$4D,$01,$01,$06; 
   DC.B    $98,$A4,$A4,$A4,$A4,$A4,$A4,$44; 
   DC.B    $04,$04,$84,$44,$38,$04,$04,$04; 
   DC.B    $04,$04,$FC,$04,$54,$34,$14,$0C; 
   DC.B    $00,$F8,$04,$04,$04,$04,$04,$04; 
   DC.B    $04,$04,$04,$F8,$04,$04,$04,$04; 
   DC.B    $04,$04,$04,$FC,$02,$02,$0C,$00; 
   DC.B    $0C,$12,$12,$0C,$04,$04,$04,$04; 
   DC.B    $04,$04,$0C,$12,$12,$0C,$04,$04; 
   DC.B    $04,$04,$0C,$12,$12,$0C,$02,$01; 
   DC.B    $00,$18,$04,$44,$04,$04,$04,$04; 
   DC.B    $04,$04,$04,$58,$04,$04,$04,$04; 
   DC.B    $04,$04,$04,$0C,$02,$22,$02,$0C; 
   DC.B    $18,$34,$5C,$14,$0C,$04,$04,$04; 
   DC.B    $04,$1C,$34,$5C,$14,$0C,$04,$04; 
   DC.B    $04,$04,$04,$18,$28,$28,$18,$00; 
   DC.B    $00,$F0,$08,$14,$24,$44,$84,$00; 
   DC.B    $04,$04,$04,$04,$1E,$15,$0D,$00; 
   DC.B    $0C,$14,$05,$04,$05,$04,$04,$04; 
   DC.B    $04,$04,$05,$0C,$05,$04,$04,$04; 
   DC.B    $04,$08,$0A,$05,$01,$01,$06,$00; 
   DC.B    $04,$04,$04,$04,$04,$04,$78,$00; 
   DC.B    $00,$00,$04,$14,$14,$0C,$04,$00; 
   DC.B    $80,$40,$40,$5C,$40,$40,$20,$00; 
   DC.B    $00,$20,$A0,$BC,$60,$20,$00,$00; 
   DC.B    $00,$20,$BE,$B5,$6D,$20,$00,$00; 
   DC.B    $00,$01,$05,$FD,$03,$01,$00,$00; 
   DC.B    $00,$12,$15,$15,$A9,$A1,$66,$20; 
   DC.B    $00,$00,$00,$00,$FC,$00,$00,$00; 
   DC.B    $0C,$14,$04,$05,$04,$14,$0C,$00; 
   DC.B    $00,$00,$4C,$14,$54,$0C,$00,$00; 
   DC.B    $0C,$14,$44,$04,$44,$14,$0C,$00; 
   DC.B    $0C,$14,$44,$84,$44,$14,$0C,$00; 
   DC.B    $04,$04,$0D,$14,$15,$15,$03,$00; 
   DC.B    $04,$04,$0C,$15,$15,$15,$03,$00; 
   DC.B    $04,$04,$0C,$14,$55,$15,$03,$00; 
   DC.B    $00,$00,$1C,$24,$24,$24,$00,$00; 
   DC.B    $00,$00,$1C,$24,$A4,$24,$00,$00; 
   DC.B    $00,$00,$00,$0C,$02,$01,$01,$00; 
   DC.B    $00,$00,$2C,$02,$01,$01,$00,$00; 
   DC.B    $18,$04,$1C,$04,$1E,$01,$01,$06; 
   DC.B    $18,$44,$9C,$44,$1E,$01,$01,$06; 
   DC.B    $18,$14,$14,$0C,$06,$01,$01,$06; 
   DC.B    $18,$54,$14,$0C,$06,$01,$01,$06; 
   DC.B    $00,$00,$44,$28,$10,$28,$44,$00; 
   DC.B    $00,$18,$14,$14,$0C,$7C,$04,$04; 
   DC.B    $00,$18,$54,$14,$0C,$7C,$04,$04; 
   DC.B    $00,$14,$14,$0D,$05,$02,$00,$00; 
   DC.B    $00,$14,$54,$0D,$05,$02,$00,$00; 
   DC.B    $04,$04,$04,$04,$04,$04,$04,$04; 
   DC.B    $0C,$54,$14,$0C,$04,$04,$18,$00; 
   DC.B    $00,$5E,$15,$4D,$01,$01,$06,$00; 
   DC.B    $00,$FC,$04,$54,$34,$14,$0C,$00; 
   DC.B    $00,$00,$0F,$15,$55,$95,$02,$00; 
   DC.B    $00,$00,$7E,$01,$01,$06,$00,$00; 
   DC.B    $00,$00,$0F,$55,$95,$55,$02,$00; 
   DC.B    $0C,$14,$14,$0C,$04,$03,$00,$00; 
   DC.B    $00,$06,$01,$11,$01,$06,$00,$00; 
   DC.B    $00,$00,$0C,$14,$14,$0C,$00,$00; 
   DC.B    $00,$00,$1E,$15,$0D,$00,$00,$00; 
   DC.B    $00,$F0,$08,$14,$A4,$A4,$64,$20; 
   DC.B    $00,$F0,$08,$94,$64,$44,$44,$20; 
   DC.B    $05,$F5,$0B,$15,$24,$44,$84,$00; 
   DC.B    $00,$00,$0C,$55,$95,$55,$0E,$00; 
   DC.B    $00,$00,$0C,$55,$15,$55,$0E,$00; 
   DC.B    $00,$10,$14,$0A,$02,$02,$0C,$00; 
   DC.B    $00,$10,$14,$0B,$02,$03,$0C,$00; 
   DC.B    $00,$00,$00,$40,$9E,$40,$00,$00; 
   DC.B    $00,$00,$00,$40,$1E,$40,$00,$00; 
   DC.B    $A0,$A0,$A0,$00,$00,$00,$00,$00; 
   DC.B    $60,$60,$D0,$C0,$20,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$05,$05,$05; 
   DC.B    $00,$00,$80,$80,$80,$80,$00,$00; 
   DC.B    $04,$F4,$08,$14,$24,$44,$84,$00; 
   DC.B    $C0,$C0,$20,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$01,$01,$01,$01,$00,$00; 
   DC.B    $00,$08,$08,$2A,$08,$08,$00,$00; 
   DC.B    $00,$40,$20,$40,$20,$40,$00,$00; 
   DC.B    $0C,$14,$04,$A4,$A4,$64,$24,$04; 
   DC.B    $04,$04,$04,$AC,$A4,$64,$24,$04; 
   DC.B    $04,$08,$0A,$05,$51,$51,$36,$10; 
   DC.B    $04,$04,$04,$24,$BE,$B5,$6D,$20; 
   DC.B    $04,$04,$04,$18,$A8,$28,$98,$00; 
   DC.B    $04,$84,$44,$44,$58,$40,$40,$20; 
   DC.B    $02,$02,$0A,$12,$0A,$06,$02,$00; 
   DC.B    $04,$04,$04,$18,$A8,$28,$98,$00; 
   DC.B    $04,$84,$44,$44,$58,$40,$40,$20; 
   DC.B    $02,$02,$0A,$12,$0A,$06,$02,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 
   DC.B    $00,$00,$00,$00,$00,$00,$00,$00; 

CharMapNum7x5:
   DC.B    $7F,$41,$41,$41,$7F
   DC.B    $00,$00,$7F,$40,$00
   DC.B    $79,$49,$49,$49,$4F
   DC.B    $7F,$49,$49,$41,$41
   DC.B    $3F,$08,$08,$08,$78
   DC.B    $4F,$49,$49,$49,$79
   DC.B    $0F,$09,$09,$09,$7F
   DC.B    $7F,$48,$48,$40,$40
   DC.B    $7F,$49,$49,$49,$7F
   DC.B    $7F,$48,$48,$48,$78
   DC.B    $00,$00,$00,$00,$00

   
   
CharMapNum8x4:
   DC.B    $FF,$81,$81,$FF;      0
   DC.B    $00,$FF,$00,$00;      1
   DC.B    $61,$91,$89,$87;      2
   DC.B    $FF,$89,$89,$81;      3
   DC.B    $FF,$08,$08,$F8;      4
   DC.B    $86,$89,$89,$F1;      5
   DC.B    $0F,$09,$09,$FF;      6
   DC.B    $FF,$80,$80,$80;      7
   DC.B    $FF,$89,$89,$FF;      8
   DC.B    $FF,$88,$88,$F8;      9


CharMapNum9x5:
   DC.B   $FF,$01,$01,$01,$FF,$01,$01,$01,$01,$01
   DC.B   $FF,$00,$00,$00,$00,$01,$01,$00,$00,$00
   DC.B   $F1,$11,$11,$11,$1F,$01,$01,$01,$01,$01
   DC.B   $FF,$11,$11,$01,$01,$01,$01,$01,$01,$01
   DC.B   $7F,$10,$10,$10,$F0,$00,$00,$00,$00,$01
   DC.B   $1F,$11,$11,$11,$F1,$01,$01,$01,$01,$01
   DC.B   $1F,$11,$11,$11,$FF,$00,$00,$00,$00,$01
   DC.B   $FF,$10,$00,$00,$00,$01,$01,$01,$01,$01
   DC.B   $EF,$11,$11,$11,$EF,$01,$01,$01,$01,$01
   DC.B   $FF,$10,$10,$10,$F0,$01,$01,$01,$01,$01
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00


CharMapNum24x13:
   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$F0,$F0,$F0,$F0,$F0,$FF,$FF,$FF,$FF

   DC.B   $00,$00,$00,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$FF,$FF,$FF,$FF,$F0,$F0,$F0,$F0,$00,$00

   DC.B   $0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $FC,$FC,$FC,$FC,$3C,$3C,$3C,$3C,$3C,$3F,$3F,$3F,$3F
   DC.B   $FF,$FF,$FF,$FF,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F
   DC.B   $FF,$FF,$FF,$FF,$3C,$3C,$3C,$3C,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0

   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$3C,$3C,$3C,$3C,$3C,$FC,$FC,$FC,$FC
   DC.B   $0F,$0F,$0F,$0F,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$FC,$FC,$FC,$FC
   DC.B   $F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$FF,$FF,$FF,$FF

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$FF,$FF,$FF,$FF
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF

   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$3C,$3C,$3C,$3C,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0,$F0

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$3C,$3C,$3C,$3C,$3C,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$F0,$F0,$F0,$F0,$F0,$FF,$FF,$FF,$FF

   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$3C,$3C,$3C,$3C,$3C,$FC,$FC,$FC,$FC
   DC.B   $FF,$FF,$FF,$FF,$F0,$F0,$F0,$F0,$F0,$FF,$FF,$FF,$FF

   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00


CharMapNum30x17:
   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3F,$3F,$3F,$3F

   DC.B   $00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$00,$00,$00

   DC.B   $0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F
   DC.B   $FF,$FF,$FF,$FF,$E0,$E0,$E0,$E0,$E0,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$01,$01,$01,$01,$01,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C

   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0
   DC.B   $FF,$FF,$FF,$FF,$01,$01,$01,$01,$01,$01,$01,$01,$01,$FF,$FF,$FF,$FF
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$3F,$3F,$3F,$3F

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F
   DC.B   $FF,$FF,$FF,$FF,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0
   DC.B   $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$FF,$FF,$FF,$FF
   DC.B   $3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3F,$3F,$3F,$3F

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$FF,$FF,$FF,$FF
   DC.B   $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$FF,$FF,$FF,$FF
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$3F,$3F,$3F,$3F

   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$E0,$E0,$E0,$E0,$E0,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$01,$01,$01,$01,$01,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C

   DC.B   $FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$FF,$FF,$FF,$FF
   DC.B   $FF,$FF,$FF,$FF,$01,$01,$01,$01,$01,$01,$01,$01,$01,$FF,$FF,$FF,$FF
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3F,$3F,$3F,$3F

   DC.B   $FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $FF,$FF,$FF,$FF,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$E0
   DC.B   $FF,$FF,$FF,$FF,$01,$01,$01,$01,$01,$01,$01,$01,$01,$FF,$FF,$FF,$FF
   DC.B   $3F,$3F,$3F,$3F,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3C,$3F,$3F,$3F,$3F

   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   
   
CharMapNum21x32:
;0
   DC.B   $00,$C0,$F0,$F8,$FC,$FE,$FE,$7F,$3F,$1F,$1F,$1F,$1F,$3F,$FE,$FE,$FC,$FC,$F0,$E0,$00
   DC.B   $FC,$FF,$FF,$FF,$FF,$FF,$01,$00,$00,$00,$00,$00,$00,$00,$01,$FF,$FF,$FF,$FF,$FF,$FE
   DC.B   $7F,$FF,$FF,$FF,$FF,$FF,$80,$00,$00,$00,$00,$00,$00,$00,$80,$FF,$FF,$FF,$FF,$FF,$3F
   DC.B   $00,$07,$0F,$3F,$3F,$7F,$7F,$FC,$F8,$F8,$F8,$F8,$FC,$FE,$7F,$7F,$3F,$1F,$0F,$03,$00
;1   
   DC.B   $1F,$1F,$1F,$1F,$1F,$1F,$FF,$FF,$FF,$FF,$FF,$FF,$1F,$1F,$1F,$1F,$1F,$1F,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$00,$00,$80,$80,$C0,$E0,$E0,$E0,$00
   DC.B   $00,$00,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$7F,$3E,$1F,$0F,$0F,$07,$03,$03,$01,$00
;2
   DC.B   $3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$3F,$BF,$FF,$FF,$FF,$FF,$FF,$FF,$7F
   DC.B   $00,$00,$00,$00,$00,$80,$C0,$E0,$F0,$F8,$FC,$FE,$7F,$3F,$1F,$0F,$07,$03,$01,$00,$00
   DC.B   $00,$E0,$F8,$FE,$FF,$FF,$FF,$FF,$1F,$07,$03,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$0F,$3F,$7F,$FF,$FF,$FF,$FF,$FC,$F8,$F0,$F0,$F0,$F0,$F0,$F8,$F8,$7C,$7C,$3C,$00
;3   
   DC.B   $E0,$F8,$FC,$FE,$FE,$FF,$FF,$FF,$3F,$1F,$1F,$0F,$0F,$0F,$0F,$0F,$0F,$1F,$1F,$3E,$3E
   DC.B   $0F,$1F,$3F,$7F,$FF,$FF,$FF,$FF,$FC,$F0,$F0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$00,$00,$00
   DC.B   $00,$F0,$FC,$FE,$FE,$FF,$FF,$FF,$FF,$0F,$07,$07,$03,$03,$03,$03,$03,$03,$00,$00,$00
   DC.B   $00,$03,$0F,$1F,$3F,$7F,$7F,$FF,$FF,$FF,$FE,$FC,$FC,$FC,$FC,$7C,$7E,$7E,$3F,$3F,$00
;4   

   DC.B   $80,$80,$80,$FF,$FF,$FF,$FF,$FF,$FF,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80,$80
   DC.B   $0F,$0F,$0F,$FF,$FF,$FF,$FF,$FF,$FF,$0F,$0F,$0F,$0F,$0F,$8F,$EF,$FF,$FF,$FF,$7F,$1F
   DC.B   $00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$00,$80,$E0,$F8,$FE,$FF,$7F,$1F,$07,$01,$00,$00
   DC.B   $00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FE,$7F,$3F,$0F,$03,$01,$00,$00,$00,$00,$00,$00
   
;5   
   DC.B   $00,$00,$00,$C0,$F0,$F0,$F8,$FC,$FC,$7E,$7F,$3F,$3F,$3F,$3F,$3F,$3E,$7C,$7C,$F8,$F8
   DC.B   $00,$00,$7F,$FF,$FF,$FF,$FF,$FF,$C1,$80,$80,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$01,$07,$07,$0F,$0F,$1F,$1F,$1F,$1F,$1F,$1F,$1F,$FF,$FF,$FF,$FF,$FF,$00
   DC.B   $00,$00,$00,$00,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$FF,$FF,$FF,$FF,$FF,$00
;6   
   DC.B   $00,$E0,$F8,$FC,$FC,$FE,$FE,$7F,$3F,$1F,$1F,$1F,$1F,$3F,$FE,$FE,$FC,$FC,$F0,$E0,$00
   DC.B   $00,$3F,$7F,$FF,$FF,$FF,$FF,$F0,$E0,$C0,$C0,$C0,$C0,$E0,$E0,$FF,$FF,$FF,$FF,$FF,$FE
   DC.B   $00,$00,$00,$00,$01,$01,$07,$07,$07,$07,$07,$07,$07,$03,$C3,$FF,$FF,$FF,$FF,$FF,$3F
   DC.B   $00,$00,$00,$3E,$3E,$7C,$FC,$FC,$FC,$FC,$FC,$FE,$FE,$FF,$7F,$7F,$3F,$1F,$0F,$03,$00

;7   
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$C0,$F8,$FE,$FE,$FE,$FE,$3E,$0E,$02,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$C0,$F0,$FC,$FF,$FF,$FF,$FF,$3F,$0F,$01,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$C0,$F8,$FE,$FF,$FF,$FF,$7F,$1F,$07,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FB,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8,$F8


;8   
   DC.B   $C0,$F0,$F8,$FC,$FE,$FE,$7F,$3F,$1F,$1F,$1F,$1F,$1F,$3F,$7F,$FE,$FE,$FE,$FC,$F8,$E0
   DC.B   $0F,$1F,$3F,$7F,$FF,$FF,$F8,$F0,$E0,$E0,$C0,$E0,$E0,$F0,$F8,$FF,$7F,$7F,$3F,$1F,$07
   DC.B   $00,$C0,$F0,$F8,$FC,$FF,$FF,$3F,$0F,$07,$07,$0F,$1F,$3F,$FF,$FF,$FE,$FC,$F8,$E0,$00
   DC.B   $00,$07,$1F,$3F,$7F,$7F,$FF,$FC,$F8,$F8,$F8,$F8,$F8,$FC,$7F,$7F,$3F,$3F,$1F,$07,$00
 
 
 ;9  
   DC.B   $00,$00,$C0,$F0,$F8,$FC,$FE,$FF,$FF,$7F,$7F,$3F,$3F,$3F,$3F,$3E,$3E,$7C,$7C,$00,$00
   DC.B   $00,$FC,$FF,$FF,$FF,$FF,$FF,$C3,$C0,$E0,$E0,$E0,$E0,$E0,$E0,$E0,$80,$80,$00,$00,$00
   DC.B   $00,$7F,$FF,$FF,$FF,$FF,$FF,$07,$07,$03,$03,$03,$03,$07,$0F,$FF,$FF,$FF,$FF,$FE,$FC
   DC.B   $00,$00,$07,$0F,$3F,$3F,$7F,$FF,$FC,$F8,$F8,$F8,$F8,$FC,$FE,$FF,$7F,$3F,$3F,$1F,$07

;blank
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   DC.B   $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
   


   
