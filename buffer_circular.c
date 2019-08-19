/* Teste de um buffer circular
 * Thiago Soares Laitz - 224898
 *
 * Este programa serve para implementar e testar uma implementação de buffer
 * circular
 */

#include <stdio.h>

char buffer[5];
int i = 0;//guarda a posição livre do buffer
int j = 5;//Posiçao do elemento mais antigo nao processado -- inicializado com o valor 5
int vazio = 0;//indica que o buffer esta vazio
int cheio = 0;//indica que o buffer esta cheio

void adicionar_buffer(char c) {
/* Adiciona um item ao buffer se ele não estiver cheio */
	if(cheio == 0){
		if(i == 4){
			vazio = 0;
			buffer[i] = c;//devemos salvar o char c antes de incrementar i
			i = 0;
		}else{
			vazio = 0;
			if(j == 5) j = i;//salva o primeiro valor de j
			buffer[i] = c;
			i++;
		}
	}
	if(i == j) cheio = 1;//caso o índice i alcance o índice j
}

void remover_buffer() {
/* Remove um item do buffer no caso de não estar vazio*/
	cheio = 0;
	if(j != 5 & vazio == 0){
		if(j+1 == i | (j == 4 & i == 0)){//caso que o buffer se torna vazio
			vazio = 1;
			if(j != 4) j++;
			if(j == 4) j = 0;
		}else{ //caso que só é removido um item
			if(j == 4) j = 0;
			if(j != 4) j++;
		}
	}
}	

void imprimir_buffer() {
/* Função responsável por printar o buffer o circular */
	int aux_j = j;
	if(j != 5){
		do{ 
			if(vazio == 0){
				printf("%c", buffer[aux_j]);
				if(aux_j == 4){
					aux_j = 0;
				}else{
					aux_j++;
					}
				if(aux_j != i) printf(" ");//coloca um espaço entre cada valor printado com exceção do último
			}
		}while(i != aux_j);		
	}
	printf("\n");
}

int main() {

	char c;
	do {
	scanf("%c", &c);
	if (c == '\n') break;
		if(c >= 48 & c <= 57){//c é um numero entre 0 e 9 -- ASCII
			adicionar_buffer(c);
			imprimir_buffer();
		}
		if((c >= 65 & c <= 90) | (c >= 97 & c <= 122)){ //c é uma letra do alfabeto -- ASCII
			remover_buffer();
			imprimir_buffer();
		}
	} while (1);

	return 0;
}
