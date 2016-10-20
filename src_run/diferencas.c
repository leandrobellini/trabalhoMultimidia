#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "wave_reader.c"



void comprimeRun(int numeroAmostras,unsigned short *canalEsquerda, unsigned short *canalDireita, wav_hdr wave_reader){
	int i;

	FILE *saida;

	saida = fopen("saida_run.b", "wb+");


	//cabecalho
	fwrite(&wave_reader,sizeof(wav_hdr), 1, saida);



	//canal da esquerda
	unsigned short anterior, atual;
	unsigned char frequencia=0;

	anterior = canalEsquerda[0];
	for(i = 1; i < numeroAmostras; i++){
		if(canalEsquerda[i] != anterior){
			if(frequencia == 0){
				//achei um cara soh
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else if(frequencia == 1){
				//achei dois caras
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else{
				//achei 3 caras ou mais
				frequencia++;
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&frequencia, sizeof(unsigned char), 1, saida);
			}

			//printf("%u x%d\n", anterior, frequencia);
			anterior = canalEsquerda[i];
			frequencia = 0;
			
		}else{
			frequencia++; //achei um cara igual

		}

	}


	anterior = canalDireita[0];
	for(i = 1; i < numeroAmostras; i++){
		if(canalDireita[i] != anterior){
			if(frequencia == 0){
				//achei um cara soh
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else if(frequencia == 1){
				//achei dois caras
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else{
				//achei 3 caras ou mais
				frequencia++;
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&frequencia, sizeof(unsigned char), 1,saida);
			}

			//printf("%u x%d\n", anterior, frequencia);
			anterior = canalEsquerda[i];
			frequencia = 0;
			
		}else{
			frequencia++; //achei um cara igual

		}

	}


	fclose(saida);
	
}




void descomprimeRun(){
	int i, j;

	FILE *saida;
	FILE *entrada;

	entrada = fopen("saida_run.b", "rb+");
	saida = fopen("saida_final.wav", "w+");


	wav_hdr header;
	fread(&header, sizeof(wav_hdr),1,entrada);


	int numeroAmostras = header.Subchunk2Size/4;
	printf("Numero de amostras: %d\n", numeroAmostras);


	unsigned short canalEsquerda[numeroAmostras];
	unsigned short canalDireita[numeroAmostras];



	//canal da esquerda
	unsigned short anterior, atual;
	unsigned char frequencia=0;

	fread(&anterior, sizeof(unsigned short), 1, entrada);


	for(i = 1; i < numeroAmostras; i++){
		fread(&atual, sizeof(unsigned short), 1, entrada);
		if(atual != anterior){
			if(frequencia == 0){
				//achei um cara soh
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else if(frequencia == 1){
				//achei dois caras
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else{
				//achei 3 caras ou mais
				fread(&frequencia,sizeof(unsigned char),1, entrada);
				for(j = 0; j < frequencia; j++){
					fwrite(&anterior, sizeof(unsigned short), 1, saida);
				}
			}

			//printf("%u x%d\n", anterior, frequencia);
			anterior = atual;
			frequencia = 0;
			
		}else{
			frequencia++; //achei um cara igual

		}

	}


	//direita

	fread(&anterior, sizeof(unsigned short), 1, entrada);


	for(i = 1; i < numeroAmostras; i++){
		fread(&atual, sizeof(unsigned short), 1, entrada);
		if(atual != anterior){
			if(frequencia == 0){
				//achei um cara soh
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else if(frequencia == 1){
				//achei dois caras
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
				fwrite(&anterior, sizeof(unsigned short), 1, saida);
			}else{
				//achei 3 caras ou mais
				fread(&frequencia,sizeof(unsigned char),1, entrada);
				for(j = 0; j < frequencia; j++){
					fwrite(&anterior, sizeof(unsigned short), 1, saida);
				}
			}

			//printf("%u x%d\n", anterior, frequencia);
			anterior = atual;
			frequencia = 0;
			
		}else{
			frequencia++; //achei um cara igual

		}

	}

	fclose(saida);
	
}


