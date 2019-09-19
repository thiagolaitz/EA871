/*
 * lab05.c
 *
 * Created: 12/09/2019 08:22:53
 * Author : Thiago Soares Laitz - 224898
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

extern void delay (short int q);//cria o protótipo da função externa delay 

short int aux_PinChange;//auxiliar para cada pressionamento do botão contar apenas uma vez
short int estado;
short int tempo;
short int sentido; //1 = horário // 0 = anti horário
unsigned char * sreg = (unsigned char *) 0x5f;//aponta para o registrador sreg 	
unsigned char * eicra = (unsigned char *) 0x69;//aponta para o registrador eicra
unsigned char * eimsk = (unsigned char *) 0x3d;//aponta para o registrador eimsk
unsigned char * pcicr = (unsigned char *) 0x68;//aponta para o registrador pcicr
unsigned char * ddrb = (unsigned char *) 0x24;//aponta para o registrador de direção B
unsigned char * portb = (unsigned char *) 0x25;//aponta para o registrador PortB
unsigned char * ddrd = (unsigned char *) 0x2A;//aponta para o registrador DDRD
unsigned char * ddrc = (unsigned char *) 0x27;//aponta para o registrador DDRC
unsigned char * pcmsk1 = (unsigned char *) 0x6C;//aponta para o registrador PCMSK1

void setup(){
	/*Realiza as configurações iniciais necessárias */
	estado = 0;
	tempo = 1000;
	sentido = 1;
	aux_PinChange = 0;
	*ddrd = *ddrd & 0x00;//defino a porta D como entrada
	*ddrb = *ddrb | 0x07;//Defino os pinos PB0/PB1/PB2 como saída
	*ddrc = *ddrc & 0x00;//Defino a porta C como entrada
	*pcicr = *pcicr | 0x02;//ativa pcie1 (responsável por pcint8)
	*eicra = *eicra | 0x03;//ISC01 e ISC00 = 1
	*eimsk = *eimsk | 0x01;//int0 habilitado
	*sreg = *sreg | 0x80;//ativo a interrupção global
	*portb = *portb & 0x00;//saída inicial dos leds = 000
	*pcmsk1 = *pcmsk1 | 0x01;//Ativa a interrupção pelo pino PCINT8
}

ISR(PCINT1_vect){//tratará a interrupção externa int8 - modo pin change
	if (aux_PinChange == 0){
		if (sentido == 1){
			sentido = 0;
			}else{
			sentido = 1;
		}
		aux_PinChange = 1;
	}else{
		aux_PinChange = 0;
	}
	*sreg = *sreg | 0x80;//ativo a interrupção global
}

ISR(INT0_vect){//tratará a interrupção INT0
	/*Responsável por alterar a duração de cada estado */
	if (tempo == 125){
		 tempo = 1000;
	}else{
		tempo = tempo / 2;
	}
	*sreg = *sreg | 0x80;//ativo a interrupção global
}

int main(void)
{
	setup();
    while (1) 
    {
		//_delay_ms(1000);
		delay(tempo);
		if (sentido == 1){//Sentido horário
			if (estado == 7){
				estado = 0;
			}
			else{
				estado++;
			}
		}else{//sentido anti horário
			if (estado == 0){
				estado = 7;
			}else{
				estado = estado - 1;
			}
		}
		//agora devemos alterar as cores do led RGB
		if (estado == 0){
			//rgb = 0 0 0
			*portb = *portb & 0x00;
			
		}else if (estado == 1){
			//rgb = 1 0 0 (PB0 PB1 PB2)
			*portb = *portb & 0x00;
			*portb = *portb | 0x01;
			
		}else if (estado == 2){
			// rgb = 0 1 0
			*portb = *portb & 0x00;
			*portb = *portb | 0x02;
		}else if (estado == 3){
			//rgb = 0 0 1
			*portb = *portb & 0x00;
			*portb = *portb | 0x04;
		}else if (estado == 4){
			//rgb = 1 1 0
			*portb = *portb & 0x00;
			*portb = *portb | 0x03;
		}else if (estado == 5){
			//rgb = 0 1 1
			*portb = *portb & 0x00;
			*portb = *portb | 0x06;
		}else if (estado == 6){
			//rgb = 1 0 1
			*portb = *portb & 0x00;
			*portb = *portb | 0x05;
		}else if (estado == 7){
			//rgb = 1 1 1 
			*portb = *portb & 0x00;
			*portb = *portb | 0x07;
		}
    }
}
