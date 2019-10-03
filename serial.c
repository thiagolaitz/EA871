/*
 * atividade6.c
 *
 * Created: 30/09/2019 10:08:18
 * Author : Thiago Soares Laitz
 */  

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char * p_ucsr0a = (unsigned char *) 0xC0;
unsigned char * p_ucsr0b = (unsigned char *) 0xC1;
unsigned char * p_ucsr0c = (unsigned char *) 0xC2;
unsigned char * p_ubrr0h = (unsigned char *) 0xC5;
unsigned char * p_ubrr0l = (unsigned char *) 0XC4;
unsigned char * p_udr0 =   (unsigned char *) 0xC6;

char msg[] = " Out of the night that covers me,\n Black as the Pit from pole to pole,\n I thank whatever gods may be\n For my unconquerable soul.\n In the fell clutch of circumstance\n I have not winced nor cried aloud.\n Under the bludgeonings of chance\n My head is bloody, but unbowed.\n Beyond this place of wrath and tears\n Looms but the Horror of the shade,\n And yet the menace of the years\n Finds, and shall find, me unafraid.\n It matters not how strait the gate,\n How charged with punishments the scroll.\n I am the master of my fate:\n I am the captain of my soul.\n\n";
char msg2[] = "Mensagem transmitida com sucesso!\n\n";

void setup(){
	*p_ucsr0a = *p_ucsr0a & 0xFC;
	*p_ucsr0b = *p_ucsr0b | 0x08;
	*p_ucsr0c = *p_ucsr0c | 0x36;
	*p_ubrr0h = *p_ubrr0h & 0xF0;
	*p_ubrr0l = *p_ubrr0l | 0x19;
}

int main(void)
{
	setup();
	unsigned int contador = 0;

    while (1) 
    {		
		while(msg[contador] != '\0'){
			*p_udr0 = msg[contador];
			contador++;
			while(!(*p_ucsr0a & 0x20));
		}
		contador = 0;
		while(msg2[contador] != '\0'){
			*p_udr0 = msg2[contador];
			contador++;
			while(!(*p_ucsr0a & 0x20));
		}
		_delay_ms(5000);
    }
}

