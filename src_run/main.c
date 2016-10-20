#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "wave_reader.c"
#include "diferencas.c"


int main(int argc, char **argv) {
	FILE *f;
	int headerSize = sizeof(wav_hdr);
	int fileLength;
	wav_hdr header;

	if(argc <= 1) {
		printf("Usage: ./wave_reader file.wav\n");
		return EXIT_FAILURE;
	}

	f = fopen(argv[1], "r");
	if(f == NULL) {
		printf("Could not open wave file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	fread(&header, headerSize, 1, f);
	fileLength = getFileSize(f);
 

	printf("File size: %d bytes\n", fileLength);
	printf("Tamanho do cabecalho: %d bytes\n", headerSize);
	printf("ChunkID: %s\n", header.RIFF);
	printf("ChunkSize: %u\n", header.ChunkSize);
	printf("AudioFormat: %d\n", header.AudioFormat);
	printf("NumOfChan: %d\n", header.NumOfChan);
	printf("SamplesPerSec: %u\n", header.SamplesPerSec);
	printf("bytesPerSec: %u\n", header.bytesPerSec);
	printf("bitsPerSample: %hu\n", header.bitsPerSample); 
	printf("Tamanho dos dados: %d\n", header.Subchunk2Size);


	int numeroAmostras = header.Subchunk2Size/2;
	printf("Numero de amostras: %d\n", numeroAmostras);


	//Aqui tenho o arquivo aberto f... vou pular o cabeçalho
	fseek(f, headerSize,SEEK_SET);

	/*unsigned short canalEsquerda[numeroAmostras/2];
	unsigned short canalDireita[numeroAmostras/2];*/


	unsigned short *canalEsquerda;
	unsigned short *canalDireita;

	canalEsquerda  = malloc((numeroAmostras/2)*sizeof(unsigned short));
	canalDireita = malloc((numeroAmostras/2)*sizeof(unsigned short));


	int i,j = 0;

	//Esquerda
	for(i=0;i < numeroAmostras; i+=2){
		fread(&canalEsquerda[j++], sizeof(unsigned short), 1, f);
		fseek(f, sizeof(unsigned short), SEEK_CUR);
	}


	//Direita
	j=0;
	fseek(f, headerSize+sizeof(unsigned short),SEEK_SET);
	for(i=1;i < numeroAmostras; i+=2){
		fread(&canalDireita[j++], sizeof(unsigned short), 1, f);
		fseek(f, sizeof(unsigned short), SEEK_CUR);
	}


	/*short atual; 
	for(i=0; i < numeroAmostras/2; i++){
		printf("\n%u %u ", canalEsquerda[i], canalDireita[i]);
	} 
	exit(0);*/



	comprimeRun(numeroAmostras/2, canalEsquerda, canalDireita, header);

	descomprimeRun();

	fclose(f);

	return EXIT_SUCCESS;
}
