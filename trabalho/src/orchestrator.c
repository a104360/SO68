#include "../include/request.h"
#include "../include/linkedList.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>

static const char fifoComum[] = "../tmp/requestCenter";
static const char completed[] = "../tmp/completed";
static const char scheduled[] = "../tmp/schedule";
static const char executing[] = "../tmp/executing";

int main(int argc,char ** argv){ // ./orchestrator output_folder parallel-tasks sched-policy

    // INICIAR ARMAZENAMENTO DE REQUESTS

    if(argc <= 2){
        perror("Parametros de inicialização inválidos");
        return -1;
    }
    
    int numOfSons = atoi(argv[2]);
    if(numOfSons <= 0) return -1;
    
    // Inicializar o controlador
    int fdServerController[2];
    pipe(fdServerController);
    if(fork() == 0){
        
        /************************************************
         * 
         * CÓDIGO RESPETIVO AO CONTROLLER
         * 
        ***********************************************/

        // Fechar o descritor de escrita no pipe
        close(fdServerController[1]); 

        // Inicializar a lista de queries em espera
        //LinkedList * queriesOnHold = createLinkedList(); 
        /*int fdTaskCtrlFile[2];
        pipe(fdTaskCtrlFile);
        int fdControllerCtrlFile[2];
        pipe(fdControllerCtrlFile);
        if(fork() == 0){ 
            /********************************************************
             * 
             * CÓGIDO RESPETIVO AO CONTROL FILE
             * 
            *********************************************************
            close(fdTaskCtrlFile[1]);
            close(fdControllerCtrlFile[1]);
            int n = numOfSons;
            while(1){
                int buffer = 0;
                if(fork() == 0){
                    if(read)
                }
                if(read(fdTaskCtrlFile[0],&buffer,sizeof(int))){
                    n++;
                    writeTaskerStatus(n);
                }
            }

            /********************************************************
             * 
             * FIM CONTROL FILE
             * 
            *********************************************************
        }*/

        int fdCompleted = open(completed,O_CREAT | O_RDWR | O_APPEND,0666);
        int fdScheduled = open(scheduled,O_CREAT | O_RDWR | O_TRUNC,0666);
        int fdExecuting = open(executing,O_CREAT | O_RDWR,0666);


        int fdControllerTasks[2];
        pipe(fdControllerTasks);
        
        /****************************************************
         * 
         * Ciclo para os filhos que executam as tarefas
         * 
        ***************************************************/
        for(int i = 0;i < numOfSons;i++){
            if(fork() == 0){
                close(fdControllerTasks[1]);
                //close(fdTaskCtrlFile[0]);
                while(1){
                    //read()
                    // Primeiro verificar se existe algum request em espera
                    // Senão, ler do fifo
                    Request * request = NULL;
                    //request = popFront(&queriesOnHold,destroyRequest);
                    //if(request == NULL)
                    request = fdReadRequest(fdControllerTasks[0]); // Bloqueado na leitura
                    
                    //fdWriteRequest();
                    
                    // VERIFICAÇÃO PARA A MENSAGEM DE TURNOFF DO SERVIDOR
                    // O request para desligar deve possuir o id -404
                    int id = getRid(request);
                    if(id == -404){
                        destroyRequest((void *)request);
                        close(fdControllerTasks[0]);
                        close(fdCompleted);
                        close(fdScheduled);
                        close(fdExecuting); 
                        //destroyLinkedList(queriesOnHold,destroyRequest);
                        break;
                    }
                    

                    // Obter a designação do ficheiro de output
                    int argv1Size = strlen(argv[1]);
                    int outputNameSize = 10 + argv1Size;
                    char outputName[outputNameSize];
                    memset(outputName,'\0',outputNameSize);
                    if(argv[1][argv1Size-1] == '/')snprintf(outputName,outputNameSize,"%s%d",argv[1],id);
                    else snprintf(outputName,outputNameSize,"%s/%d",argv[1],id);


                    // Abrir o ficheiro e verificar se não ocorreu erro
                    int fdOutput = open(outputName,O_CREAT | O_WRONLY,0666);
                    if(fdOutput == -1){
                        perror("Erro na criação/abertura do ficheiro de output");
                        
                        continue;
                    }


                    // Redirecionar o standard output/error
                    // para o ficheiro respetivo 
                    dup2(fdOutput,STDOUT_FILENO);
                    dup2(fdOutput,STDERR_FILENO);


                    if(fork() == 0){
                        executeRequest(request);
                        //write(STDERR_FILENO,"Erro na execução do request\n",31);
                        _exit(0);
                    }

                    // Libertar o pipeline;

                    wait(NULL); // Esperar que o processo termine
                    close(fdOutput);

                    // Avisar o orchestrator que um request/task foi concluído
                    /*
                    Request * reqTer = createRequest(-10,-10,NULL);
                    writeRequest(fifoComum,reqTer);
                    destroyRequest(reqTer);
                    */
                }
                close(fdControllerTasks[0]);
                //destroyLinkedList(queriesOnHold,destroyRequest);
                _exit(0);
            }
        }
        //*************************************************
        close(fdControllerTasks[0]);

        while(1){
            Request * readBuffer;
            readBuffer = fdReadRequest(fdServerController[0]);
            if(getRid(readBuffer) == -404){
                for(int w = 0;w < numOfSons;w++) fdWriteRequest(fdControllerTasks[1],readBuffer);
                destroyRequest(readBuffer);
                close(fdCompleted);
                close(fdScheduled);
                close(fdExecuting); 
                break;
            }
            fdWriteRequest(fdControllerTasks[1],readBuffer);
            destroyRequest(readBuffer);


            // ADICIONAR CONDIÇÃO PARA ALTERAR A POLITICA
            //orderInsert(&queriesOnHold,readBuffer,compareRequest);
        }

        for(int i = 0;i < numOfSons;i++) wait(NULL);
        close(fdServerController[0]);
        close(fdControllerTasks[1]);
        close(fdCompleted);
        close(fdScheduled);
        close(fdExecuting); 
        _exit(0);


    /******************************************************
     * FIM DO CÓDIGO DO CONTROLLER
    *****************************************************/
    }


    close(fdServerController[0]);
    //lseek(fd,0,SEEK_SET);
    mkfifo(fifoComum,0666);
    int listenFifo = open(fifoComum,O_RDWR);

    // Ciclo eterno do servidor
    while(1){
        // Ler um request do fifo
        Request * r = fdReadRequest(listenFifo);
        // Guardar id do cliente
        int clientId = getRid(r);
        if(fork() == 0){
            char * reply = nameFifo(clientId);
            int fdReply = open(reply,O_WRONLY);
            writeReply(fdReply,clientId);
            close(fdReply);
            free(reply);
            _exit(0);
        }
        wait(NULL);
        fdWriteRequest(fdServerController[1],r);
        if(clientId == -404){
            destroyRequest(r);
            break;
        }
        destroyRequest(r);
    }

    close(listenFifo);
    unlink(fifoComum);

    return 0;
}