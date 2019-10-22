/*
 * atividade7.c
 *
 * Created: 03/10/2019 09:28:08
 * Author : Thiago Soares Laitz - 224898
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

unsigned char * sreg			  = (unsigned char *) 0x5f;//aponta para o registrador sreg 	
unsigned char * p_ucsr0a		  = (unsigned char *) 0xC0;
unsigned char * p_ucsr0b		  = (unsigned char *) 0xC1;
unsigned char * p_ucsr0c		  = (unsigned char *) 0xC2;
unsigned char * p_ubrr0h		  = (unsigned char *) 0xC5;
unsigned char * p_ubrr0l		  = (unsigned char *) 0XC4;
volatile unsigned char * p_udr0   = (unsigned char *) 0xC6;
unsigned char * ddrb			  = (unsigned char *) 0x24;//aponta para o registrador de direção B
volatile unsigned char * portb    = (unsigned char *) 0x25;//aponta para o registrador PortB

volatile int contador = 0;
char buffer[10];
int i = 0;//guarda a posição livre do buffer
int j = 0;//Posiçao do elemento mais antigo nao processado
int espacos = 10; //numero de espaços para escrever 
volatile int fim = 1;//indica o fim de uma transmissao de msg
volatile char msg[30];

volatile int flag = 0;//indica se esta ou não imprimindo um comando

void setup(){
	*p_ucsr0a = *p_ucsr0a & 0xFC;//modo multiprocessador e velocidade dupla desativados
	*p_ucsr0b = 0xD8;//Ativado a recepção e transmissão/ interrupções de recepçao, buffer vazio e transmissão completa
	*p_ucsr0c = 0x0E;//Modo assíncrono/ Sem bits de paridade/ dois bits de parada/ baudrate = 9600 bps/ 8bits por frame
	*p_ubrr0h = *p_ubrr0h & 0xF0;
	*p_ubrr0l = 0x67;//baudrate 9600bps = 0x67 = 103
	*sreg     = *sreg | 0x80;//ativa a interrupção global
	*ddrb = 0x07;//Defino os pinos PB0/PB1/PB2 como saída
	*portb = *portb & 0x00;//saída inicial dos leds = 000
}

char remover_buffer(){
/*Remove um item do buffer no caso de não estar vazio*/
	int aux = j;
	if(espacos != 10){
		if(j != 9){
			j++;
		}else{
			j = 0;
		}
		espacos++;
	}
	return buffer[aux];
}

void processar(){
/*Responsável por processar os dados dentro do buffer circular*/
	if((flag == 0) & (fim == 1)){//se não estiver enviando nenhum comando
		char c = remover_buffer();
		if(c == 'r'){//r = acender vermelho
			*portb = *portb & 0x00;
			*portb = *portb | 0x01;
			if(fim == 1){
				flag = 1;
				strcpy(msg, "Comando: Acender LED - cor vermelha \n\0");
				contador = 0;
				//flag = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}
			_delay_ms(200);
		}else if(c == 'g'){//g = acender verde
			*portb = *portb & 0x00;
			*portb = *portb | 0x02;
			if(fim == 1){
				strcpy(msg, "Comando: Acender LED - cor verde \n\0");
				contador = 0;
				flag = 1;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}
			_delay_ms(200);
		}else if(c == 'b'){//b = acender azul
			*portb = *portb & 0x00;
			*portb = *portb | 0x04;
			_delay_ms(200);
			/*if(fim == 1){
				strcpy(msg, "Comando: Acender LED - cor azul\n");
				contador = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}*/
		}else if(c == 121){//y = acender amarelo
			*portb = *portb & 0x00;
			*portb = *portb | 0x03;
			_delay_ms(200);
			/*if(fim == 1){
				strcpy(msg, "Comando: Acender LED - cor amarela\n");
				contador = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}*/
		}else if(c == 99){//c = acender ciano
			*portb = *portb & 0x00;
			*portb = *portb | 0x06;
			_delay_ms(200);
			/*if(fim == 1){
				strcpy(msg, "Comando: Acender LED - cor ciano\n");
				contador = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}*/
		}else if(c == 109){//m = acender magenta
			*portb = *portb & 0x00;
			*portb = *portb | 0x05;
			_delay_ms(200);
			/*if(fim == 1){
				strcpy(msg, "Comando: Acender LED - cor magenta\n");
				contador = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}*/
		}else if(c == 119){//w = acender branco
			*portb = *portb & 0x00;
			*portb = *portb | 0x07;
			_delay_ms(200);
			/*if(fim == 1){
				strcpy(msg, "Comando: Acender LED - cor branco\n");
				contador = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}*/
		}else{//char invalido: apagado
			*portb = *portb & 0x00;
			/*if(fim == 1){
				strcpy(msg, "Comando incorreto\n");
				contador = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;
			}*/
		}
	}
}

void adicionar_buffer(char c) {
/* Adiciona um item ao buffer se ele não estiver cheio */
	if (espacos != 0){
		buffer[i] = c;
		if(i != 9){
			i++;
		}else{
			i = 0;
		}
		espacos--;
	}
}

int main(void){
	setup();
	do{
		if(espacos == 10){
			*portb = *portb & 0x00;
			if(fim == 1){//se a transmissão já terminou
				strcpy(msg, "Vazio!\n");
				contador = 0;
				*p_udr0 = msg[contador];
				contador = 1;
				fim = 0;//Comecei uma transmissão
			}
		}else{
			processar();
		}
		
	} while(1);
	
	//return 0;
}

ISR(USART_RX_vect){
/*Rotina de interrupção: recepção completa*/
	char m;
	m = *p_udr0;
	//*portb = *portb | 0x02;
	adicionar_buffer(m);
	//processar(m);
}

ISR(USART_TX_vect){
/*Rotina responsável por transmitir os caracteres restantes*/ 
//o primeiro char é passado fora da interrupção contador deve ser 1

	if(msg[contador] != '\0'){
		*p_udr0 = msg[contador];
		contador++;
	}else{
		if(flag == 1){//se estava printando um comando => acabou
			flag = 0;
		}//indica que a transmissão acabou
		fim = 1;
	}
}
