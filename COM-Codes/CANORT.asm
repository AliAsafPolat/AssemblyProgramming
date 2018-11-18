PAGE 60,80
TITLE CAN HESAPLAMA

CODESG SEGMENT PARA 'CODE'
	ORG 100H
	ASSUME CS:CODESG,SS:CODESG,DS:CODESG,ES:CODESG
	
	BILGI :JMP BASLA
	DIZI 	 DW 50,50,40,40,60,30,60,30,20,20
	ELEMAN	 DW 10
	ORTALAMA DW 0
	ESIK 	 DW 40
	ESIKUSTU DW 0
	ESIKSAY  DW 0
	BUT		 DW 0
	BASARILI DW 0
	KALDI 	 DW 0

BASLA PROC NEAR
	
			XOR SI,SI
			LEA SI,DIZI
			MOV CX,ELEMAN

L1: 
			MOV AX,[SI]     ;ilk eleman atandı.
			CMP AX,ESIK	;esikten buyukse kalmadı labelıne zıplatıyorum ve orada esıkten buyuk olanların sayısı bir artırıyorum.
			JAE KALMADI	
			INC KALDI	;esikten kucukse kaldı sayısını bir artırıyorum.
			JMP SON		;ve esikten buyuklerın sayısına etkilemesın dıye son etiketıne zıplatıyorum.
KALMADI:	
			INC ESIKSAY
			ADD ESIKUSTU,AX
			
SON:		ADD SI,2
			LOOP L1			;ilk loopum  esigi gecenlerin ortalamasını hesaplamak ıcın tasarlandı ve kalanların sayısı belırlendı
			
			XOR AX,AX
			XOR DX,DX		;ıkıncı loopum ıse esıgın ustundekılerı kontrol edıp kımın basarılı kimin bute girecegini hesaplamak icin tasarlandı.
			MOV AX,ESIKUSTU		; esıkustu degiskenim esigi gecenlerinin notlarının toplandigi degiskendir.
			DIV ESIKSAY		; bu toplami esigi gecenlerin sayisina boldugumde ortalama hesaplanmis olacaktir.
			MOV ORTALAMA,AX
			
			XOR SI,SI
			MOV CX,ELEMAN
			LEA SI,DIZI
			
L2:			MOV AX,[SI]		
			CMP AX,ORTALAMA
			JB	BUTE 		;Eger deger ortalamadan kucukse BUTE girecektir ve basarili sayılmaz bu sebepten zıplatıldi
			INC BASARILI		;Basarilılarin sayisini bulursam esigi gecenlerden basarili olanları çıkardıgım zaman büte girecekleri 
	BUTE :					;hesaplarim demektir.	
			ADD SI,2
			LOOP L2
			
			XOR AX,AX
			MOV AX,ESIKSAY
			SUB AX,BASARILI
			MOV BUT,AX
			
			
			RET
		BASLA ENDP
CODESG ENDS
END BILGI		
			
			
			
			
			
			
	
