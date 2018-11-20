PAGE 60,80
TITLE ELDELI CIKARMA

STACKSG SEGMENT PARA STACK 'STACK'
	DW 20 DUP(?)
STACKSG ENDS

DATASG SEGMENT PARA 'DATA'
	
	SAYI1 DW 1234H
	SAYI2 DW 00FFH
	SONUC1 DW 0
	SONUC2 DW 0
	
DATASG ENDS

CODESG SEGMENT PARA 'CODE'
	ASSUME CS:CODESG,SS:STACKSG,DS:DATASG
	
	BASLA PROC FAR
	
		PUSH DS
		XOR AX,AX
		PUSH AX
		
		MOV AX,DATASG
		MOV DS,AX
		
		XOR SI,SI
		
		MOV AX,SAYI1
		SUB AX,SAYI2
		MOV SONUC1,AX
		
		XOR AX,AX
		MOV AL,BYTE PTR[SAYI1]
		MOV AH,BYTE PTR [SAYI2]
		SUB AL,AH
		MOV BYTE PTR [SONUC2],AL
		MOV AL,BYTE PTR[SAYI1+1]
		MOV AH,BYTE PTR[SAYI2+1]
		SBB AL,AH
		MOV BYTE PTR [SONUC2+1],AL		
		
		RETF
		BASLA ENDP
		CODESG ENDS
		END BASLA
		
	
