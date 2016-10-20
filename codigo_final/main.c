
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "wave_reader.c"
#include "huffman.c"
#include "diferencas.c"



int main(int argc, char **argv) {
	system("clear");

	printf("\n\t********************************************\n");
	printf("\t    Bem vindo. Use uma das opcoes abaixo:\n");
	printf("Codificacao Huffman: ./main -ch arquivoEntrada.wav arquivoSaida.b \n");
	printf("Codificacao por Diferencas: ./main -cd arquivoEntrada.wav arquivoSaida.b\n");
	printf("Codificacao Run-Length: ./main -cr arquivoEntrada.wav arquivoSaida.b\n");
	printf(" \n");
	printf("Decodificacao: ./main -d arquivoEntrada.b arquivoSaida.wav\n");
	printf(" \n");


	if(argc <= 3) {
		printf("Você nao entrou com uma das opcoes acima!!!\n");
		return EXIT_FAILURE;
	}

	//argv[1]  -> opcao
	//argv[2]  -> arquivo de entrada
	//argv[3]  -> arquivo de saida

	if(strcmp(argv[1],"-ch") == 0){
		huffmanComprime(argv[2], argv[3]);

	}else if(strcmp(argv[1],"-cd") == 0){
		codificacaoDiferencas(argv[2], argv[3]);

	}else if(strcmp(argv[1],"-cr") == 0){
		printf("Run-Length\n");

	}else if(strcmp(argv[1],"-d") == 0){
		printf("Decodificacao\n");
		decodificaoDiferencas(argv[2], argv[3]);

	}else{
		printf("Opcao invalida!!!!\n");
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
