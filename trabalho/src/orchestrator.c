#include "../include/request.h"
#include "../collections/linkedList.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>

static const char fifoComum[] = "../tmp/requestCenter";

int main(int argc,char ** argv){

    // INICIAR ARMAZENAMENTO DE REQUESTS
    

    // Inicializar o controlador
    int fdController[2];
    pipe(fdController);
    if(fork() == 0){
        LinkedList * queriesOnHold = createLinkedList();


        _exit(0);
    }

    //lseek(fd,0,SEEK_SET);
    mkfifo(fifoComum,0666);
    int listenFifo = open(fifoComum,O_RDONLY);

    while(1){
        read
    }

    // TER UM FIFO PARA LER OS REQUESTS

    // ENVIAR ID AO CLIENTE

    // ANALIZAR OU NAO O TEMPO DE CADA UM

    // EXECUTAR

    // ESCREVER OS OUTPUTS

    // VOLTAR A LER REQUESTS 

    unlink(fifoComun);

    return 0;
}