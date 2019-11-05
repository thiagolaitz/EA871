/*
 * atividade9.c
 *
 * Created: 05/11/2019 09:51:18
 * Author : Thiago Soares Laitz
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int contador = 0;//contador para contar o numero de overflows do temporizador
volatile int sentido = 1;//1 = incrementando -- 0 = decrementando

void setup(){
	asm("sei");
	TCCR2A = 0xE3;//
	TCCR2B = 0x0C;//Fast PWM Mode e Prescaler = 64
	OCR2A = 0xC8;//Valor máximo do temporizador
	OCR2B = 0;//valor inicial PWM
	TIMSK2 = 0x02;
	DDRD = 0x08;//defino o pino como saída
}

int main(void)
{
    /* Replace with your application code */
    setup();	
	do{
	} while (1);
}

ISR(TIMER2_COMPA_vect){
	contador++;
	if(sentido == 1){//subida
		if((contador % 6) == 0){
			OCR2B++;
		}
		if((contador % 25) == 0){
			OCR2B++;
		}
		if(OCR2B == 0xC8){
			sentido = 0;
		}
	}else{//descida
		if((contador % 6) == 0){
			OCR2B--;
		}
		if((contador % 25) == 0){
			OCR2B--;
		}
		if(OCR2B == 0){
			sentido = 1;
		}
	}
	
}
