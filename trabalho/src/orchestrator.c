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


int executing = 0;


void execute(char * argv,Request * r){
    int argv1Size = strlen(argv);
    int id = getRid(r);
    int outputNameSize = 10 + argv1Size;
    char outputName[outputNameSize];
    memset(outputName,'\0',outputNameSize);
    if(argv[argv1Size-1] == '/')snprintf(outputName,outputNameSize,"%s%d",argv,id);
    else snprintf(outputName,outputNameSize,"%s/%d",argv,id);


    // Abrir o ficheiro e verificar se não ocorreu erro
    int fdOutput = open(outputName,O_CREAT | O_WRONLY,0666);
    if(fdOutput == -1){
        perror("Erro na criação/abertura do ficheiro de output");
        return;
    }


    // Redirecionar o standard output/error
    // para o ficheiro respetivo 
    dup2(fdOutput,STDOUT_FILENO);
    dup2(fdOutput,STDERR_FILENO);

    executeRequest(r);
    close(fdOutput);
}

void checkAndExecute(char * argv,int max,LinkedList ** list){
    // Enquanto o número de processos a executar 
    // não exceder o máximo imposto
    while(executing < max){
        Request * r = pop(list);
        if(r == NULL){
          break;  // Não existem mais requests para executar
        }
        if(fork() == 0){
            execute(argv,r); // Executar o request 
            _exit(0);
        }
        printf("Processo %d a executar");
        executing++; // Aumentar o numero de processos a executar
        destroyRequest(r); // Destruir o request 
    }
}


void monitorChilds(char * argv,int max,LinkedList ** list){
    int status; // Status holder variable 

    // While there are processes executing
    while(executing > 0){ 
        // Wait for any process 
        // But does is not left hanging
        // If the child is has not ended, it returns 0, not being left hanging
        // If the child has ended, it returns the pid of the son
        pid_t pid = waitpid(-1,&status,WNOHANG); 
        // If the child has finished 
        if(pid > 0){
            // Descrease the executing childs
            executing--;
            // Check if there are room for one more process and execute it
            checkAndExecute(argv,max,list);
        } else if(pid == 0){ // If the child has not finished
            break; // Just break the cycle and exit the function
        } else { // In case of any other outcome to the wait
            perror("waitpid"); // Just display a error message
        }
    }
}


static const char fifoComum[] = "../tmp/requestCenter";

int main(int argc,char ** argv){ // ./orchestrator output_folder parallel-tasks sched-policy

    if(argc <= 2){
        perror("Parametros de inicialização inválidos");
        return -1;
    }
    
    int numOfSons = atoi(argv[2]); // Obter o numero de filhos maximos

    mkfifo(fifoComum,0666); // Criar o FIFO para o servidor receber os pedidos dos clientes
    
    LinkedList * holder = createLinkedList(); // Criar a estrutura para armazenar os pedidos em espera
    
    int listenFifo = open(fifoComum,O_CREAT | O_RDONLY); // Abrir o fifo para leitura (bloqueia até alguém escrever)

    // Ciclo principal
    while (1) 
    {
        Request * r = fdReadRequest(listenFifo); // Ler o request do FIFO
        

        // Caso não tenha sido lido nenhum pedido 
        if(r == NULL){ 
            close(listenFifo); // Garantir que o descritor está fechado depois de receber o EOF
            listenFifo = open(fifoComum,O_RDONLY); // Abrir outra vez o descritor
            continue; // Não executar o resto do ciclo, para voltar a ler o request 
        }
        
        int clientId = getRid(r); // Obter do Id do cliente
        
        // Caso o cliente queira desligar o servidor (executar o cliente sem mais argumentos)
        if(clientId == -404){ 
            destroyLinkedList(holder,destroyRequest); // Destruir a estrutura de dados
            destroyRequest(r); // Destruir o request de encerramento do servidor
            break; // Sair do ciclo eterno
        }

        // Inicio da resposta ao cliente
        char * reply = nameFifo(clientId); // Nomear um named pipe com id cliente
        int fdReply = open(reply,O_WRONLY); // Abrir para escrita
        writeReply(fdReply,clientId); // Escrita da resposta
        close(fdReply); // Fecho do descritor de ficheiro
        free(reply); // Libertação de memória
        // Resposta ao cliente concluida 


        append(holder,(void *) r,copyRequest); // Colocar o request na lista de espera
        monitorChilds(argv[1],numOfSons,&holder); // Verificar se os filhos estão 
        checkAndExecute(argv[1],numOfSons,&holder);
    }
    

    close(listenFifo); // Fechar o FIFO de leitura dos pedidos 
    unlink(fifoComum); // Remover o FIFO do sistema de ficheiros

    return 0;
}
