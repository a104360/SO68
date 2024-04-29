#include "../include/request.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

struct request {
    int id;
    int time;
    Command * commands;
    int argc;
};

Request * createRequest(int id,int time,Command * commands,int argc){
    Request * request = malloc(sizeof(struct request));
    request->id = id;
    request->time = time;
    request->argc = argc;
    request->commands = malloc(sizeof(char *) * argc);
    for(int i = 0;i < argc;i++){
        request->commands[i] = strdup(commands[i]);        
    }
    return request;
}

int getRid(Request * request){
    return request->id;
}

int getRtime(Request * request){
    return request->time;
}

int getNCommands(Request * request){
    return request->argc;
}

// Função não deixa memory leaks
void destroyRequest(Request * request){
    for(int i = 0;i < request->argc;i++){
        free(request->commands[i]);
        request->commands[i] = NULL;
    }
    free(request->commands);
    request->commands = NULL;
    free(request);
    request = NULL;
}

void printRequest(Request * r){
    printf("id : %d\ntime (usecs) : %d\nargc : %d\n",r->id,r->time,r->argc);
    for(int i = 0; i < r->argc;i++){
        printf("comando %d : %s\n",i,r->commands[i]);
    }
}

int writeRequest(const char * filename,Request * r){
    int fd = open(filename,O_CREAT | O_WRONLY,0666);
    
    lseek(fd,0,SEEK_END);

    write(fd,&(r->id),sizeof(int));
    write(fd,&(r->time),sizeof(int));
    write(fd,&(r->argc),sizeof(int));

    for(int i = 0;i < r->argc;i++){
        int length = strlen(r->commands[i]) + 1;
        write(fd,&length,sizeof(int));
        write(fd,r->commands[i],sizeof(char) * length);
    }

    close(fd);
}


// Função para navegar para o request no index arbitrário
static void skipToN(int fd,int index){
    lseek(fd,0,SEEK_SET);
    for(int i = 0;i < index;i++){
        lseek(fd,sizeof(int),SEEK_CUR);
        lseek(fd,sizeof(int),SEEK_CUR);
        int argc = 0;
        read(fd,&argc,sizeof(int));
        for(int j = 0;j < argc;j++){
            int size = 0;
            read(fd,&size,sizeof(int));
            lseek(fd,sizeof(char)*size,SEEK_CUR);
        }
    }
}

Request * readRequest(const char * filename,int index){
    int fd = open(filename,O_RDONLY);
    // skipar para a posicao desejada
    
    skipToN(fd,index);
    
    int id = 0;
    int time = 0;
    int argc = 0;
    read(fd,&id,sizeof(int));
    read(fd,&time,sizeof(int));
    read(fd,&argc,sizeof(int));
    
    Command * comandos = malloc(sizeof(char *) * (argc + 1));
    comandos[argc] = NULL;
    
    for(int i = 0;i < argc;i++){
        comandos[i] = NULL;
        int length = 0;
        read(fd,&length,sizeof(int));
        comandos[i] = malloc(sizeof(char) * length);
        read(fd,comandos[i],sizeof(char) * length);
    }

    close(fd);
    
    Request * r = createRequest(id,time,comandos,argc);
    
    for(int i = 0;i < argc;i++){
        if(comandos[i]) free(comandos[i]);
    }
    free(comandos);
    
    
    return r;
}


