#include "../include/request.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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

int writeRequest(int fifo,Request * request){

    if(fifo == -1) return -1;

    if(write(fifo,request,sizeof(struct request)) == -1){
        perror("Escrita falhada");
        close(fifo);
        return -1;
    }

    close(fifo);

    return 0;
}