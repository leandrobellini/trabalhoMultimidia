/*
Fernando Pereira Lisboa 7656241
Leandro Luis Bellini 7572908
*/

/*
    Para comprimir um arquivo "input.txt" em um arquivo "compressed.huf":
    huffman -c input.txt compressed.huf
    Para descomprimir "compressed.huf" em um arquivo "output.txt":
    huffman -d compressed.huf output.txt
*/

#include "huffman.h"

int main(int argc, char *argv[]){

        //nomeDoPrograma [flag compressao/descompressao] [file] [file] ==> 4 argumentos
    if(argc != 4){
        printf("\nUse: \n\thuffman [-c/-d] [file-input] [file-output]\n\n");
        return 1;
    }

    char *opcao = argv[1];
    char *arquivoEntrada = argv[2];
    char *arquivoSaida = argv[3];


        //valida flags de compressao/descompressao
    if(strcmp(opcao,"-c") && strcmp(opcao,"-d")){
        printf("\nUse:");
        printf("\t-c para comprimir\n");
        printf("\t-d para descomprimir\n\n");
        return 1;
    }

        //verifica se os arquivos nao possuiem o mesmo nome...
    if(!strcmp(arquivoEntrada,arquivoSaida)){
        printf("\n\tAtencao, o arquivo de origem \n\tpossui o mesmo nome do arquivo de destino!\n\n");
        return 1;
    }

    if(!strcmp(opcao,"-c")){
        printf("\n\t-> Ativando a compressao do arquivo %s...",arquivoEntrada);
        huffmanComprime(arquivoEntrada,arquivoSaida);
    }else{
        printf("\n\t-> Ativando a descompressao do arquivo %s...",arquivoEntrada);
        huffmanDescomprime(arquivoEntrada,arquivoSaida);
    }


    return 0;

}
