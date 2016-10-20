#ifndef _DIFERENCAS_H_
#define _DIFERENCAS_H_

#include "wave_reader.c"

void codificacaoDiferencas(char *arqEntrada, char *arqSaida){ 
	printf("\n\t-> Codificacao por Diferencas...");  

	FILE *f, *arqOrigem;
	wav_hdr header;
	int numeroAmostras;

		//variaveis
	short diferenca;
	int i, j=0;


	arqOrigem = fopen(arqEntrada,"r");
	if(arqOrigem == NULL){
        printf("\nImpossivel abrir o arquivo de origem... \n");
        return;
    }

    fread(&header, sizeof(wav_hdr), 1, arqOrigem);
    numeroAmostras = header.Subchunk2Size/4;

    unsigned short canalEsquerda[numeroAmostras];
	unsigned short canalDireita[numeroAmostras]; 

	//Esquerda
	for(i=0;i < (numeroAmostras*2); i+=2){
		fread(&canalEsquerda[j++], sizeof(unsigned short), 1, arqOrigem);
		fseek(arqOrigem, sizeof(unsigned short), SEEK_CUR);
	}


	//Direita
	j=0;
	fseek(arqOrigem, sizeof(wav_hdr)+sizeof(unsigned short),SEEK_SET);
	for(i=1;i < (numeroAmostras*2); i+=2){
		fread(&canalDireita[j++], sizeof(unsigned short), 1, arqOrigem);
		fseek(arqOrigem, sizeof(unsigned short), SEEK_CUR);
	}


	//Salvando valores!!
	f = fopen(arqSaida, "wb+");
	if(f == NULL){
        printf("\nImpossivel abrir o arquivo de destino... \n");
        return;
    }

	//gravo cabeçalho
	fwrite(&header,sizeof(wav_hdr), 1,f); 

	//gravo primeira amostra canal esquerda
	fwrite(&canalEsquerda[0],sizeof(unsigned short), 1,f);

	for(i=1; i < numeroAmostras; i++){
		diferenca = canalEsquerda[i] - canalEsquerda[i-1];
		fwrite(&diferenca,sizeof(short), 1,f);
	}

	//gravo primeira amostra canal direita
	fwrite(&canalDireita[0],sizeof(unsigned short), 1,f);


	for(i=1; i < numeroAmostras; i++){
		diferenca = canalDireita[i] - canalDireita[i-1];
		fwrite(&diferenca,sizeof(short), 1,f);
	}

	printf("\n\t-> Arquivo Comprimido com sucesso!\n");

	fclose(f);
	fclose(arqOrigem);

}


void decodificaoDiferencas(char *arqEntrada, char *arqSaida){
	FILE *f;
	FILE *f_saida;

	int i, j;
	wav_hdr header;
	short atual;
	unsigned short sample;


	f = fopen(arqEntrada, "rb");
	f_saida = fopen(arqSaida, "w+");

	fread(&header, sizeof(wav_hdr), 1, f);
	fwrite(&header, sizeof(wav_hdr),1 , f_saida);

	int numeroAmostras = header.Subchunk2Size/4;

	unsigned short canalEsquerda[numeroAmostras];
	unsigned short canalDireita[numeroAmostras];


	//Para o canal da esquerda
		//le primeira amostra
	fread(&sample, sizeof(unsigned short),1, f);
	canalEsquerda[0] = sample;

	for (int i = 1; i < numeroAmostras; i++)
	{
		fread(&atual, sizeof(short), 1,f);
		sample = sample + atual;
		canalEsquerda[i] = (unsigned short)sample;
	}

	//Para o canal da direita
		//le primeira amostra
	fread(&sample, sizeof(unsigned short), 1,f);
	canalDireita[0] = sample;

	for (int i = 1; i < numeroAmostras; i++)
	{
		fread(&atual, sizeof(short), 1,f);
		sample = sample + atual;
		canalDireita[i] = (unsigned short)sample;
	}


	for (int i = 0; i < numeroAmostras; i++)
	{
		fwrite(&canalEsquerda[i], sizeof(unsigned short), 1, f_saida);
		fwrite(&canalDireita[i], sizeof(unsigned short), 1, f_saida);
	}
	

	fclose(f);
	fclose(f_saida);

}



#endif