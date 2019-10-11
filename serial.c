/*
 * atividade6.c
 *
 * Created: 30/09/2019 10:08:18
 * Author : Thiago Soares Laitz - 224898
 */  

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char * p_ucsr0a = (unsigned char *) 0xC0; //Aponta para o registrador de controle da UART A
unsigned char * p_ucsr0b = (unsigned char *) 0xC1; //Aponta para o registrador de controle da UART B
unsigned char * p_ucsr0c = (unsigned char *) 0xC2; //Aponta para o registrador de controle da UART C
unsigned char * p_ubrr0h = (unsigned char *) 0xC5; //Aponta para o registrador High de defini??o de baudrate
unsigned char * p_ubrr0l = (unsigned char *) 0XC4; //Aponta para o registrador LOW de defini??o de baudrate
unsigned char * p_udr0 =   (unsigned char *) 0xC6; //Aponta para o registrador de dados da UART

char msg[] = " Out of the night that covers me,\n Black as the Pit from pole to pole,\n I thank whatever gods may be\n For my unconquerable soul.\n In the fell clutch of circumstance\n I have not winced nor cried aloud.\n Under the bludgeonings of chance\n My head is bloody, but unbowed.\n Beyond this place of wrath and tears\n Looms but the Horror of the shade,\n And yet the menace of the years\n Finds, and shall find, me unafraid.\n It matters not how strait the gate,\n How charged with punishments the scroll.\n I am the master of my fate:\n I am the captain of my soul.\n\n";
char msg2[] = "Mensagem transmitida com sucesso!\n\n";

void setup(){
	*p_ucsr0a = *p_ucsr0a & 0xFC; //modo normal de transmiss?o e modo multiprocessador desligado
	*p_ucsr0b = *p_ucsr0b | 0x08; //ativa a transmiss?o UART e 0 em UCSZ02
	*p_ucsr0c = *p_ucsr0c | 0x36; //Numero de bits / frame = 8 ;Modo ass?ncrono;bit de paridade ?mpar; um bit de parada
	*p_ubrr0h = *p_ubrr0h & 0xF0; //Carrega 25 em ubrr0h e ubrr0l para baudrate = 38.4 kbps
	*p_ubrr0l = *p_ubrr0l | 0x19;
}

int main(void)
{
	setup();
	unsigned int contador = 0; //var auxiliar para percorrer a msg

    while (1) 
    {		
		while(msg[contador] != '\0'){ //Transmite a msg at? que o caracter nulo seja encontrado (fim de mensagem)
			*p_udr0 = msg[contador]; //primeiro char colocado no registrador de dados
			contador++; //contador incrementado para no proximo loop avan?ar na mensagem
			while(!(*p_ucsr0a & 0x40)); //Espera at? que o char seja enviado
			*p_ucsr0a = *p_ucsr0a & 0x40; //Ativa a flag de transmiss?o
		}
		contador = 0;
		while(msg2[contador] != '\0'){ //O processo ? refeito para a mensagem 2
			*p_udr0 = msg2[contador];
			contador++;
			while(!(*p_ucsr0a & 0x40));
			*p_ucsr0a = *p_ucsr0a & 0x40;
		}
		_delay_ms(5000);//delay de 5s entre cada trasmiss?o
    }
}

