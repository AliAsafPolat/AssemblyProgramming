PAGE 60,80
TITLE DIZIKARSILASTIRMA
   
   STACKSG SEGMENT PARA STACK 'STACK'
    DW 32 DUP(?)
   ENDS
   
   DATASG SEGMENT PARA 'DATA'
   SAYI DW  5
    ELEMAN1 DW 1,2,3,4,5
    ELEMAN2 DW 2,1,5,4,3
    COUNT1 DW 0
    COUNT2 DW 0
    
   ENDS
   
   CODESG SEGMENT PARA 'CODE'
       ASSUME CS:CODESG,SS:STACKSG,DS:DATASG       
              
    BASLA PROC FAR
        
        MOV AX,DATASG
        MOV DS,AX
        
             
         LEA SI,ELEMAN1
         LEA DI,ELEMAN2
            MOV CX,SAYI
    L1:  MOV AX,[SI]
         MOV BX,[DI]
         
         CMP AX,BX
         JE SON
         JB ELEMAN2BIG
         INC COUNT1 
         JMP SON
   ELEMAN2BIG:
          INC COUNT2
      SON:   
      ADD SI,2
      ADD DI,2
      LOOP L1             
              
              MOV AX,COUNT1
              MOV BX,COUNT2
              
        
        
        
        
        
        RETF
     BASLA ENDP   
      CODESG ENDS
        END BASLA
    
    


