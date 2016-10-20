#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "wave_reader.c"

#define ASCII 256       //Usando amostras de 1 bytes pra codificar! Assim tenho 256 possíveis valores

//estrutura principal usada na àrvore de huffman
typedef struct No{
    unsigned short c;
    int num;
    char codigoBinario[20];
    struct No *esq;
    struct No *dir;
}No;

//protótipos de todas as funções
void huffmanComprime(char *, char *);
void huffmanDescomprime(char *, char *);
void setaCodigoBinario(No *, int *, int *);
void geraCodigos(No *,int *, int *);
void ordenaNos(No **, int );
void pilhaAdd(int *, int *, int );
void pilhaRemove(int *, int *);
void escreveBits(char *, FILE *,char *,int *);
void emOrdem(No *);
No* leByte(No *raiz, No *estado, char c,FILE *);
void leUltimoByte(No *raiz, No *estado,char c,FILE *arqDestino,int ultimo);


//funcao chave para compressao
void huffmanComprime(char *arqEntrada, char *arqSaida){ 
    int i;
    unsigned char sample1;
    wav_hdr cabecalho;
    int numeroAmostras;


            //Manipulação de arquivos...
    FILE *arqDestino;
    FILE *arqOrigem;

    arqOrigem = fopen(arqEntrada,"r");
    arqDestino = fopen(arqSaida,"wb+");


    //Lendo cabeçalho
    fread(&cabecalho, sizeof(wav_hdr), 1, arqOrigem);

    numeroAmostras = (int)cabecalho.Subchunk2Size;  //numero de amostras(Cada amostra 1 byte)
    unsigned char amostras[numeroAmostras];


        //vetor -> tabela geral(A tabela de frequencia...) -> tabela dinâmica
    No **tabela = (No **)malloc(sizeof(No *)*ASCII);

        //alocagem dos nós
    for(i = 0; i < ASCII; i++) // ASCII  --> constante == 128
        tabela[i] = (No *)malloc(sizeof(No));

        //setando tabela --> Inicio toda tabela como zero...
    for(i = 0; i < ASCII; i++){
        tabela[i]->num = 0;
        tabela[i]->c = (unsigned short)i;
        tabela[i]->esq = NULL;
        tabela[i]->dir = NULL;
    }


               //leitura dos caractes
        unsigned char c;

                //leio todos caracteres do arquivo e jah incremento a tabela na posicao c
        for(i=0; i< numeroAmostras;i++){
            fread(&c, sizeof(unsigned char), 1, arqOrigem);
            amostras[i] = c;
            tabela[(int)c]->num++;
        }


            //ASCII 256 caracteres --> Validos guarda a quantidade de caracteres usados pelo arquivo
        int validos = 0;


        printf("\n\t-> Construindo Tabela de frequencia...");

            //contagem dos caracteres válidos...
        for(i = 0; i < ASCII ; i++){
            if(tabela[i]->num != 0){
                validos++;
            }
        }
            //liberação de nós cujos caracteres nao aparecem no arquivo...
        No **novos = (No **)malloc(validos * sizeof(No *));
        int count = 0;

            //copia dos dados para um nova tabela q tem tamanho certo de acordo com os dados lidos(Pode ser menor q 256)
        for(i = 0; i < ASCII; i++){
            if(tabela[i]->num != 0){
                novos[count++] = tabela[i];
            }
        }


        ordenaNos(novos,validos); //ordeno nova tabela

        i = validos;


        printf("\n\t-> Construindo a arvore de huffman...");

            // ***** Criação da Árvore de Huffman *****   -> No final tenho apenas um nó ----> RAIZ!!!!
        while(i != 1){
            int c1 = i-1;
            int c2 = i-2; //maior que c1 (Tabela Ordenada)

            No *aux = (No *)malloc(sizeof(No)); //crio um novo nó que liga as duas folhas...
            aux->dir = novos[c2];
            aux->esq = novos[c1];

            aux->c = 999; //nó de ligação --> Apenas os nós folhas guardam realmente informações
            aux->num = novos[c2]->num + novos[c1]->num;

            novos[c2] = aux;

            i--; //vou cada vez diminuindo minha tabela --> ateh chegar a apenas um nó --> RAIZ da arvore :)

            ordenaNos(novos,i); //Vou restruturando minha tabela...

        }
       
        //*** ainda tenho a referência (tabela) que aponta para meus blocos de forma ordenada...

        No *raizHuffman = *novos; //primeira posicao novos[0]


                //Uso uma pilha para gerar os códigos binários --> Percorro a arvore(Esq add 0 -- Dir add 1)
        int countPilha = 0;
        int pilha[100];

                //inicialização da pilha...
        for(i = 0; i < 100; i++)
            pilha[i] = 0;

                //percorre toda àrvore de huffman setando a sequencia binária de cada caracter...
        
        geraCodigos(raizHuffman,pilha,&countPilha);


        //Cabeçalho do WAV
        fwrite(&cabecalho,sizeof(wav_hdr), 1,arqDestino);

        //Gravando cabeçalho do arquivo --> Guardo a tabela de frequencia para depois montar a árvore...

        /**
                                        DEFINIÇÃO DO CABEÇALHO

            Primeiro Byte N do arquivo compactado   -> tamanho da minha tabela de frequência
            Próximos N*sizeof(No)                   -> tabela de frequência
            Ultimo Byte do arquivo                  -> Armazena se o ultimo byte gravado no arquivo está completo
        */

            //gravando primeiro Byte do cabeçalho -> tamanho da minha tabela(Soh guardo caracteres usados)
        fwrite(&validos,sizeof(int),1,arqDestino); 

            //gravando tabela
            //crio uma tabela temporiaria e ordenada(Apenas com os caracteres válidos)
                //pois tenho q manter a tabela original para acessar depois as possicos c diretamente
        No **tabelaTemp = (No **)malloc(validos * sizeof(No *));

            int j  = 0;

        for(i = 0; i < ASCII; i++)
            if(tabela[i]->num != 0)
                tabelaTemp[j++] = tabela[i];

        ordenaNos(tabelaTemp,validos);


        for(i = 0; i < validos; i++)
                fwrite(tabelaTemp[i],1,sizeof(No),arqDestino);


        int contador = 0; //usado para definir se jah usei um byte ou ainda á bits no byte atual disponivel...
                                                    //Ao completar o byte(Todos 8 bits usados), gravo no arquivo...

        char ch = 0;   //Esse byte eh tratado como um vetor... vejo se tenho espaço para add bits até completar os 8 bits e gravar no arquivo...

        printf("\n\t-> Comprimindo arquivo...");

                //Vou lendo cada caractere do arquivo de origem e jah salvo seus bits relacionados no arquivo de saida...

        /*
        while( !feof(arqOrigem)){

            fread(&sample1, sizeof(unsigned char), 1, arqOrigem);

            if(c == EOF)
                break;
            escreveBits(tabela[c]->codigoBinario,arqDestino,&ch,&contador); //função responsável por gravar a sequencia de bits

        }*/

        for(i = 0; i < numeroAmostras; i++){
            c = amostras[i];
            escreveBits(tabela[c]->codigoBinario,arqDestino,&ch,&contador);
        }

        char contadorChar;

        if(contador == 0){
            contadorChar = 0;
            fwrite(&contadorChar,1,sizeof(char),arqDestino); //ultimo byte foi completo, apenas gravo 0 no final do arquivo para indicar isto
        }else{
            fwrite(&ch,1,sizeof(char),arqDestino);
            contadorChar = contador;

            //gravo quanto usei do arquivo...
            fwrite(&contadorChar,1,sizeof(char),arqDestino);
        }

                //arquivo compactado -> encerro função de compactação...
        fclose(arqDestino);
        fclose(arqOrigem);

        printf("\n\t-> Arquivo comprimido com sucesso!\n\n");
}

//função chave da descompressao do arquivo
void huffmanDescomprime(char *arqEntrada, char *arqSaida){
    
    /*                                   DEFINIÇÃO DO CABEÇALHO

            Primeiro Byte N do arquivo compactado   -> tamanho da minha tabela de frequência
            Próximos N*sizeof(No)                   -> tabela de frequência
            Ultimo Byte do arquivo                  -> Armazena se o ultimo byte gravado no arquivo está completo
    */
    
    int i;

    FILE *arqDestino;
    FILE *arqOrigem;

    arqOrigem = fopen(arqEntrada,"r");

    arqDestino = fopen(arqSaida,"w");

    if(arqDestino == NULL){
        printf("\nImpossivel abrir o arquivo de destino... \n");
        return;
    }

    printf("\n\t-> Reconstruindo Tabela de frequencia apartir do arquivo...");

    //Cabeçalho do wav
    wav_hdr header;
    int tamanhoTabela = 0;

    fread(&header, sizeof(wav_hdr), 1, arqOrigem);
    fread(&tamanhoTabela, sizeof(int), 1, arqOrigem);

    fwrite(&header, sizeof(wav_hdr), 1, arqDestino);

            //montando tabela de frequencia e lendo do arquivo....
    No **tabela = (No **)malloc(sizeof(No *)*tamanhoTabela);
        for(i = 0;i < tamanhoTabela; i++ ){
            tabela[i] = (No *)malloc(sizeof(No));
            fread(tabela[i],1,sizeof(No),arqOrigem);
        }


    //criação da árvore de huffman...
    No **tabelaTemporaria = (No **)malloc(tamanhoTabela * sizeof(No *));

    for(i = 0; i < tamanhoTabela; i++){
        tabelaTemporaria[i] = tabela[i];
    }

    i = tamanhoTabela;

    printf("\n\t-> Reconstruindo a Arvore de huffman apartir do arquivo...");

            // ***** Criação da Árvore de Huffman *****   -> No final tenho apenas um nó ----> RAIZ!!!!
    while(i != 1){
            int c1 = i-1;
            int c2 = i-2; //maior que c1 (Tabela Ordenada)

            No *aux = (No *)malloc(sizeof(No)); //crio um novo nó que liga as duas folhas...
            aux->dir = tabelaTemporaria[c2];
            aux->esq = tabelaTemporaria[c1];

            aux->c = 999; //nó de ligação --> Apenas os nós folhas guardam realmente informações
            aux->num = tabelaTemporaria[c2]->num + tabelaTemporaria[c1]->num;

            tabelaTemporaria[c2] = aux;

            i--; //vou cada vez diminuindo minha tabela --> ateh chegar a apenas um nó --> RAIZ da arvore :)

            ordenaNos(tabelaTemporaria,i); //Vou restruturando minha tabela...

    }

    No *raizHuffman = *tabelaTemporaria;
    
    //leitura dos caracteres do arquivo... -> leilendo...lllo sempre dois caracteres em conjunto:
        //isso eh necessário pois o ultimo byte antes do final do arquivo guarda se o ultimo caractere usa todo o byte final...
    char c,prox,prox2;
    int error = 0;

    fread(&prox,1,sizeof(char),arqOrigem);
    fread(&prox2,1,sizeof(char),arqOrigem);

    No *estado = NULL; //armazena onde eu parei na àrvore...
    int voltas = 0;

    printf("\n\t-> Descomprimindo arquivo...");

    while(1){ //sempre leio um caracter a frente...
            //pois o ultimo byte do arquivo contem a informacao de o penultimo byte estah completo
        c = prox;
        prox = prox2;

        error = fread(&prox2,1,sizeof(char),arqOrigem);

        if(feof(arqOrigem) || error == 0){
            //cheguei no ultimo byte...

            if(voltas == 0){ //condição especial: há apenas 1 caracter no meu arquivo
                estado = raizHuffman;
            }
            leUltimoByte(raizHuffman,estado,c,arqDestino,prox);

            break;
        }else{
            voltas = 1;
            estado = leByte(raizHuffman,estado,c,arqDestino);
        }


    }

    printf("\n\t-> Arquivo descomprimido com sucesso!\n");

    fclose(arqOrigem);
    fclose(arqDestino);
}

//para cada caracter, verifica na pilha de bits qual a sequencia atual
void setaCodigoBinario(No *raiz, int *pilha, int *countPilha){
    int i;
    for(i = 0; i < *countPilha; i++){
        raiz->codigoBinario[i] = pilha[i] + 48; // 48 = 0; 49 = 1  ==> tabela ASCII
    }
    raiz->codigoBinario[*countPilha] = '\0';
}

//gera a sequencia de bits para cada caracter da àrvore de huffman
void geraCodigos(No *raiz,int *pilha, int *countPilha){
    if(raiz->c == 999){
        pilhaAdd(pilha,countPilha,0);
        geraCodigos(raiz->esq,pilha,countPilha);
        pilhaRemove(pilha,countPilha);
        pilhaAdd(pilha,countPilha,1);
        geraCodigos(raiz->dir,pilha,countPilha);
        pilhaRemove(pilha,countPilha);
    }else{
        setaCodigoBinario(raiz,pilha,countPilha);
    }
}

//ordena a tabela de frequencia de acordo com os caracteres mais frequentes no arquivo
void ordenaNos(No **tabela, int validos){
    int j,k;
    for(j = 0; j < validos; j++)
                for(k = 0; k < validos; k++){
                    No *aux;
                    if( tabela[j]->num  > tabela[k]->num){
                        aux = tabela[j];
                        tabela[j] = tabela[k];
                        tabela[k] = aux;
                    }
                }
}

//adiciona elemento na pilha de bits
void pilhaAdd(int *pilha, int *countPilha, int value){
    pilha[*countPilha] = value;
    (*countPilha)++;
}

//remove um elemento da pilha de bits(Usado para montar a sequencia de bits de cada caracter)
void pilhaRemove(int *pilha, int *countPilha){
    pilha[*countPilha] = 0;
    (*countPilha)--;
}

//função responsável por escrever a sequencia de bits no arquivo...
void escreveBits(char *sequencia, FILE *arquivo,char *c,int *count){
    //se count atingir 8 bits, gravo no arquivo e começo um novo
    int tam = strlen(sequencia);
    int i,j;

    char zero = 254; //11111110 -> colocar 0 no final -> faço um AND
    char um = 1;   //00000001 -> colocar 1 no final -> faço um OU

    for(i = 0; i < tam; i++){
        //para cada bit da sequencia...


        //desloco sequencia -> ganho a ultima posicao para colocar meu bit (0 ou 1)
        *c = *c << 1;

        if(sequencia[i] == '1'){
            //colocar 1 no final -> faço um OU (00000001)

            *c = *c | um;

        }else{
            //and com zero(11111110)

            *c = *c & zero;
        }


        (*count)++; //mais um bit salvo :-)

            //bit gravado... vejo se chegou a 8 caracteres

            if(*count == 8){
                *count  = 0; //começo um novo bit

                fwrite(c,1,sizeof(char),arquivo); //gravo bit...

                *c = 0; //00000000
            }

    }

}

//percorre a arvore de huffman fazendo o percurso em ordem
void emOrdem(No *raiz){
    if(raiz == NULL)
        return;
    emOrdem(raiz->esq);
    if(raiz->c != 999)
        printf("\n[%u] => %s",raiz->c,raiz->codigoBinario);
    emOrdem(raiz->dir);
}

//leitura dos bytes do arquivo compactado
No* leByte(No *raiz, No *estado, char c,FILE *arqDestino){
   int i;
   No *p;
   char temp;
   int count = 0;
   char um = 1;   //00000001 -> verifico de há 1 no final...

   if(estado == NULL){
        p = raiz; //começo da raiz
   }else{
        p = estado; //continuo de onde parei...
   }


   for(i = 0; i < 8; i++){
        temp = c;

        temp = temp >> (7 - count);  //desloco ateh o final e verifico fazendo um AND com 1

        count++;

        temp = temp & um;

        if(temp == 1){ //bit = 1
            p = p->dir;
        }else{
            p = p->esq; //bit = 0
        }

        if(p->c != 999){
            //encontrei um nó folha...
            fputc(p->c,arqDestino);
            p = raiz; //recomeço do inicio da árvore...
        }

   }

   return p; //retorno de onde eu parei... pode ter parado em um nó não-folha,
                        //assim a sequencia de bits pode continuar em outro byte, logo tenho q ler um novo byte


}

//mesma função do leByte mas só eh usada pelo ultimo byte, que pode estar completo ou nao! --tem um for especial!
void leUltimoByte(No *raiz, No *estado, char c,FILE *arqDestino,int ultimo){
    No *p = estado;
    int i;
    char temp;
    char um = 1;

       for(i = 0; i < ultimo; i++){
            temp = c;

            temp = temp >> (--ultimo);

            //count++;

            temp = temp & um;

            if(temp == 1){
                p = p->dir;
            }else{
                p = p->esq;
            }
            if(p->c != 999){
                //encontrei um nó folha...
                fprintf(arqDestino,"%c",p->c);
                p = raiz; //recomeço do inicio da árvore...
            }
       }


}

#endif

























