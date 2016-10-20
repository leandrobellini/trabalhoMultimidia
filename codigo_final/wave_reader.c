
#ifndef _WAVE_READER_H_
#define _WAVE_READER_H_

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


#endif