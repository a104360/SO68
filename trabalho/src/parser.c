#include "../include/parser.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Conta quantos comandos existem no pipeline passado como argumento
int countCommands(const char *argv) {
    if (argv == NULL || argv[0] == '\0')
        return 0;
    int count = 0;
    char *copy = strdup(argv);
    char *saveptr = NULL;
    char *token = strtok_r(copy, "|", &saveptr);
    while (token) {
        count++;
        token = strtok_r(NULL, "|", &saveptr);
    }
    free(copy);
    return count;
}

// Apresenta o comando na posicao N da pipeline de comandos
static int getIndexNCommand(const char *argv, int n) {
    if (n == 0)
        return 0;
    int size = strlen(argv);
    int count = 0;
    int i = 0;
    for (; i < size && count != n && argv[i] != '\0'; i++) {
        if (argv[i] == '|')
            count++;
    }
    if (argv[i] == '\0') // Correct the condition to check for end of string
        return 0;
    return i + 1;
}

// Separa cada comando do pipeline por ordem 
static char *cmdtok(const char *argv, int start, char *saveptr) {
    if (argv == NULL)
        return NULL;
    char *copy = strdup(argv + start); // Start from the given position
    if (saveptr == NULL)
        return strtok_r(copy, "|", &saveptr);
    saveptr = saveptr + 1;
    return strtok_r(NULL, "|", &saveptr);
}


char * parseUniqueCommand(const char * argv){
    return strdup(argv);
}

char ** parseMultipleCommands(const char * argv){
    if(argv == NULL) return NULL;
    int max = countCommands(argv);
    char ** args = malloc(sizeof(char *) * max);
    char * saveptr = NULL;
    for(int i = 0;i < max;i++){
        args[i] = cmdtok(argv,getIndexNCommand(argv,i),saveptr);
    }
    return args;
}

// Corrigida e testada
void getReply(int fd, char *reply) {
    lseek(fd, 0, SEEK_SET);
    char chunk[128 + 1];
    int bytesRead;
    int totalBytesRead = 0;

    
    while ((bytesRead = read(fd, chunk, 128)) > 0) {
        // Terminar o chunk com \0
        chunk[bytesRead] = '\0';

        // Copiar o chunk para o Array de reposta
        strcpy(reply + totalBytesRead, chunk);

        // Atualizar o total de bytes lidos
        totalBytesRead += bytesRead;
    }

    if (bytesRead == -1) {
        perror("Erro a ler o descritor de ficheiro");
    }
}