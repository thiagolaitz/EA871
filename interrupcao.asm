/*
 * lab05.c
 *
 * Created: 12/09/2019 08:22:53
 * Author : Thiago Soares Laitz - 224898
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

extern void delay (short int q);//cria o protótipo da função externa delay 

short int estado;
short int tempo;
short int sentido; //1 = horário // 0 = anti horário
unsigned char * sreg = (unsigned char *) 0x5f;//aponta para o registrador sreg 	
unsigned char * eicra = (unsigned char *) 0x69;//aponta para o registrador eicra
unsigned char * eimsk = (unsigned char *) 0x3d;//aponta para o registrador eimsk
unsigned char * pcicr = (unsigned char *) 0x68;//aponta para o registrador pcicr

void setup(){
	/*Realiza as configurações iniciais necessárias */
	estado = 0;
	tempo = 1000;
	sentido = 1;
	*pcicr = *pcicr | 0x01;//ativa pcie0 (responsável por pcint1)
	*eicra = *eicra | 0x03;//ISC01 e ISC00 = 1
	*eimsk = *eimsk | 0x01;//int0 habilitado
	*sreg = *sreg | 0x80;//ativo a interrupção global
}

ISR(PCINT1_vect){//tratará a interrupção externa int1 - modo pin change
	
	if (sentido == 1){
		sentido = 0;
	}else{
		sentido = 1;
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
    while (1) 
    {
		delay(tempo);
		if (sentido == 1){//Sentido horário
			if (estado == 7){
				estado = 0;
			}
			else{
				estado++
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
		}else if (estado == 1){
			//rgb = 1 0 0
		}else if (estado == 2){
			// rgb = 0 1 0
		}else if (estado == 3){
			//rgb = 0 0 1
		}else if (estado == 4){
			//rgb = 1 1 0
		}else if (estado == 5){
			//rgb = 0 1 1
		}else if (estado == 6){
			//rgb = 1 0 1
		}else if (estado == 7){
			//rgb = 1 1 1 
		}
		
    }
}

