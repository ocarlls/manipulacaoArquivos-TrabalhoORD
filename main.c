#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX_REC_SIZE = 200;

int get_rec(FILE* arq, char* buffer, int size){

    short rec_lgth;

    if (fread(&rec_lgth, sizeof(rec_lgth), 1, arq) == 0) 
       return 0;


    if (rec_lgth < size) {	      
    rec_lgth = fread(buffer, sizeof(char), rec_lgth, arq);
      buffer[rec_lgth] = '\0';
      return rec_lgth;	
    }
    else {	  	
      fprintf(stderr, "Buffer overflow\n");
      return 0;
    }	  
}

void buscaRegistro(char chaveRegistro[]){
  fprintf(stdout, "Busca pelo registro de chave \"%s\"\n", chaveRegistro);
  FILE *arq;
  arq = fopen("dados.dat", "rb");
  if(arq == NULL){
    fprintf(stderr, "Erro: Arquivo de leitura não encontrado!\n");
    return;
  }

  char buffer[MAX_REC_SIZE];
  int rec_lgth = get_rec(arq, buffer, MAX_REC_SIZE);
  int found=0;
	
  while (rec_lgth > 0) {

		//printf("Registro %d\n", ++rec_count);
		int fld_count = 0;
		
    
    char recId[10];
    for(int i = 0; buffer[i] != '|'; i++){
      recId[i] = buffer[i]; 
      //buffer[rec_lgth-1] = '\0';
    }
    
    if(strcmp(recId, chaveRegistro) == 0){
      printf("%s (%d bytes)\n", buffer, rec_lgth);
      found=1;
      break;
    }
		
		//printf("\n");
		rec_lgth = get_rec(arq, buffer, MAX_REC_SIZE);
  }
  if(!found){
    printf("Registro não encontrado.\n");
  }

  fclose(arq);
}

void insereRegistro(char registroInserir[]){
}

void removeRegistro(char idRemover[]){
}

void executarOperacoes(char nomeArquivoOps[]){
  char ops[MAX_REC_SIZE+2];
  FILE* arqOps;
  arqOps = fopen(nomeArquivoOps, "r");
  while(fgets(ops, sizeof(ops), arqOps)){
    ops[strlen(ops)-1] = '\0'; // removendo \n do final;
    char opType[5];
    char opInput[MAX_REC_SIZE];
    strncpy(opType, ops+0, 1);
    strncpy(opInput, ops+2, strlen(ops));

    if(strcmp(opType, "b") == 0)
      buscaRegistro(opInput);
    
    else if(strcmp(opType, "i") == 0)
      insereRegistro(opInput);

    else if(strcmp(opType, "r") == 0)
      removeRegistro(opInput);
    else
      printf("A operação \"%s\" é inválida.", opType);

    printf("\n");
    

    
  }
}


void escreveRegistro(FILE *arqEscrita, char strEntrada[], short tamanhoStr){
    if(strlen(strEntrada)==0){
        return;
    }
    fwrite(&tamanhoStr, sizeof(short), 1, arqEscrita);
    fputs(strEntrada, arqEscrita);
}
void importar(char nomeArquivo[]){
    FILE *arq;
    arq = fopen(nomeArquivo, "r");
    if(arq == NULL){
        exit(EXIT_FAILURE);
    }
    FILE *arqCriar;
    arqCriar = fopen("dados.dat", "wb");

    char gameLido[MAX_REC_SIZE]; // COLOQUEI MAX_REC_SIZE
    char str[MAX_REC_SIZE];
    while(fgets(str, sizeof(str), arq)){
        str[strlen(str)-1] = '\0'; //REMOVE O \n DO FINAL DA STRING LIDA
        short tamanhoStr = strlen(str);
        //printf("%d\n -- %s\n", tamanhoStr, str);
        escreveRegistro(arqCriar, str, tamanhoStr);
    }
    printf("Importacao do arquivo realizada com sucesso!!\n");
}

int main(int argc, char *argv[]) {

    if (argc == 3 && strcmp(argv[1], "-i") == 0) {

        printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);
        importar(argv[2]);

    } else if (argc == 3 && strcmp(argv[1], "-e") == 0) {

        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        executarOperacoes(argv[2]);
        //buscaRegistro(argv[2]);

    } else if (argc == 2 && strcmp(argv[1], "-p") == 0) {

        printf("Modo de impressao da LED ativado ...\n");
        // imprimir_led();

    } else {

        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        fprintf(stderr, "$ %s -p\n", argv[0]);
        exit(EXIT_FAILURE);

    }

    return 0;
}
