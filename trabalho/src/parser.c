#include "../include/parser.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int countCommands(const char * cmd){
    if(!cmd || cmd[0] == '\0') return 0;
    int count = 1;
    for(int i = 0;cmd[i] != '\0';i++){
        if(cmd[i] == '|' && cmd[i+1] >= ' ' && cmd[i+1] <= '~') 
            count++;
    }
    return count;
}

static int countTokens(const char * cmd){
    if(!cmd || cmd[0] == '\0') return 0;
    int count = 1;
    short first = 0;
    for(int i = 0;cmd[i] != '\0' && cmd[i] != '|' && cmd[i+1] != '|';i++){
        if(cmd[i] == ' '){
            if(first == 0) continue;
            if(cmd[i+1] > ' ' && cmd[i+1] <= '~')
                count++;
            continue;
        }
        if(first == 0){
            first = 1;
        }
    }
    return count;
}



char ** cmdTok(const char * cmd){
    if(!cmd || cmd[0] == '\0') return NULL;
    char * copy = strdup(cmd);
    char * token = NULL;
    char * saveptr = copy;
    
    int size = countTokens(cmd) + 1;
    
    char ** arraycmds = malloc(sizeof(char *) * size);
    
    token = strtok_r(copy," |\0",&saveptr);
    
    arraycmds[0] = strdup(token);
    
    for(int i = 1;i < size-1;i++){
        token = strtok_r(NULL," |\0",&saveptr);
        if(token == NULL && i < size - 1) token = strtok_r(NULL," |\0",&saveptr);
        arraycmds[i] = strdup(token);
    }
    
    if(copy) free(copy);
    arraycmds[size - 1] = NULL;
    return arraycmds;
}

char *** parsePipe(const char * cmdPipe){
    if(!cmdPipe || (cmdPipe[0] < ' ' || cmdPipe[0] > '~')) return NULL;
    
    char * cmdPipeCopy = strdup(cmdPipe);
    char * cmdPipeToken = NULL;
    char * saveptr = cmdPipeCopy;
    int parseArraySize = countCommands(cmdPipe) + 1; // Determinar quantos comandos vao ser executados
    
    char *** parseArray = malloc(sizeof(char **) * parseArraySize);
    
    cmdPipeToken = strtok_r(cmdPipeCopy,"|\0\n",&saveptr);

     parseArray[0] = cmdTok(cmdPipeToken);
    
    // Ciclo itera por quantos comandos houver
    for(int i = 1;i < parseArraySize;i++){
        cmdPipeToken = strtok_r(NULL,"|\0\n",&saveptr);
        parseArray[i] = cmdTok(cmdPipeToken);
    }
    if(cmdPipeCopy) free(cmdPipeCopy);
    parseArray[parseArraySize - 1] = NULL;
    return parseArray;
}

void printQuery(Query cmd){
    int i = 0;
    while(cmd[i] != NULL){
        printf("%s ",cmd[i]);
        i++;
    }
    putchar('\n');
}


void freeQuery(Query tok) {
    int i = 0;
    while(tok[i]){
        free(tok[i]);
        i++;
    }
    free(tok);
}


void freeCmdPipeline(Query ** pipeline){
    for(int i = 0;(*pipeline)[i];i++){
        freeQuery((*pipeline)[i]);
    }
    free(*pipeline);
    *pipeline = NULL;
}

int writeTaskReport(int id,long time){
    const char logFile[] = "../tmp/taskReport";

    char line[25];
    memset(line,'\0',25);
    snprintf(line,25,"ID : %d\ntime : %ld\n",id,time);
    
    int fd = open(logFile,O_CREAT | O_WRONLY,0666);

    lseek(fd,0,SEEK_END);

    if(write(fd,line,25) == -1){
        perror("Escrita no ficheiro falhou");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

char * nameFifo(int pid){
	char * nome = malloc(sizeof(char) * REPLYSIZE);

	snprintf(nome,REPLYSIZE,"../tmp/replyTo_%d",pid);

	return nome;
}


void writeReply(int fd,int id){
    char * resposta = malloc(sizeof(char) * 25);
    snprintf(resposta,25,"REQUEST %d Received",id);
    write(fd,resposta,25);
    free(resposta);
}

