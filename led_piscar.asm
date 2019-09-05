; delay.asm
;
; Created: 05/09/2019 08:15:13
; Author : Thiago Soares Laitz - 224898

;O compilador passa o parâmetro da subrotina pelos registradores r25 (MSB) e r24
.cseg

;Carregados inicialmente com o parâmetro de 1s (0x03E8 = 1000)
ldi r25, 0x03
ldi r24, 0xE8

sbi 0x04, 5;configura o pino como saída

piscar:
	sbi 0x05, 5;acende o led interno
	rcall delay
	cbi 0x05, 5;desliga o led interno
	rcall delay
	rjmp piscar


delay:
	;Devemos salvar todos os registradores na pilha a fim de não perdermos nenhuma informação
	push r27
	push r26
	push r17
	push r18
	push r19
	push r1

	;passagem dos parâmetros - 16 bits; 100x = 0,1s , 1000x = 1s
	mov r27, r25
	mov r26, r24;o registrador X recebe o parametro

	;r17: registrador auxiliar de condição de parada (registrador X zerado)
	;Cada loop da subrotina decrementa em 1 o registrador X (r27, r26)
	loop:; esta rotina deve ter exatamente 16000 ciclos
		ldi r17,0
		or r17,r27
		or r17,r26;o único caso positivo é quando r26 = r27 = 0
		cpi r17,0
		breq fim
		ld r1, X+
		ld r1, -X
		nop
		nop
		nop
		ld r1, -X
		ldi r18, 235;parametro para o loop_int

	loop_int:
		cpi r18,0 
		breq fora 
		ldi r19,12 
		nop
		nop
		nop

	loop_int_2:
		;Gasta 5 ciclos
		dec r19 
		cpi r19,0
		breq volta_loop_int
		rjmp loop_int_2 

	volta_loop_int:
		dec r18 
		rjmp loop_int 

	fora:
		rjmp loop

	fim:;acrescentar RET para voltar da subrotina no projeto final
		pop r1
		pop r19
		pop r18
		pop r17
		pop r26
		pop r27
		ret
