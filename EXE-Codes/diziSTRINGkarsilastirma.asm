PAGE 60,80
TITLE DIZIKARSILASTIRMA
   
   STACKSG SEGMENT PARA STACK 'STACK'
    DW 32 DUP(?)
   ENDS
   
   DATASG SEGMENT PARA 'DATA'
   SAYI DW  8
    ELEMAN1 DB 'ASAFREIS'
    ELEMAN2 DB 'ASFARESA'
    COUNT1 DB 0
   
    
   ENDS
   
   CODESG SEGMENT PARA 'CODE'
       ASSUME CS:CODESG,SS:STACKSG,DS:DATASG       
              
    BASLA PROC FAR
                  
        PUSH DS
        XOR AX,AX
        PUSH AX          
                  
        MOV AX,DATASG
        MOV DS,AX
        
             
         LEA SI,ELEMAN1
         LEA DI,ELEMAN2
            MOV CX,SAYI
    L1:  MOV AL,[SI]
         MOV BL,[DI]
         
         CMP AL,BL
         JNE SON
          INC COUNT1
          
         
      SON:   
      ADD SI,1
      ADD DI,1
      LOOP L1             
              XOR AX,AX
              MOV AL,COUNT1
              
              
        
        
        
        
        
        RETF
     BASLA ENDP   
      CODESG ENDS
        END BASLA
    
    


