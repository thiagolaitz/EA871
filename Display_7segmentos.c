/*
 * Lab03.c
 *
 * Created: 22/08/2019 08:10:57
 * Author : Thiago Soares Laitz
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#define pressionado 0x00
#define solto 0x01

unsigned char * p_pinc = (unsigned char *) 0x26;//porta de entrada do sinal do botao
unsigned char * p_ddrc = (unsigned char *) 0x27;//direcao do pino do botao
unsigned char * p_portc = (unsigned char *) 0x28; //porta C
unsigned char * p_portd = (unsigned char *) 0x2B;//saida para o display
unsigned char * p_ddrd = (unsigned char *) 0x2A;//direçao port D

int estado = 2;
int aux;
int num = 0;

void mudar_estado(){
	if(num == 0){
		*p_portd = 0x0c;
	}else if(num == 1){
		*p_portd = 0xb6;
	}else if(num == 2){
		*p_portd = 0x9e;
	}else if(num == 3){
		*p_portd = 0xcc;
	}else if(num == 4){
		*p_portd = 0xda;
	}else if(num == 5){
		*p_portd = 0xf8;
	}else if(num == 6){
		*p_portd = 0x0e;
	}else if(num == 7){
		*p_portd = 0xfe;
	}else if(num == 8){
		*p_portd = 0xce;
	}else if(num == 9){
		*p_portd = 0x7e;
	}
}

void contar(){
	if(num != 9) {
		num++;
	}else{
		num = 0;
	}
}

void debounce(){
//realiza o debounce em software
	aux = (*p_pinc & 0x01);
	_delay_ms(20);
}
	
int main(void){
    /* Replace with your application code */
	*p_ddrc = *p_ddrc & 0x00; //saida: A0
	*p_ddrd = *p_ddrd | 0xFF; //*p_ddrd | 0xFE; //entradas : port D[7-1]
	// o Programa começa no estado 2 e com o valor 0 no display:
	*p_portd = *p_portd & 0x00;
	*p_portd = *p_portd | 0x7E;
	*p_portc = *p_portc | 0x01;
	
    while (1) 
    {
		debounce();
		
		if(((*p_pinc & 0x01) == pressionado) & (aux == pressionado) & (estado == 2)){//transição 2 => 1
			estado = 1;
		}
		if(((*p_pinc & 0x01) == solto) & (aux == solto) & (estado == 1)){//transiçao 1 => 2
			estado = 2;
			contar();
			mudar_estado();
		}
	}
}
