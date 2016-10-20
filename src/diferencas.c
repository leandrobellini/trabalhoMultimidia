#ifndef _DIFERENCAS_H_
#define _DIFERENCAS_H_

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "wave_reader.c"

void codificacaoDiferencas(wav_hdr header,short *canalEsquerda,short *canalDireita, int numeroAmostras){
	printf("\n\nDiferencas...\n");

	FILE *f;

		//variaveis
	signed char diferenca;
	int i, j;


	f = fopen("saida_diferencas.b", "wb+");

	//gravo cabeçalho
	fwrite(&header,sizeof(wav_hdr), 1,f);

	//gravo primeira amostra canal esquerda
	fwrite(&canalEsquerda[0],sizeof(short), 1,f);

	for(i=1; i < numeroAmostras; i++){
		diferenca = canalEsquerda[i] - canalEsquerda[i-1];
		fwrite(&diferenca,sizeof(signed char), 1,f);
	}

	//gravo primeira amostra canal direita
	fwrite(&canalDireita[0],sizeof(short), 1,f);


	for(i=1; i < numeroAmostras; i++){
		diferenca = canalDireita[i] - canalDireita[i-1];
		fwrite(&diferenca,sizeof(signed char), 1,f);
	}




	fclose(f);

}

void decodificaoDiferencas(int numeroAmostras){
	FILE *f;
	FILE *f_saida;


	int i, j;
	wav_hdr header;
	signed char atual;
	short sample;
	short canalEsquerda[numeroAmostras];
	short canalDireita[numeroAmostras];


	f = fopen("saida_diferencas.b", "rb");
	f_saida = fopen("saida_final.wav", "w+");

	fread(&header, sizeof(wav_hdr), 1, f);
	fwrite(&header, sizeof(wav_hdr),1 , f_saida);


	//Para o canal da esquerda
		//le primeira amostra
	fread(&sample, sizeof(short),1, f);
	canalEsquerda[0] = sample;

	for (int i = 1; i < numeroAmostras; ++i)
	{
		fread(&atual, sizeof(signed char), 1,f);
		sample = sample + (short)atual;
		canalEsquerda[i] = sample;
	}

	//Para o canal da direita
		//le primeira amostra
	fread(&sample, sizeof(short), 1,f);
	canalDireita[0] = sample;

	for (int i = 1; i < numeroAmostras; ++i)
	{
		fread(&atual, sizeof(signed char), 1,f);
		sample = sample + (short)atual;
		canalDireita[i] = sample;
	}


	for (int i = 0; i < numeroAmostras; ++i)
	{
		fwrite(&canalEsquerda[i], sizeof(short), 1, f_saida);
		fwrite(&canalDireita[i], sizeof(short), 1, f_saida);
	}
	

	fclose(f);
	fclose(f_saida);

}



#endif