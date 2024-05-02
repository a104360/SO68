#include "../include/request.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static const char clientID[] = "../tmp/clientID.qjm";

static const char fifoComun[] = "../tmp/requestCenter";

int main(){

    // INICIAR ARMAZENAMENTO DE REQUESTS

    //lseek(fd,0,SEEK_SET);

    Request * r = readRequest(fifoComun,0);

    printRequest(r);

    destroyRequest(r);


    // TER UM FIFO PARA LER OS REQUESTS

    // ENVIAR ID AO CLIENTE

    // ANALIZAR OU NAO O TEMPO DE CADA UM

    // EXECUTAR 

    // ESCREVER OS OUTPUTS

    // VOLTAR A LER REQUESTS 

    return 0;
}