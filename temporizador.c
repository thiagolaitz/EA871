/*
 * atividade8.c
 *
 * Created: 24/10/2019 09:12:08
 * Author : Thiago Soares Laitz - 224898
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/*eventos: 500ms e 780ms / 16 MHz/ modo CTC = 100/ timer2: 8 bits/ faixa de contagem:6,25 uS/
Prescaler = 32/ faixa de contagem = 32*100/16MHz = 200uS/ 2500x => 500ms 3900x => 780mS
tccr2a = 1100--10 (seta a flag 0c2a na comparação / modo CTC de operação)
tccr2b = 00--0011(wgm22 = 0 , prescaler = 32)
timsk2 = -----010 (ativa interrupção OCIE2A)
OCR2A = 0x64 == 100
*/ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int contador_int = 0;
volatile int contador_ext = 0;
volatile int contador = 0;
unsigned char * sreg = (unsigned char *) 0x5f;//aponta para o registrador sreg 	
unsigned char * p_ucsr0a = (unsigned char *) 0xC0; //Aponta para o registrador de controle da UART A
unsigned char * p_ucsr0b = (unsigned char *) 0xC1; //Aponta para o registrador de controle da UART B
unsigned char * p_ucsr0c = (unsigned char *) 0xC2; //Aponta para o registrador de controle da UART C
unsigned char * p_ubrr0h = (unsigned char *) 0xC5; //Aponta para o registrador High de definicao de baudrate
unsigned char * p_ubrr0l = (unsigned char *) 0XC4; //Aponta para o registrador LOW de definicao de baudrate
unsigned char * p_udr0 =   (unsigned char *) 0xC6; //Aponta para o registrador de dados da UART
char msg[] = "Atividade 8 – Interrupcoes temporizadas tratam concorrencia entre tarefas! \n";

void setup(){
	*sreg = *sreg | 0x80;
	TCCR2A = 0b11000010;
	TCCR2B = 0b00000011;
	TIMSK2 = 0b00000010;
	OCR2A = 0x64;
	DDRB = 0x30;
	PORTB = 0x00;
	*p_ucsr0a = *p_ucsr0a & 0xFC; //modo normal de transmiss?o e modo multiprocessador desligado
	*p_ucsr0b = *p_ucsr0b | 0x08; //ativa a transmiss?o UART e 0 em UCSZ02
	*p_ucsr0c = *p_ucsr0c | 0x36; //Numero de bits / frame = 8 ;Modo ass?ncrono;bit de paridade ?mpar; um bit de parada
	*p_ubrr0h = *p_ubrr0h & 0xF0; //Carrega 25 em ubrr0h e ubrr0l para baudrate = 38.4 kbps
	*p_ubrr0l = *p_ubrr0l | 0x19;
}

int main(void){
	setup();
	do{
		while(msg[contador] != '\0'){ //Transmite a msg at? que o caracter nulo seja encontrado (fim de mensagem)
			*p_udr0 = msg[contador]; //primeiro char colocado no registrador de dados
			contador++; //contador incrementado para no proximo loop avan?ar na mensagem
			while(!(*p_ucsr0a & 0x40)); //Espera at? que o char seja enviado
			*p_ucsr0a = *p_ucsr0a & 0x40; //Ativa a flag de transmiss?o
		}
		contador = 0;
		_delay_ms(5000);
	}while(1);
}

void toggle(){//Troca o estado do LED interno
	if((PORTB & 0x20) == 0x20){
		PORTB = PORTB & 0x10;
	}else{
		PORTB |= 0x20;
	}
}
void toggle_ext(){//Troca o estado do LED externo
	if((PORTB & 0x10) == 0x10){
		PORTB = PORTB & 0x20;
	}else{
		PORTB |= 0x10;
	}
}

ISR(TIMER2_COMPA_vect){
	contador_int++;
	contador_ext++;
	if(contador_int == 2500){//caso o contador interno alcance 2500 => passou 0,5s
		toggle();
		contador_int = 0;
	}
	if(contador_ext == 3900){//caso o contador externo alcance 3900 => passou 0,78s
		toggle_ext();
		contador_ext = 0;
	}
}
