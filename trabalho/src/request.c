#include "../include/request.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

static const char fifoComun[] = "../tmp/requestCenter";

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
    return strdup(request->commands);
}

// Função não deixa memory leaks
void destroyRequest(Request * request){
    free(request);
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


// Função para navegar para o request no index arbitrário
static void skipToN(int fd,int index){
    lseek(fd,0,SEEK_SET);
    for(int i = 0;i < index;i++){
        lseek(fd,sizeof(struct request),SEEK_CUR);
    }
}

Request * readRequest(const char * filename,int index){
    int fd = open(filename,O_RDONLY);
    
    // skipar para a posicao desejada
    if(index >= 0) skipToN(fd,index);
    
    Request * r = malloc(sizeof(struct request));
    
    read(fd,r,sizeof(struct request));

    close(fd);
    
    return r;
}


