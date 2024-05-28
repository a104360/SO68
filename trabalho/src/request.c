#include "../include/request.h"
#include "../include/linkedList.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/time.h>

struct request {
    int id;
    int time;
    char commands[REQUESTMAXSIZE];
};

Request * createRequest(int id,int time,Command commands){
    Request * request = malloc(sizeof(struct request));
    request->id = id;
    request->time = time;
    memset(request->commands,'\0',300);
    if(!commands) return request;
    strncpy(request->commands,commands,300);
    return request;
}

void setRid(Request * request,int id){
    request->id = id;
}

void setRtime(Request * request,int time){
    request->time = time;
}

int getRid(Request * request){
    return request->id;
}

int getRtime(Request * request){
    return request->time;
}

char * getRCommand(Request * request){
    if(request == NULL) return NULL;
    return strdup(request->commands);
}

// Função não deixa memory leaks
void destroyRequest(void * request){
    if(request) free(request);
    request = NULL;
}

void printRequest(Request * r){
    printf("id : %d\ntime (usecs) : %d\ncommand : %s\n",r->id,r->time,r->commands);
}

int writeRequest(const char * filename,Request * r){
    int fd = open(filename,O_CREAT | O_WRONLY,0666);
    if(fd == -1){
        perror("Não foi possível abrir o ficheiro corretamente\n");
        return -1;
    }
    
    write(fd,r,sizeof(struct request));
    
    close(fd);
    return 0;
}

int fdWriteRequest(int fd,Request * request){
    if(write(fd,request,sizeof(struct request))==-1) return -1;
    return 0;
}

/*
// Função para navegar para o request no index arbitrário
static void skipToN(int fd,int index){
    lseek(fd,0,SEEK_SET);
    for(int i = 0;i < index;i++){
        lseek(fd,sizeof(struct request),SEEK_CUR);
    }
}*/

Request * readRequest(const char * filename){
    int fd = open(filename,O_RDONLY);
    
    if(fd == -1){
        perror("o ficheiro não foi corretamente aberto\n");
        return NULL;
    }
    
    // skipar para a posicao desejada
    //if(index >= 0) skipToN(fd,index);
    
    Request * r = malloc(sizeof(struct request));
    if(read(fd,r,sizeof(struct request)) == -1){
        perror("EOF encontrado");
        return NULL;
    }

    close(fd);
    
    return r;
}

Request * fdReadRequest(int fd){    
    Request * r = malloc(sizeof(struct request));
    if(read(fd,r,sizeof(struct request)) == -1){
        perror("EOF encontrado");
        return NULL;
    }
    return r;
}

void * copyRequest(Request * request){
    void * obj = malloc(sizeof(struct request));
    Request * new = (Request *) obj;
    new->id = request->id;
    new->time = request->time;
    strncpy(new->commands,request->commands,REQUESTMAXSIZE);
    return obj;
}


int compareRequest(void *a,void *b){
    if(!a || !b) return -10;
    Request * r1 = (Request *) a;
    Request * r2 = (Request *) b;

    if(r1->time == r2->time) return 0;

    return r1->time > r2->time ? 1 : -1;
}


void executeRequest(Request * request){
    char * argv = getRCommand(request);
    if(argv == NULL){
        return;
    }

    int nCommands = countCommands(argv);
    Query *commands = parsePipe(argv);
    free(argv);
    
    int oldPipe[2], newPipe[2];
    // Para cada comando, executar o ciclo
    struct timeval * start;
    struct timeval * end;
    gettimeofday(start,NULL);


    for (int i = 0; i < nCommands; i++) {
        // Em todos os comandos, anteriores ao ultimo, cria-se um pipe
        if (i < nCommands - 1) {
            pipe(newPipe);
        }   

        if (fork() == 0) {
            if (i > 0) {  // Assim não executa o primeiro
                dup2(oldPipe[0], STDIN_FILENO);
                close(oldPipe[0]);
                close(oldPipe[1]);
            }
            if (i < nCommands - 1) {  // Assim não vai executar o último comando
                dup2(newPipe[1], STDOUT_FILENO);
                close(newPipe[0]);
                close(newPipe[1]);
            }
            execvp(commands[i][0], commands[i]);
            perror("execvp falhou");
            _exit(-1);
        } // Adicionar aqui verificação de erro

        // O pai fecha os file descriptors do pipe antigo
        if (i > 0) {
            close(oldPipe[0]);
            close(oldPipe[1]);
        }
        // Para todos os comandos, exceto o último,
        // cada descritor torna-se os respetivos novos descritores
        if (i < nCommands - 1) {
            oldPipe[0] = newPipe[0];
            oldPipe[1] = newPipe[1];
        }

        // Espera para que o comando execute
        //wait(NULL);
    }
    // Wait for all child processes to prevent zombies
    for (int i = 0; i < nCommands; i++) {
        wait(NULL);
    }
    gettimeofday(end,NULL);
    long duration = end->tv_usec - start->tv_usec;
    // Libertar memória da Query
    freeCmdPipeline(&commands);
    reportCompletedRequest(request,duration);
}


char * requestToCompleteStatus(Request * r,long time){
    int statusSize = COMPLETEREQUESTSIZE;
    char * status = malloc(sizeof(char) * statusSize);
    memset(status,'\0',statusSize);
    snprintf(status,statusSize,"%d %s %d ms %ld ms\n",r->id,r->commands,r->time,time);
    return status;
}

char * requestToNormalStatus(Request * r){
    int statusSize = NORMALREQUESTSIZE;
    char * status = malloc(sizeof(char) * statusSize);
    memset(status,'\0',statusSize);
    snprintf(status,statusSize,"%d %s %d ms\n",r->id,r->commands,r->time);
    return status;
}

Request * statusToRequest(int fd){
    char * string = malloc(NORMALREQUESTSIZE);
    read(fd,string,NORMALREQUESTSIZE);
    char * token = NULL;
    char * saveptr = string;
    token = strtok_r(string," ",&saveptr);
    int id = atoi(token);
    token = strtok_r(NULL," ",&saveptr);
    char * comand = strdup(token);
    token = strtok_r(NULL," ",&saveptr);
    int time = atoi(token);
    if(string) free(string);
    Request * r = createRequest(id,time,comand);
    free(comand);
    return r;
}


static char * readCompleteStatus(int fd){
    char * final = malloc(COMPLETEREQUESTSIZE * sizeof(char));
    read(fd,final,COMPLETEREQUESTSIZE);
    return final;
}

static char * readNormalStatus(int fd){
    char * final = malloc(sizeof(char) * NORMALREQUESTSIZE);
    read(fd,final,NORMALREQUESTSIZE);
    return final;
}


void writeStatusToUser(int exec,int sched,int comp,int user){
    char * r = NULL;
    //Executing
    if(exec > 0){
        lseek(exec,0,SEEK_SET);
        write(user,"Executing\n",sizeof(char) * 11);
        while((r = readNormalStatus(exec))){
            write(user,r,strlen(r));
            free(r);
            r = NULL;
        }
    }
    //Scheduled
    if(sched > 0){
        lseek(sched,0,SEEK_SET);
        write(user,"Scheduled\n",sizeof(char) * 11);
        while((r = readNormalStatus(sched))){
            write(user,r,strlen(r));
            free(r);
            r = NULL;
        }
    }
    //Completed
    if(comp > 0){
        lseek(comp,0,SEEK_SET);
        write(user,"Completed\n",sizeof(char) * 11);
        while((r = readCompleteStatus(comp))){
            write(user,r,strlen(r));
            free(r);
            r = NULL;
        }
    }
}


void reportCompletedRequest(Request * r,long time){
    char * junction = requestToCompleteStatus(r,time);
    int fd = open("../tmp/completed",O_CREAT | O_RDWR | O_APPEND,0666);
    write(fd,junction,COMPLETEREQUESTSIZE);
    free(junction);
    close(fd);
}

void setRequestToExecuting(Request * r){
    char * string = requestToNormalStatus(r);
    int fd = open("../tmp/executing",O_CREAT | O_RDWR | O_APPEND,0666);
    write(fd,string,NORMALREQUESTSIZE);
    close(fd);
}

void listToSchedule(LinkedList ** l){
    int fd = open("../tmp/schedule",O_CREAT | O_RDWR | O_TRUNC,0666);
    while(*l){
        Request * r = popFront(l,destroyRequest);
        char * string = requestToNormalStatus(r);
        destroyRequest(r);
        write(fd,string,NORMALREQUESTSIZE);
        free(string);
        string = NULL;
    }
    close(fd);
}


void updateScheduleFile(LinkedList * l){
    int fd = open("../tmp/schedule",O_RDWR,0666);
    lseek(fd,0,SEEK_SET);
    char * buffer = malloc(NORMALREQUESTSIZE);
    Request * r = NULL;
    while((r=statusToRequest(fd))){
        orderInsert(&l,(void *)r,compareRequest);
        destroyRequest(r);
    }
    close(fd);
}