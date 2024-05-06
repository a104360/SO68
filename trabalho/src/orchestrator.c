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

int main(int argc,char ** argv){ // ./orchestrator output_folder parallel-tasks sched-policy

    // INICIAR ARMAZENAMENTO DE REQUESTS

    if(argc <= 1){
        perror("Parametros de inicialização inválidos");
        return -1;
    }
    
    int numOfSons = atoi(argv[2]);

    // Inicializar o controlador
    int fdServerController[2];
    pipe(fdServerController);
    if(fork() == 0){
        
        /************************************************
         * CÓDIGO RESPETIVO AO CONTROLLER
        ***********************************************/

        // Fechar o descritor de escrita no pipe
        close(fdServerController[1]); 

        // Inicializar a lista de queries em espera
        //LinkedList * queriesOnHold = createLinkedList(); 

        int fdControllerTasks[2];
        pipe(fdControllerTasks);
        
        /****************************************************
         * Ciclo para os filhos que executam as tarefas
        ***************************************************/
        for(int i = 0;i < numOfSons;i++){
            if(fork() == 0){
                close(fdControllerTasks[1]);
                while(1){
                    //read()
                    // Primeiro verificar se existe algum request em espera
                    // Senão, ler do fifo
                    Request * request = NULL;
                    //request = popFront(&queriesOnHold,destroyRequest);
                    //if(request == NULL)
                    request = fdReadRequest(fdControllerTasks[0]); // Bloqueado na leitura
                    

                    
                    // VERIFICAÇÃO PARA A MENSAGEM DE TURNOFF DO SERVIDOR
                    // O request para desligar deve possuir o id -404
                    int id = getRid(request);
                    if(id == -404){
                        destroyRequest((void *)request);
                        close(fdControllerTasks[0]);
                        //destroyLinkedList(queriesOnHold,destroyRequest);
                        break;
                    }
                    
                    // Obter a mensagem
                    char * command = getRCommand(request);
                    destroyRequest(request);
                    // Fazer parse ao pipeline de comandos a executar
                    Query * argv = parsePipe(command);
                    if(command) free(command);

                    // Obter a designação do ficheiro de output
                    char outputName[10];
                    memset(outputName,'\0',10);
                    snprintf(outputName,10,"%d",id);


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
                        // Executar o pipeline extensível
                        execvp(argv[0][0],argv[0]);
                        write(STDERR_FILENO,"Erro na execução do request\n",31);
                        freeCmdPipeline(&argv);
                        _exit(-1);
                    }

                    // Libertar o pipeline;
                    freeCmdPipeline(&argv);

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
                fdWriteRequest(fdControllerTasks[1],readBuffer);
                destroyRequest(readBuffer);
                break;
            }
            // ADICIONAR CONDIÇÃO PARA ALTERAR A POLITICA
            //orderInsert(&queriesOnHold,readBuffer,compareRequest);
        }

        for(int i = 0;i < numOfSons;i++) wait(NULL);
        close(fdServerController[0]);
        close(fdControllerTasks[1]);
        _exit(0);


    /******************************************************
     * FIM DO CÓDIGO DO CONTROLLER
    *****************************************************/
    }


    close(fdServerController[0]);
    //lseek(fd,0,SEEK_SET);
    mkfifo(fifoComum,0666);
    int listenFifo = open(fifoComum,O_RDONLY);


    while(1){
        Request * r = fdReadRequest(listenFifo);
        int clientId = getRid(r);
        if(fork() == 0){
            char * reply = nameFifo(clientId);
            mkfifo(reply,0666);
            int fdReply = open(reply,O_WRONLY);
            writeReply(fdReply,clientId);
            close(fdReply);
            unlink(reply);
            free(reply);
            _exit(0);
        }
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