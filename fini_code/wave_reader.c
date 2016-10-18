#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// *** References *** 
// http://soundfile.sapp.org/doc/WaveFormat/
// http://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file

typedef struct  WAV_HEADER{
    char                RIFF[4];        // RIFF Header      Magic header
    unsigned int        ChunkSize;      // RIFF Chunk Size  
    char                WAVE[4];        // WAVE Header      
    char                fmt[4];         // FMT header       
    unsigned int        Subchunk1Size;  // Size of the fmt chunk                                
    unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM 
    unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio                   
    unsigned int        SamplesPerSec;  // Sampling Frequency in Hz                             
    unsigned int        bytesPerSec;    // bytes per second 
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo 
    unsigned short      bitsPerSample;  // Number of bits per sample      
    char                Subchunk2ID[4]; // "data"  string   
    unsigned int        Subchunk2Size;  // Sampled data length    

} wav_hdr; 

// find the file size 
int getFileSize(FILE *inFile){
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}


int main(int argc, char **argv) {
	FILE *f;
	int i=0;
	int headerSize = sizeof(wav_hdr);
	int fileLength;
	wav_hdr header;
	uint16_t *sampleNumBytes;

	/*
		if, for some reason, you are having problems to read WAVE fields, 
		check whether the sizes of the following types are in accordance with 
		the WAVE header specification:

	printf("char size: %lu\n", sizeof(char));	
	printf("unsigned long size: %lu\n", sizeof(unsigned long));
	printf("unsigned short: %lu\n", sizeof(unsigned short));
	printf("unsigned int: %lu\n", sizeof(unsigned int));

	*/



	f = fopen("sample1.wav", "r");
	if(f == NULL) {
		printf("Could not open wave file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	fread(&header, headerSize, 1, f);
	fileLength = getFileSize(f);
	
	printf("File size: %d bytes\n", fileLength);
	printf("ChunkID: %s\n", header.RIFF);
	printf("ChunkSize: %u\n", header.ChunkSize);
	printf("AudioFormat: %d\n", header.AudioFormat);
	printf("NumOfChan: %d\n", header.NumOfChan);
	printf("SamplesPerSec: %u\n", header.SamplesPerSec);
	printf("bytesPerSec: %u\n", header.bytesPerSec);
	printf("bitsPerSample: %hu\n", header.bitsPerSample);
	printf("Subchunk2Size: %u bytes\n", header.Subchunk2Size);


	sampleNumBytes = (uint16_t*)malloc(sizeof(header.Subchunk2Size));

	while (!feof(f)){
		//printf("etoru\n");
		fread(&sampleNumBytes[i],sizeof(uint16_t),1,f);
		printf("\nSanple[%d] = %u",i,sampleNumBytes[i]);
		i++;
	}

	int max = 1;	//printf("%d\n", i);

	for(i = 0; i <= max; i++)
	{
	//	printf("\nSanple[%d] = %u",i,sampleNumBytes[i]);
	}

	//Fechar arquivo
	fclose(f);
	
	return EXIT_SUCCESS;
}
