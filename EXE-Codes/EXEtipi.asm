STACKSG SEGMENT PARA STACK 'STACK'
    DW 32 DUP(?)
    
STACKSG ENDS

DATASG SEGMENT PARA 'DATA'

SAYI DB ?
ELEMAN DW ?

DATASG ENDS

CODESG SEGMENT PARA 'CODE'
    ASSUME CS:CODESG,DS:DATASG,SS:STACKSG
    
BASLA PROC FAR
    ;Programin geri donus adreslerini yigin olarak tanimlanmis kesimde sakla
    
    PUSH DS
    XOR AX,AX
    PUSH AX
    ;Degisken tanimlandigi kesime erismek icin gereken islemler
    MOV AX,DATASG
    MOV DS,AX
    ;Programin esas kodu bu satirdan itibaren baslar.
    
    ;..
    ;..
    ;..
    
    RETF
    
BASLA ENDP
CODESG ENDS
END BASLA

