/*
 * atividade10.c
 * Created: 10/11/2019 14:32:10
 * Author : Thiago Soares Laitz - 224898
*/ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

/* Para se atingir a frequência de PWM de 62,5 kHz podemos atualizar o valor de amostragem a cada overflow do temporizador,
uma vez que com 256 posições para o overflow teremos uma frequência de PWM = 256/16*10^6 = 62,5 kHz */

volatile char comando = 'k'; //Guarda qual sinal deve ser gerado em OCR2A
volatile int indice = 0; //Utilizado para percorrer o vetor de amostragem

unsigned char dente[200] = {0, 1, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 15, 17, 18, 19, 20, 22, 23, 24, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 43, 45, 46, 47, 48, 50, 51, 52, 54, 55, 56, 57, 59, 60, 61, 62, 64, 65, 66, 68, 69, 70, 71, 73, 74, 75, 77, 78, 79, 80, 82, 83, 84, 85, 87, 88, 89, 91, 92, 93, 94, 96, 97, 98, 99, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 115, 116, 117, 119, 120, 121, 122, 124, 125, 126, 128, 129, 130, 131, 133, 134, 135, 136, 138, 139, 140, 142, 143, 144, 145, 147, 148, 149, 150, 152, 153, 154, 156, 157, 158, 159, 161, 162, 163, 164, 166, 167, 168, 170, 171, 172, 173, 175, 176, 177, 179, 180, 181, 182, 184, 185, 186, 187, 189, 190, 191, 193, 194, 195, 196, 198, 199, 200, 201, 203, 204, 205, 207, 208, 209, 210, 212, 213, 214, 215, 217, 218, 219, 221, 222, 223, 224, 226, 227, 228, 230, 231, 232, 233, 235, 236, 237, 238, 240, 241, 242, 244, 245, 246, 247, 249, 250, 251, 252, 254};
unsigned char am[200] = {128, 141, 155, 168, 181, 192, 203, 211, 218, 223, 226, 226, 223, 218, 211, 201, 189, 176, 161, 145, 128, 110, 93, 76, 60, 46, 34, 23, 16, 10, 8, 9, 13, 19, 29, 41, 55, 71, 89, 108, 127, 147, 166, 185, 202, 217, 230, 241, 249, 253, 255, 253, 249, 241, 230, 217, 202, 185, 166, 147, 128, 108, 89, 71, 55, 41, 29, 19, 13, 9, 8, 10, 16, 23, 34, 46, 60, 76, 93, 110, 127, 145, 161, 176, 189, 201, 211, 218, 223, 226, 226, 223, 218, 211, 203, 192, 181, 168, 155, 141, 128, 114, 102, 91, 81, 72, 65, 60, 57, 55, 56, 58, 62, 67, 73, 81, 90, 99, 108, 118, 127, 137, 145, 153, 160, 166, 171, 175, 177, 178, 178, 177, 174, 171, 166, 161, 155, 148, 142, 135, 128, 121, 114, 108, 102, 97, 93, 89, 87, 86, 85, 86, 87, 89, 93, 97, 102, 108, 114, 121, 127, 135, 142, 148, 155, 161, 166, 171, 174, 177, 178, 178, 177, 175, 171, 166, 160, 153, 145, 137, 128, 118, 108, 99, 90, 81, 73, 67, 62, 58, 56, 55, 57, 60, 65, 72, 81, 91, 102, 114};
unsigned char triangular[200] = {0, 2, 4, 7, 9, 12, 14, 17, 19, 22, 25, 27, 30, 32, 35, 37, 40, 43, 45, 48, 50, 53, 55, 58, 60, 63,  66, 68, 71, 73, 76, 78, 81, 83, 86, 89, 91, 94, 96, 99, 101, 104, 107, 109, 112, 114, 117, 119, 122, 124, 127, 130, 132, 135, 137, 140, 142, 145, 147, 150, 153, 155, 158, 160, 163, 165, 168, 171, 173, 176, 178, 181, 183, 186, 188, 191, 194, 196, 199, 201, 204, 206, 209, 211, 214, 217, 219, 222, 224, 227, 229, 232, 235, 237, 240, 242, 245, 247, 250, 252, 255, 255, 252, 250, 247, 245, 242, 240, 237, 235, 232, 229, 227, 224, 222, 219, 217, 214, 211, 209, 206, 204, 201, 199, 196, 194, 191, 188, 186, 183, 181, 178, 176, 173, 171, 168, 165, 163, 160, 158, 155, 153, 150, 147, 145, 142, 140, 137, 135, 132, 130, 127, 124, 122, 119, 117, 114, 112, 109, 107, 104, 101, 99, 96, 94, 91, 89, 86, 83, 81, 78, 76, 73, 71, 68, 66, 63, 60, 58, 55, 53, 50, 48, 45, 43, 40, 37, 35, 32, 30, 27, 25, 22, 19, 17, 14, 12, 9, 7, 4, 2};
unsigned char senoidal[200] = {127, 131, 135, 139, 143, 147, 151, 155, 159, 163, 167, 170, 174, 178, 181, 185, 189, 192, 196, 199, 202, 205, 209, 212, 215, 217, 220, 223, 226, 228, 231, 233, 235, 237, 239, 241, 243, 244, 246, 247, 249, 250, 251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 250, 249, 247, 246, 245, 243, 241, 239, 237, 235, 233, 231, 228, 226, 223, 220, 218, 215, 212, 209, 206, 202, 199, 196, 192, 189, 185, 182, 178, 174, 171, 167, 163, 159, 155, 151, 147, 143, 139, 135, 131, 127, 123, 119, 115, 111, 107, 103, 99, 95, 92, 88, 84, 80, 76, 73, 69, 66, 62, 59, 55, 52, 49, 46, 43, 40, 37, 34, 31, 29, 26, 24, 21, 19, 17, 15, 13, 11, 10, 8, 7, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 8, 9, 11, 13, 15, 17, 19, 21, 23, 26, 28, 31, 33, 36, 39, 42, 45, 48, 51, 55, 58, 62, 65, 69, 72, 76, 80, 83, 87, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127};

unsigned char * p_ucsr0a = (unsigned char *) 0xC0;
unsigned char * p_ucsr0b = (unsigned char *) 0xC1;
unsigned char * p_ucsr0c = (unsigned char *) 0xC2;
unsigned char * p_ubrr0h = (unsigned char *) 0xC5;
unsigned char * p_ubrr0l = (unsigned char *) 0XC4;
volatile unsigned char * p_udr0 = (unsigned char *) 0xC6;
volatile char msg[90];
volatile contador = 0;//contador da mensagem UART
volatile estado = 0;//indica o estado atual do led
volatile contador_led = 0;//contador para o LED
volatile onda = 0;//indica que está gerando uma forma de onda

void setup(){	
	TCCR2A = 0x83;//Fast PWM Mode - Overflow temporizador 2 = 0xFF
	TCCR2B = 0x01;//Sem prescaling
	OCR2A = 0x00;//valor inicial -- Saída PWM
	TIMSK2 = 0x01;//Flag ativada com o overflow do temporizador
	DDRB = 0x28;//Seta o led interno e a saída 11 como saída
	*p_ucsr0a = *p_ucsr0a & 0xFC;//modo multiprocessador e velocidade dupla desativados
	*p_ucsr0b = 0xD8;//Ativado a recepção e transmissão/ interrupções de recepçao, buffer vazio e transmissão completa
	*p_ucsr0c = 0x0E;//Modo assíncrono/ Sem bits de paridade/ dois bits de parada/ baudrate = 9600 bps/ 8bits por frame
	*p_ubrr0h = *p_ubrr0h & 0xF0;
	*p_ubrr0l = 0x67;//baudrate 9600bps = 0x67 = 103
	asm("sei");//habilita a interrupção global
}

int main(void){
	setup();
    while (1){
    }
}

ISR(TIMER2_OVF_vect){
/*Interrupção gerada com o overflow do temporizador*/
	unsigned int limite_led;
	//Seta o registador OCR2A com o valor de amostragem
	if(comando == 's') {
		OCR2A = senoidal[indice];
		limite_led = 62500;//1 / 16 * 10^-6 
		onda = 1;
	}
	else if(comando == 'a') {
		OCR2A = am[indice];
		limite_led = 7813;//0.125 / 16 * 10^-6 
		onda = 1;
	}
	else if(comando == 'd') {
		OCR2A = dente[indice];
		limite_led = 15625;//0.25 / 16 * 10^-6 
		onda = 1;
	}
	else if(comando == 't') {
		OCR2A = triangular[indice];
		limite_led = 31250;//0.5 / 16 * 10^-6 
		onda = 1;
	}
	else {//caso não esteja gerando nenhuma onda
		onda = 0;
	}
	
	if(indice != 199){//mudança do índice
		indice++;
	}else{
		indice = 0;
	}
	if(contador_led >= limite_led) {//caso em que devemos alterar o estado do LED
		contador_led = 0;
		led();
	}else{
		contador_led++;
	}
}

void led(){
/*Responsável por alterar o estado do led*/
	if(estado == 0){
		estado = 1;
		if(onda == 1) PORTB |= 0x20;
		
	}else{
		estado = 0;
		PORTB &= 0xDF;
	}
}

void transmitir(){
/*Transmite o primeiro caractere e atualiza a primeira mensagem*/
	contador = 1;
	if(comando == 'a'){
		strcpy(msg, "Onda AM\n");		
	}else if(comando == 'd'){
		strcpy(msg, "Onda dente-de-serra\n"); 
	}else if(comando == 't'){
		strcpy(msg, "Onda triangular\n"); 
	}else if(comando =='s'){
		strcpy(msg, "Onda senoidal\n");
	}else{
		strcpy(msg, "Nenhuma forma de onda selecionada\n");
		OCR2A = 0;//faz com que não gere nenhuma onda
	}
		*p_udr0 = msg[0];//transmite o primeiro char
}

ISR(USART_RX_vect){
/*Responsável por receber a forma de onda escrita*/
	comando = *p_udr0;
	transmitir();
}

ISR(USART_TX_vect){
/*Rotina responsável por transmitir os caracteres*/
	if(msg[contador] != '\0'){
		*p_udr0 = msg[contador];
		contador++;
	}else{
		contador = 0;
	}
}
