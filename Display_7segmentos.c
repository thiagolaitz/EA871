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
unsigned char * p_portc = (unsigned char *) 0x28; //porta C do botao
unsigned char * p_portd = (unsigned char *) 0x2B;//saida para o display
unsigned char * p_ddrd = (unsigned char *) 0x2A;//direçao port D

int estado = 2;//estado inicial da maquina de estados
int aux;//variavel para realizar o debounce
int num = 0;//numero atual no display

void mudar_estado(){
	if(num == 0){
		*p_portd = 0x0c;//mostra o valor 1 no display
	}else if(num == 1){
		*p_portd = 0xb6;//mostra o valor 2 no display
	}else if(num == 2){
		*p_portd = 0x9e;//mostra o valor 3 no display
	}else if(num == 3){
		*p_portd = 0xcc;//mostra o valor 4 no display
	}else if(num == 4){
		*p_portd = 0xda;//mostra o valor 5 no display
	}else if(num == 5){
		*p_portd = 0xf8;//mostra o valor 6 no display
	}else if(num == 6){
		*p_portd = 0x0e;//mostra o valor 7 no display
	}else if(num == 7){
		*p_portd = 0xfe;//mostra o valor 8 no display
	}else if(num == 8){
		*p_portd = 0xce;//mostra o valor 9 no display
	}else if(num == 9){
		*p_portd = 0x7e;//mostra o valor 0 no display
	}
}

void debounce(){
//realiza o debounce em software
	aux = (*p_pinc & 0x01);
	_delay_ms(20);
}
	
int main(void){
    /* Seta o conteúdo dos registradores para a correta operação*/
	*p_ddrc = *p_ddrc & 0x00; //registrador da porta C definido com entrada
	*p_ddrd = *p_ddrd | 0xFF; //*p_ddrd | 0xFE; //entradas : port D[7-1]
	//o Programa começa com o valor 0 no display:
	*p_portc = *p_portc | 0x01;//setado como valor alto para utilizar o resistor de pull up
	*p_portd = *p_portd & 0x00;
	*p_portd = *p_portd | 0x7E;
	
    while (1) 
    {
		debounce();
		
		if(((*p_pinc & 0x01) == pressionado) & (aux == pressionado) & (estado == 2)){//transição 2 => 1
			/*Caso o botao seja pressionado e não seja um ruído, o estado é alterado para 1 */
			estado = 1;
			mudar_estado();
			if(num != 9) {
				num++;
				}else{
				num = 0;
				}
		}
		if(((*p_pinc & 0x01) == solto) & (aux == solto) & (estado == 1)){//transiçao 1 => 2
			/*Caso o botão seja solto e não seja um ruído, o estado é trocado para 2 */
			estado = 2;
		}
	}
	return 0;
}
