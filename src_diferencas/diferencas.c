#ifndef _DIFERENCAS_H_
#define _DIFERENCAS_H_

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "wave_reader.c"

void codificacaoDiferencas(wav_hdr header,unsigned short *canalEsquerda,unsigned short *canalDireita, int numeroAmostras){
	printf("\n\nDiferencas...\n");

	FILE *f;

		//variaveis
	short diferenca;
	int i, j;


	f = fopen("saida_diferencas.b", "wb+");

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

	fclose(f);

}


void decodificaoDiferencas(int numeroAmostras){
	FILE *f;
	FILE *f_saida;


	int i, j;
	wav_hdr header;
	short atual;
	unsigned short sample;
	unsigned short canalEsquerda[numeroAmostras];
	unsigned short canalDireita[numeroAmostras];


	f = fopen("saida_diferencas.b", "rb");
	f_saida = fopen("saida_final.wav", "w+");

	fread(&header, sizeof(wav_hdr), 1, f);
	fwrite(&header, sizeof(wav_hdr),1 , f_saida);


	//Para o canal da esquerda
		//le primeira amostra
	fread(&sample, sizeof(unsigned short),1, f);
	canalEsquerda[0] = sample;

	for (int i = 1; i < numeroAmostras; ++i)
	{
		fread(&atual, sizeof(short), 1,f);
		sample = sample + atual;
		canalEsquerda[i] = (unsigned short)sample;
	}

	//Para o canal da direita
		//le primeira amostra
	fread(&sample, sizeof(unsigned short), 1,f);
	canalDireita[0] = sample;

	for (int i = 1; i < numeroAmostras; ++i)
	{
		fread(&atual, sizeof(short), 1,f);
		sample = sample + atual;
		canalDireita[i] = (unsigned short)sample;
	}


	for (int i = 0; i < numeroAmostras; ++i)
	{
		fwrite(&canalEsquerda[i], sizeof(unsigned short), 1, f_saida);
		fwrite(&canalDireita[i], sizeof(unsigned short), 1, f_saida);
	}
	

	fclose(f);
	fclose(f_saida);

}



#endif