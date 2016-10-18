#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "wave_reader.c"


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


	int numeroAmostras = ((fileLength- headerSize)/(header.bitsPerSample/8));
	printf("Numero de amostras: %d\n", numeroAmostras);



	//Aqui tenho o arquivo aberto f... vou pular o cabeçalho
	fseek(f, headerSize,SEEK_SET);
	int i;

	char nomeSaida[256];
	strcpy(nomeSaida, "saida.b");

	char novoArq[256];
	strcpy(novoArq, "novo.wav");

	fseek(f, headerSize,SEEK_SET);
	rewind(f);

	/*huffmanComprime(f, nomeSaida, &header, numeroAmostras);

	return 0;

	huffmanDescomprime(nomeSaida, novoArq);*/

	fclose(f);

	return EXIT_SUCCESS;
}
