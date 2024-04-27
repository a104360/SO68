#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "../include/parser.h"
#include <stdlib.h>

int main(int argc,char ** argv){
    // Testes para comandos unicos

    char * comandoUnico = parseUniqueCommand(argv[4]);

    printf("Primeiro comando unico : %s\n", comandoUnico);

    free(comandoUnico);


    printf("\n\n\nPIPELINES DE COMANDOS\n\n");

    // Testes para comandos em pipe

    char ** pipeline = parseMultipleCommands(argv[4]);
    int size = countCommands(argv[4]);

    printf("numero de comandos : %d\n\n\n",size);
    
    for(int i = 0;i < size;i++){
        printf("Comando no index %d : %s\n",i,pipeline[i]);
        free(pipeline[i]);
    }
    free(pipeline);


    printf("\n\n\nLEITURA DE REPLIES\n\n");

    int fd = open("c10",O_RDONLY); 
    if(fd == -1){
        perror("Não abriu corretamente");
    }


    return 0;
}